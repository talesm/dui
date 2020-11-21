#ifndef DUI_BASE_WIDGETS_HPP_
#define DUI_BASE_WIDGETS_HPP_

#include <string_view>
#include "Frame.hpp"

namespace dui {

inline void
renderLabel(Frame& frame,
            std::string_view text,
            const SDL_Point& p,
            SDL_Color color)
{
  frame.string(p, color, text);
}

inline void
label(Frame& frame,
      std::string_view text,
      const SDL_Point& p,
      SDL_Color color = {0, 0, 0, 255})
{
  auto adv = frame.measure(text);
  adv.x += p.x + 2;
  adv.y += p.y + 2;
  renderLabel(frame, text, {p.x + 1, p.y + 1}, color);
  frame.advance(adv);
}

inline void
renderButton(Frame& frame, const SDL_Rect& r)
{
  frame.box({r.x + 1, r.y, r.w - 2, 1}, {255, 255, 255, 255});
  frame.box({r.x, r.y + 1, 1, r.h - 2}, {255, 255, 255, 255});
  frame.box({r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {0, 0, 0, 224});
  frame.box({r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {0, 0, 0, 224});
  frame.box({r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {224, 224, 224, 255});
}

inline void
renderButtonPressed(Frame& frame, const SDL_Rect& r)
{
  frame.box({r.x + 1, r.y, r.w - 2, 1}, {0, 0, 0, 224});
  frame.box({r.x, r.y + 1, 1, r.h - 2}, {0, 0, 0, 224});
  frame.box({r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {255, 255, 255, 255});
  frame.box({r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {255, 255, 255, 255});
  frame.box({r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {224, 224, 224, 255});
}

inline bool
button(Frame& frame, std::string_view text, bool inverted, const SDL_Point& p)
{
  auto adv = frame.measure(text);
  SDL_Rect r{p.x, p.y, adv.x + 2, adv.y + 2};
  auto action = frame.testMouse(text, r);
  if ((action == MouseAction::GRAB) != inverted) {
    renderButtonPressed(frame, r);
  } else {
    renderButton(frame, r);
  }
  renderLabel(frame, text, {p.x + 1, p.y + 1}, {0, 0, 0, 255});
  adv.x += p.x + 2;
  adv.y += p.y + 2;
  frame.advance(adv);
  return action == MouseAction::ACTION;
}

inline bool
button(Frame& frame, std::string_view text, const SDL_Point& p)
{
  return button(frame, text, false, p);
}

inline bool
toggleButton(Frame& frame,
             std::string_view text,
             bool* value,
             const SDL_Point& p)
{
  if (button(frame, text, *value, p)) {
    *value = !*value;
    return true;
  }
  return false;
}

template<class T, class U>
inline bool
choiceButton(Frame& frame,
             std::string_view text,
             T* value,
             U option,
             const SDL_Point& p)
{
  bool selected = *value == option;
  if (button(frame, text, selected, p) && !selected) {
    *value = option;
    return true;
  }
  return false;
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
