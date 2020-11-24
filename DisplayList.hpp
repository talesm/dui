#ifndef DUI_DISPLAY_LIST_HPP
#define DUI_DISPLAY_LIST_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>

// TODO include this conditionally
#include <SDL2_gfxPrimitives.h>

namespace dui {

/**
 * @brief Contains the list of elements to render
 *
 */
class DisplayList
{
private:
  struct Item
  {
    SDL_Rect rect;
    SDL_Color color;
    char content;
  };

  std::vector<Item> items;

public:
  void clear() { items.clear(); }

  size_t size() { return items.size(); }

  void insert(const SDL_Rect& rect, SDL_Color color, char ch)
  {
    if (color.a > 0) {
      items.push_back({rect, color, ch});
    }
  }

  void clip(const SDL_Rect& rect, size_t pos);

  void render(SDL_Renderer* renderer) const;
};

inline void
DisplayList::clip(const SDL_Rect& rect, size_t pos)
{
  if (pos >= items.size()) {
    return;
  }
  auto it = std::remove_if(items.begin() + pos, items.end(), [&](auto& item) {
    return !SDL_IntersectRect(&rect, &item.rect, nullptr);
  });
  items.erase(it, items.end());
}

inline void
DisplayList::render(SDL_Renderer* renderer) const
{
  for (auto it = items.rbegin(); it != items.rend(); it++) {
    auto c = it->color;
    if (it->content == 0) {
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
      SDL_RenderFillRect(renderer, &it->rect);
    } else {
      characterRGBA(
        renderer, it->rect.x, it->rect.y, it->content, c.r, c.g, c.b, c.a);
    }
  }
}

} // namespace dui

#endif // DUI_DISPLAY_LIST_HPP
