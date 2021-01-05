#ifndef DUI_GROUP_HPP_
#define DUI_GROUP_HPP_

#include <string_view>
#include <SDL_rect.h>
#include "State.hpp"
#include "Target.hpp"

namespace dui {

constexpr SDL_Point
makeCaret(const SDL_Point& caret, int x, int y)
{
  return {caret.x + x, caret.y + y};
}

/**
 * @brief A grouping of widgets
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed elements
 *
 */
class Group
{
  Target parent;
  std::string_view id;
  bool locked = false;
  bool ended = false;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  Layout layout;

public:
  /**
   * @brief Construct a new branch Group object
   *
   * You probably want to use group() instead of this.
   *
   * @param parent the parent group. MUST NOT BE NULL
   * @param id the group id
   * @param rect the rect. Either w or h being 0 means it will auto size
   * @param layout the layout
   */
  Group(Target parent,
        std::string_view id,
        const SDL_Point& scroll,
        const SDL_Rect& rect,
        Layout layout);

  ~Group()
  {
    if (!ended) {
      end();
    }
  }
  Group(const Group&) = delete;
  Group(Group&& rhs);
  Group& operator=(Group&& rhs) = default;
  Group& operator=(const Group& rhs) = delete;

  operator bool() const { return !ended; }

  int width() const { return makeWidth(rect, topLeft, bottomRight, layout); }

  void width(int v) { rect.w = v; }

  int height() const { return makeHeight(rect, topLeft, bottomRight, layout); }

  void height(int v) { rect.h = v; }

  operator Target() &
  {
    return {&parent.getState(), id, rect, topLeft, bottomRight, layout, locked};
  }

  void end();
};

/**
 * @brief Create group
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param rect the group dimensions
 * @return Group
 */
inline Group
group(Target target,
      std::string_view id,
      const SDL_Rect& rect = {0},
      Layout layout = Layout::VERTICAL)
{
  return {target, id, {0, 0}, rect, layout};
}

/**
 * @brief Create group
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param rect the group dimensions
 * @return Group
 */
inline Group
offsetGroup(Target target,
            std::string_view id,
            const SDL_Point& offset,
            const SDL_Rect& r,
            Layout layout = Layout::VERTICAL)
{
  return {target, id, offset, r, layout};
}

inline Group::Group(Target parent,
                    std::string_view id,
                    const SDL_Point& scroll,
                    const SDL_Rect& rect,
                    Layout layout)
  : parent(parent)
  , id(id)
  , rect(rect)
  , topLeft(makeCaret(parent.getCaret(), rect.x - scroll.x, rect.y - scroll.y))
  , bottomRight(topLeft)
  , layout(layout)
{
  parent.lock(id, rect);
}

inline void
Group::end()
{
  SDL_assert(!ended);
  if (rect.w == 0) {
    rect.w = width();
  }
  if (rect.h == 0) {
    rect.h = height();
  }
  parent.unlock(id, rect);
  parent.advance({rect.x + rect.w, rect.y + rect.h});
  ended = true;
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , locked(rhs.locked)
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
  , layout(rhs.layout)
{
  rhs.ended = true;
}

} // namespace dui

#endif // DUI_GROUP_HPP_