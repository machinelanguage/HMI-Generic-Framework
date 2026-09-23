#pragma once

#include <Arduino.h>

namespace hmi {

enum class EventType : uint8_t { Tap, ValueChanged, Tick };

struct Event {
  EventType type;
  uint16_t targetId;
  int32_t value;
};

class Renderer {
 public:
  virtual ~Renderer() = default;
  virtual void beginFrame() = 0;
  virtual void endFrame() = 0;
  virtual void panel(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
  virtual void text(int16_t x, int16_t y, const String& value, uint16_t color) = 0;
};

class Widget {
 public:
  Widget(uint16_t id, int16_t x, int16_t y, int16_t w, int16_t h);
  virtual ~Widget() = default;
  uint16_t id() const { return id_; }
  bool hit(int16_t x, int16_t y) const;
  virtual void draw(Renderer& renderer) = 0;
  virtual bool onTap(int16_t x, int16_t y, Event& event);

 protected:
  uint16_t id_;
  int16_t x_, y_, w_, h_;
};

class Label final : public Widget {
 public:
  Label(uint16_t id, int16_t x, int16_t y, const String& text, uint16_t color = 0xFFFF);
  void setText(const String& text);
  void draw(Renderer& renderer) override;

 private:
  String text_;
  uint16_t color_;
};

class Button final : public Widget {
 public:
  Button(uint16_t id, int16_t x, int16_t y, int16_t w, int16_t h, const String& text);
  void draw(Renderer& renderer) override;
  bool onTap(int16_t x, int16_t y, Event& event) override;

 private:
  String text_;
};

class Screen {
 public:
  static constexpr uint8_t kMaxWidgets = 12;
  bool add(Widget& widget);
  void draw(Renderer& renderer);
  bool dispatchTap(int16_t x, int16_t y, Event& event);

 private:
  Widget* widgets_[kMaxWidgets]{};
  uint8_t count_ = 0;
};

class App {
 public:
  typedef void (*EventHandler)(const Event& event);
  App(Renderer& renderer, Screen& initialScreen);
  void begin(EventHandler handler);
  void setScreen(Screen& screen);
  void invalidate();
  void tap(int16_t x, int16_t y);
  void loop();

 private:
  Renderer& renderer_;
  Screen* screen_;
  EventHandler handler_ = nullptr;
  bool dirty_ = true;
};

}  // namespace hmi
