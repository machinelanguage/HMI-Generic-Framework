#include <Arduino.h>
#include "HmiFramework.h"

// Replace this adapter with TFT_eSPI, LovyanGFX, Adafruit_GFX, or an HMI UART adapter.
class SerialRenderer final : public hmi::Renderer {
 public:
  void beginFrame() override { Serial.println("[HMI] frame begin"); }
  void endFrame() override { Serial.println("[HMI] frame end"); }
  void panel(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t) override {
    Serial.printf("[HMI] panel %d,%d %dx%d\n", x, y, w, h);
  }
  void text(int16_t x, int16_t y, const String& value, uint16_t) override {
    Serial.printf("[HMI] text %d,%d: %s\n", x, y, value.c_str());
  }
};

constexpr uint16_t kStartButton = 100;
SerialRenderer renderer;
hmi::Screen home;
hmi::Label title(1, 18, 24, "ESP32 Generic HMI");
hmi::Label state(2, 18, 54, "Durum: Hazir", 0x07E0);
hmi::Button startButton(kStartButton, 18, 92, 190, 48, "Baslat");
hmi::App app(renderer, home);

void onEvent(const hmi::Event& event) {
  if (event.type == hmi::EventType::Tap && event.targetId == kStartButton) {
    state.setText("Durum: Calisiyor");
    app.invalidate();
  }
}

void setup() {
  Serial.begin(115200);
  home.add(title);
  home.add(state);
  home.add(startButton);
  app.begin(onEvent);
}

void loop() {
  app.loop();
  // A real touch/UART driver should call app.tap(x, y) on press/release policy.
  delay(10);
}
