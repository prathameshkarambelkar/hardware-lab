#include <Arduino.h>
#include <Wire.h>
#include <Preferences.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PWMServoDriver.h>
#include "esp_system.h"

constexpr uint8_t PIN_SDA = 21, PIN_SCL = 22;
constexpr uint8_t ADDR_OLED = 0x3C, ADDR_PCA = 0x40;
constexpr uint8_t CH_S1 = 0, CH_S2 = 1;
constexpr uint16_t US_MIN = 1000, US_MAX = 2000, US_MID = 1500;
constexpr uint32_t SWEEP_PERIOD_MS = 3000;
constexpr uint32_t STAGGER_MS = 60;
constexpr uint32_t COUNTDOWN_MS = 10000;

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Adafruit_PWMServoDriver pca(ADDR_PCA, Wire);
Preferences prefs;

bool oledOk = false, pcaOk = false, running = true;
uint32_t bootCount = 0, i2cErrOled = 0, i2cErrPca = 0, cycles = 0;
const char *resetName = "?";

enum Phase { COUNTDOWN, S1_ONLY, S2_ONLY, BOTH, STOPPED };
Phase phase = COUNTDOWN;
uint32_t phaseStart = 0;
uint16_t us1 = 0, us2 = 0;

const char *phaseName(Phase p) {
  switch (p) {
    case COUNTDOWN: return "WAIT";
    case S1_ONLY:   return "SERVO 1";
    case S2_ONLY:   return "SERVO 2";
    case BOTH:      return "BOTH";
    default:        return "STOPPED";
  }
}

const char *resetReasonName(esp_reset_reason_t r) {
  switch (r) {
    case ESP_RST_POWERON:  return "POWER-ON";
    case ESP_RST_SW:       return "SOFTWARE";
    case ESP_RST_PANIC:    return "CRASH";
    case ESP_RST_INT_WDT:
    case ESP_RST_TASK_WDT:
    case ESP_RST_WDT:      return "WATCHDOG";
    case ESP_RST_BROWNOUT: return "BROWNOUT!";
    case ESP_RST_EXT:      return "EN BUTTON";
    case ESP_RST_DEEPSLEEP:return "DEEPSLEEP";
    default:               return "OTHER";
  }
}

