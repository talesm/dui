#include <sstream>
#include <string>
#include <SDL.h>
// #include "DarkTheme.hpp" // enable this to check the dark theme
#include "dui.hpp"

int
main(int argc, char** argv)
{
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }

  // The ui state
  dui::State state{renderer};

  // Some test variables
  int clickCount = 0;
  std::string clickMeStr{"Click me!"};

  bool toggleOption = false;
  enum MultiOption
  {
    OPTION1,
    OPTION2,
    OPTION3,
  };
  MultiOption multiOption{};

  constexpr size_t str1Size = 100;
  char str1[str1Size] = "str1";
  std::string str2 = "str2";
  int value1 = 42;
  double value2 = 11.25;

  std::string basePath = SDL_GetBasePath();
  SDL_Surface* surface = SDL_LoadBMP((basePath + "../dui.bmp").c_str());
  SDL_SetColorKey(surface, 1, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  for (;;) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      state.event(ev);
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // UI
    auto f = dui::frame(state);

    // Free label
    dui::label(f, "Hello world", {320, 10});

    // Main panel
    auto p = dui::window(f, "Elements", {10, 10, 300, 580});
    // dui::label(f, "Error"); // You can not add anything to the frame until
    // you call p.end()

    // Labels inside the panel
    dui::label(p, "Hello world");
    dui::label(p,
               "Hello Styled World",
               {5},
               dui::themeFor<dui::Label>()
                 .withText({0xf0, 0x80, 0x80, 0xff})
                 .withScale(1));

    // Push button example. It returns true only when you click on it (press and
    // release)
    if (dui::button(p, "Click me!", clickMeStr)) {
      clickCount += 1;
      std::stringstream ss;
      ss << "Click count: " << clickCount;
      clickMeStr = ss.str();
    }

    // A button that presents the state of boolean, being pressed if true and
    // released if false, inverting its value if clicked.
    if (dui::toggleButton(p, "Toggle", &toggleOption)) {
      // Like button(), it also returns true on click, so you can do a special
      // action
      SDL_Log("Toggled options, new value is, %s",
              toggleOption ? "true" : "false");
    }
    dui::label(p, toggleOption ? "activated" : "not activated", {5});

    // Choice buttons: similar to toggle buttons, but for any types
    if (dui::choiceButton(p, "Option 1", &multiOption, OPTION1, {0, 5})) {
      // Like button(), it also returns true on click, so you can do a special
      // action.
      SDL_Log("Selected Option %d", 1 + multiOption);
      // Pay attention we only log when optin 1 was chose and not when 2 or 3.
    }
    dui::choiceButton(p, "Option 2", &multiOption, OPTION2);
    dui::choiceButton(p, "Option 3", &multiOption, OPTION3);

    // Using a panel to group elements. You can replace panel() by group() if
    // you don't want borders/custom color
    if (auto g = dui::panel(p, "group1")) {
      dui::label(g, "Grouped Label");
      dui::button(g, "Grouped button");
    }

    // Example changing background color of the next panel
    auto panelStyle =
      dui::themeFor<dui::Panel>().withBackgroundColor({224, 255, 224, 255});
    // And also making it grow horizontally
    if (auto g =
          dui::panel(p, "group2", {0}, dui::Layout::HORIZONTAL, panelStyle)) {
      dui::label(g, "Grouped Label");
      dui::button(g, "Grouped button");
    }
    static SDL_Point scrollOffset{0};
    if (auto g = dui::scrollablePanel(p, "group3", &scrollOffset)) {
      dui::label(g, "Grouped Label1");
      dui::button(g, "Grouped button1");
      dui::label(g, "Grouped Label2");
      dui::button(g, "Grouped button2");
    }

    // Text input examples
    dui::label(p, "Text input", {0, 10});
    dui::textField(p, "Str1", str1, str1Size);
    dui::textField(p, "Str2", &str2);

    // numeric input examples
    dui::label(p, "Number input", {0, 10});
    dui::numberField(p, "value1", &value1);
    dui::numberField(p, "value2", &value2);
    dui::sliderField(p, "value1 b", &value1, 0, 100);

    // New panel for images
    p.end();
    p = dui::window(f, "Textures", {480, 10});
    // images
    dui::textureBox(p, texture, {0, 0, 8, 8});
    dui::textureBox(p, texture, {0, 1, 64, 64});
    dui::textureBox(p, texture, {0, 1, 128, 128});

    // Here we explicitly end the panel p, so we can add elements to the frame
    // directly again after that.
    p.end();

    static SDL_Point scrollOffset2{0};
    if (auto w = dui::scrollableWindow(
          f, "Scroll Window", &scrollOffset2, {320, 30, 150, 0})) {
      for (int i = 0; i < 10; ++i) {
        dui::label(w, "Some label");
      }
      dui::button(w, "button");
    }

    // For example, we can add this big texture
    dui::textureBox(f, texture, {400, 300, 256, 256});

    // Render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
