#ifndef DUI_FONT_HPP
#define DUI_FONT_HPP

#include <SDL.h>

namespace dui {

struct Font
{
  SDL_Texture* texture;
  int charW, charH;
  int cols;
};

#include "defaultFont.h"

inline Font
loadDefaultFont(SDL_Renderer* renderer)
{
  SDL_RWops* src = SDL_RWFromConstMem(font_bmp, font_bmp_len);
  SDL_Surface* surface = SDL_LoadBMP_RW(src, 1);
  SDL_SetColorKey(surface, 1, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return {texture, 8, 8, 16};
}

} // namespace dui

#endif