bool ping(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

void scanI2C() {
  Serial.print("I2C scan:");
  int n = 0;
  for (uint8_t a = 1; a < 127; a++) {
    if (ping(a)) { Serial.printf(" 0x%02X", a); n++; }
  }
  Serial.printf("  (%d found; expect 0x3C OLED, 0x40 PCA9685, 0x70 normal)\n", n);
}

void servoWrite(uint8_t ch, uint16_t us) {
  if (!pcaOk) return;
  if (us == 0) pca.setPWM(ch, 0, 4096);
  else         pca.writeMicroseconds(ch, us);
}

void allOff() { us1 = us2 = 0; servoWrite(CH_S1, 0); servoWrite(CH_S2, 0); }

uint16_t sweepUs(int32_t t) {
  if (t < 0) return US_MID;
  uint32_t p = (uint32_t)(t + SWEEP_PERIOD_MS / 4) % SWEEP_PERIOD_MS;
  uint32_t half = SWEEP_PERIOD_MS / 2;
  uint32_t span = US_MAX - US_MIN;
  return (p < half) ? US_MIN + span * p / half : US_MAX - span * (p - half) / half;
}

void setPhase(Phase p) {
  phase = p; phaseStart = millis();
  Serial.printf("--> phase: %s\n", phaseName(p));
}

void runServos() {
  uint32_t t = millis() - phaseStart;
  switch (phase) {
    case COUNTDOWN:
      if (t >= COUNTDOWN_MS) setPhase(S1_ONLY);
      return;
    case S1_ONLY:
      us1 = sweepUs(t); us2 = 0;
      if (t >= 2 * SWEEP_PERIOD_MS) { us1 = US_MID; setPhase(S2_ONLY); }
      break;
    case S2_ONLY:
      us1 = US_MID; us2 = sweepUs(t);
      if (t >= 2 * SWEEP_PERIOD_MS) { us2 = US_MID; setPhase(BOTH); }
      break;
    case BOTH:
      us1 = sweepUs(t); us2 = sweepUs((int32_t)t - STAGGER_MS);
      if (t >= 5 * SWEEP_PERIOD_MS) { cycles++; setPhase(S1_ONLY); }
      break;
    case STOPPED:
      return;
  }
  servoWrite(CH_S1, us1);
  servoWrite(CH_S2, us2);
}

void drawStatus() {
  if (!oledOk) return;
  uint32_t up = millis() / 1000;
  char line[32];
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.printf("Boot #%lu %s\n", (unsigned long)bootCount, resetName);
  oled.printf("PCA:%s I2Cerr:%lu\n", pcaOk ? "OK" : "MISSING", (unsigned long)(i2cErrOled + i2cErrPca));
  oled.drawFastHLine(0, 18, 128, SSD1306_WHITE);
  oled.setCursor(0, 22);
  oled.setTextSize(2);
  oled.println(phaseName(phase));
  oled.setTextSize(1);
  if (phase == COUNTDOWN) {
    uint32_t el = millis() - phaseStart;
    uint32_t left = el >= COUNTDOWN_MS ? 0 : (COUNTDOWN_MS - el) / 1000 + 1;
    snprintf(line, sizeof line, "Switch ON now  %lus", (unsigned long)left);
    oled.println(line);
  } else {
    oled.printf("S1:%4u  S2:%4u us\n", us1, us2);
  }
  oled.printf("Cycles:%lu Up %02lu:%02lu", (unsigned long)cycles, (unsigned long)(up / 60), (unsigned long)(up % 60));
  oled.display();
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== ESP32 servo bring-up test ===");
  esp_reset_reason_t rr = esp_reset_reason();
  resetName = resetReasonName(rr);
  prefs.begin("bringup", false);
  bootCount = prefs.getUInt("boots", 0) + 1;
  prefs.putUInt("boots", bootCount);
  Serial.printf("Boot #%lu, reset reason: %s\n", (unsigned long)bootCount, resetName);
  if (rr == ESP_RST_BROWNOUT) Serial.println("!! Last reset was a BROWNOUT");

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);
  scanI2C();

  oledOk = oled.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED);
  Serial.printf("OLED 0x3C: %s\n", oledOk ? "OK" : "NOT FOUND");
  pcaOk = ping(ADDR_PCA);
  Serial.printf("PCA9685 0x40: %s\n", pcaOk ? "OK" : "NOT FOUND");
  if (pcaOk) {
    pca.begin();
    pca.setOscillatorFrequency(27000000);
    pca.setPWMFreq(50);
    allOff();
    setPhase(COUNTDOWN);
  } else {
    phase = STOPPED; running = false;
  }
  Serial.println("Commands: s = stop, g = go, r = rescan I2C");
}

void loop() {
  static uint32_t tServo = 0, tPing = 0, tDraw = 0, tLog = 0;
  uint32_t now = millis();
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 's') { allOff(); setPhase(STOPPED); running = false; }
    if (c == 'g' && pcaOk) { running = true; setPhase(S1_ONLY); }
    if (c == 'r') scanI2C();
  }
  if (running && now - tServo >= 20) { tServo = now; runServos(); }
  if (now - tPing >= 500) {
    tPing = now;
    if (oledOk && !ping(ADDR_OLED)) i2cErrOled++;
    if (pcaOk && !ping(ADDR_PCA)) i2cErrPca++;
  }
  if (now - tDraw >= 250) { tDraw = now; drawStatus(); }
  if (now - tLog >= 1000) {
    tLog = now;
    Serial.printf("[%5lus] boot#%lu rst=%s phase=%-7s S1=%4u S2=%4u i2cErr oled=%lu pca=%lu cycles=%lu\n",
                  (unsigned long)(now / 1000), (unsigned long)bootCount, resetName, phaseName(phase),
                  us1, us2, (unsigned long)i2cErrOled, (unsigned long)i2cErrPca, (unsigned long)cycles);
  }
}
