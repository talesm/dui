#ifndef DUI_BUTTON_HPP
#define DUI_BUTTON_HPP

#include <string_view>
#include "Group.hpp"
#include "Panel.hpp"

namespace dui {

// Style for button
struct ButtonStyle
{
  SDL_Color text;
  EdgeSize padding;
  BorderedBoxStyle normal;
  BorderedBoxStyle grabbed;
  BorderedBoxStyle pressed;
  BorderedBoxStyle pressedGrabbed;
};

namespace style {

constexpr BorderedBoxStyle BUTTONBOX{
  {176, 195, 222, 255},
  {
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    {0, 0, 0, 255},
    {0, 0, 0, 255},
  },
};

constexpr BorderedBoxStyle BUTTONBOX_GRABBED{
  BUTTONBOX.withBackground({147, 173, 210, 255})};
constexpr BorderedBoxStyle BUTTONBOX_PRESSED{
  BUTTONBOX.withBorderColor(BUTTONBOX.borderColor.withInvertedBorders())};
constexpr BorderedBoxStyle BUTTONBOX_PRESSED_GRABBED{
  BUTTONBOX_PRESSED.withBackground(BUTTONBOX_GRABBED.background)};

constexpr ButtonStyle BUTTON{
  TEXT,
  EdgeSize::all(3),
  BUTTONBOX,
  BUTTONBOX_GRABBED,
  BUTTONBOX_PRESSED,
  BUTTONBOX_PRESSED_GRABBED,
};
}

/**
 * @brief Common button behavior
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param pushed if the button is pushed or not
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
buttonBase(Group& target,
           std::string_view id,
           std::string_view str,
           bool pushed,
           const SDL_Point& p = {0},
           const ButtonStyle& style = style::BUTTON)
{
  if (str.empty()) {
    str = id;
  }
  auto adv = elementSize(style.padding + EdgeSize::all(1), measure(str));
  SDL_Rect r{p.x, p.y, adv.x, adv.y};
  auto action = target.checkMouse(id, r);
  bool grabbing = action == MouseAction::HOLD;

  PanelStyle curStyle = style::PANEL;
  curStyle.padding = style.padding;
  if (grabbing == pushed) {
    curStyle.border = grabbing ? style.grabbed : style.normal;
  } else {
    curStyle.border = grabbing ? style.pressedGrabbed : style.pressed;
  }
  auto g = panel(target, id, r, Layout::NONE, curStyle);
  text(g, str, {0}, style.text);
  g.end();
  return action == MouseAction::ACTION;
}

/**
 * @{
 * @brief A push button
 *
 * It can be used to make an action every time it is pushed
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
button(Group& target,
       std::string_view id,
       std::string_view str,
       const SDL_Point& p = {0},
       const ButtonStyle& style = style::BUTTON)
{
  return buttonBase(target, id, str, false, p, style);
}
inline bool
button(Group& target,
       std::string_view id,
       const SDL_Point& p = {0},
       const ButtonStyle& style = style::BUTTON)
{
  return button(target, id, id, p, style);
}
/// @}

/**
 * @{
 * @brief A button that toggle a boolean variable
 *
 * Every time it is pushed the value is negated. The button reflects this state
 * by being pushed or pulled if the variable is true of false, respectively.
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to a boolean with the state
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
inline bool
toggleButton(Group& target,
             std::string_view id,
             std::string_view str,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  if (buttonBase(target, id, str, *value, p, style)) {
    *value = !*value;
    return true;
  }
  return false;
}
inline bool
toggleButton(Group& target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  return toggleButton(target, id, id, value, p, style);
}
/// @}

/**
 * @{
 * @brief A button part of multiple choice question
 *
 * If this button is actionned the value is changed to the given option
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to the control variable
 * @param option the option this button represents if value is equivalent to
 * this, then the button will appear pressed. If the user pushed, then the value
 * is set to this option.
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             std::string_view str,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  bool selected = *value == option;
  if (buttonBase(target, id, str, selected, p, style) && !selected) {
    *value = option;
    return true;
  }
  return false;
}
template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  return choiceButton(target, id, id, value, option, p, style);
}
/// @}

} // namespace dui

#endif // DUI_BUTTON_HPP
