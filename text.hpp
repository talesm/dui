#ifndef DUI_TEXT_HPP_
#define DUI_TEXT_HPP_

#include <SDL.h>
#include "Font.hpp"
#include "Group.hpp"
#include "theme.hpp"

namespace dui {

// Text style
struct TextStyle
{
  Font font;
  SDL_Color color;
  int scale; // 0: 1x, 1: 2x, 2: 4x, 3: 8x, and so on

  constexpr TextStyle withFont(const Font& font) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withColor(SDL_Color color) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withScale(int scale) const
  {
    return {font, color, scale};
  }
};

struct Text;

namespace style {

/// Default text style
template<>
struct FromTheme<Text, SteelBlue>
{
  constexpr static TextStyle get()
  {
    return {{nullptr, 8, 8, 16}, {45, 72, 106, 255}, 0};
  }
};
}

/// Measure the given character
constexpr SDL_Point
measure(char ch, const Font& font, int scale)
{
  return {font.charW << scale, font.charH << scale};
}

/// Measure the given text
constexpr SDL_Point
measure(std::string_view text, const Font& font, int scale)
{
  return {int((font.charW << scale) * text.size()), font.charH << scale};
}

/**
 * @brief Adds a character element
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
character(Group& target,
          char ch,
          const SDL_Point& p,
          const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto& font = state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w, p.y + dstRect.h});
  SDL_Rect srcRect{(ch % font.cols) * font.charW,
                   (ch / font.cols) * font.charH,
                   font.charW,
                   font.charH};
  state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
}

/**
 * @brief Adds a text element
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
text(Group& target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto font = style.font.texture ? style.font : state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w * int(str.size()), p.y + dstRect.h});
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % font.cols) * font.charW,
                     (ch / font.cols) * font.charH,
                     font.charW,
                     font.charH};
    state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
    dstRect.x += dstRect.w;
  }
}
} // namespace dui

#endif // DUI_TEXT_HPP_
