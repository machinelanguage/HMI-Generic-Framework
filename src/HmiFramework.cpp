#include "HmiFramework.h"

namespace hmi {

Widget::Widget(uint16_t id, int16_t x, int16_t y, int16_t w, int16_t h)
    : id_(id), x_(x), y_(y), w_(w), h_(h) {}

bool Widget::hit(int16_t x, int16_t y) const {
  return x >= x_ && x < x_ + w_ && y >= y_ && y < y_ + h_;
}

bool Widget::onTap(int16_t, int16_t, Event&) { return false; }

Label::Label(uint16_t id, int16_t x, int16_t y, const String& text, uint16_t color)
    : Widget(id, x, y, 0, 0), text_(text), color_(color) {}

void Label::setText(const String& text) { text_ = text; }

void Label::draw(Renderer& renderer) { renderer.text(x_, y_, text_, color_); }

Button::Button(uint16_t id, int16_t x, int16_t y, int16_t w, int16_t h, const String& text)
    : Widget(id, x, y, w, h), text_(text) {}

void Button::draw(Renderer& renderer) {
  renderer.panel(x_, y_, w_, h_, 0x39E7);
  renderer.text(x_ + 10, y_ + h_ / 2, text_, 0xFFFF);
}

bool Button::onTap(int16_t, int16_t, Event& event) {
  event = {EventType::Tap, id_, 0};
  return true;
}

bool Screen::add(Widget& widget) {
  if (count_ >= kMaxWidgets) return false;
  widgets_[count_++] = &widget;
  return true;
}

void Screen::draw(Renderer& renderer) {
  renderer.beginFrame();
  for (uint8_t i = 0; i < count_; ++i) widgets_[i]->draw(renderer);
  renderer.endFrame();
}

bool Screen::dispatchTap(int16_t x, int16_t y, Event& event) {
  for (int8_t i = count_ - 1; i >= 0; --i) {
    if (widgets_[i]->hit(x, y) && widgets_[i]->onTap(x, y, event)) return true;
  }
  return false;
}

App::App(Renderer& renderer, Screen& initialScreen) : renderer_(renderer), screen_(&initialScreen) {}
void App::begin(EventHandler handler) { handler_ = handler; invalidate(); }
void App::setScreen(Screen& screen) { screen_ = &screen; invalidate(); }
void App::invalidate() { dirty_ = true; }

void App::tap(int16_t x, int16_t y) {
  Event event{};
  if (screen_->dispatchTap(x, y, event) && handler_) handler_(event);
}

void App::loop() {
  if (dirty_) {
    screen_->draw(renderer_);
    dirty_ = false;
  }
}

}  // namespace hmi
