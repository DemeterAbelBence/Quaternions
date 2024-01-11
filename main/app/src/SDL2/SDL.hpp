#pragma once

#include <iostream>
#include <SDL.h>
#undef main

#include "SDL2_gfxPrimitives.h"

//namespace for SDL functionality
namespace SDL {

    struct Color {
        int red;
        int green;
        int blue;
        int alpha;

        Color(int r, int g, int b, int a) {
            red = r;
            green = g;
            blue = b;
            alpha = a;
        }
    };
    class Manager {

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        unsigned int window_width;
        unsigned int window_height;

    private:
        void setSDL(char const* title, int w, int h) {
            window_width = w;
            window_height = h;

            if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                SDL_Log("SDL cannot be started: % s", SDL_GetError());
                exit(1);
            }

            window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
            if (window == NULL) {
                SDL_Log("window cannot be created: %s", SDL_GetError());
                exit(1);
            }

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == NULL) {
                SDL_Log("renderer cannot be created: %s", SDL_GetError());
                exit(1);
            }

            SDL_RenderClear(renderer);
            SDL_Init(SDL_INIT_AUDIO);

        }

    public:
        Manager() {
            renderer = NULL;
            window = NULL;
        }
        Manager(const char* title, int width, int height) {
            setSDL(title, width, height);
        }
        inline int getWindowWidth() const { return window_width; }
        inline int getWindowHeight() const { return window_height; }

        inline SDL_Renderer* getRenderer() const { return renderer; }
        inline SDL_Window* getWindow() const { return window; }
        inline void renderFrame() const { SDL_RenderPresent(renderer); }
        inline int getTime() const { return SDL_GetTicks(); }
        inline void delay(int ms) const { SDL_Delay(ms); }
        inline void clearFrame() const { SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); SDL_RenderClear(renderer); }

        void drawCircle(int x, int y, int radius, Color color) const {
            filledCircleRGBA(renderer, x, y, radius,
                color.red, color.green, color.blue, color.alpha);
        }
        void drawLine(int x1, int y1, int x2, int y2, Color color) const {
            lineRGBA(renderer, x1, y1, x2, y2,
                color.red, color.green, color.blue, color.alpha);
        }

        ~Manager() {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    };

    enum userInput {
        KEYDOWN_UP,
        KEYDOWN_DOWN,
        KEYDOWN_LEFT,
        KEYDOWN_RIGHT,
        KEYDOWN_Q,
        KEYDOWN_W,
        KEYDOWN_E,
        KEYDOWN_R,
        KEYDOWN_ESC,
        KEYDOWN_SPACE,
        KEYDOWN_CTRL,
        KEYDOWN_A,
        KEYDOWN_B,

        KEYUP_UP,
        KEYUP_DOWN,
        KEYUP_LEFT,
        KEYUP_RIGHT,
        KEYUP_Q,
        KEYUP_W,
        KEYUP_E,
        KEYUP_R,
        KEYUP_ESC,
        KEYUP_SPACE,
        KEYUP_CTRL,
        KEYUP_A,
        KEYUP_B,

        QUIT,
        NOTHING
    };
    class Event {
    private:
        SDL_Event event;
        bool(*guiEventHandler)(const SDL_Event*) = nullptr;

    public:
        Event() {}
        void setGuiEventHandler(bool(*function)(const SDL_Event*)) { guiEventHandler = function; }
        SDL_Event getEvent() const { return event; }
        userInput getNewEvent() {
            while (SDL_PollEvent(&event)) {
                if(guiEventHandler != nullptr)
                    guiEventHandler(&event);
                switch (event.type) {
                    case SDL_KEYDOWN: {
                        switch (event.key.keysym.sym) {
                            case SDLK_UP: return KEYDOWN_UP;
                            case SDLK_DOWN: return KEYDOWN_DOWN;
                            case SDLK_LEFT: return KEYDOWN_LEFT;
                            case SDLK_RIGHT: return KEYDOWN_RIGHT;
                            case SDLK_q: return KEYDOWN_Q;
                            case SDLK_w: return KEYDOWN_W;
                            case SDLK_e: return KEYDOWN_E;
                            case SDLK_r: return KEYDOWN_R;
                            case SDLK_ESCAPE: return KEYDOWN_ESC;
                            case SDLK_SPACE: return KEYDOWN_SPACE;
                            case SDLK_LCTRL: return KEYDOWN_CTRL;
                            case SDLK_a: return KEYDOWN_A;
                            case SDLK_b: return KEYDOWN_B;

                        }
                        break;
                    }
                    case SDL_KEYUP:  {
                        switch (event.key.keysym.sym) {
                            case SDLK_UP: return KEYUP_UP;
                            case SDLK_DOWN: return KEYUP_DOWN;
                            case SDLK_LEFT: return KEYUP_LEFT;
                            case SDLK_RIGHT: return KEYUP_RIGHT;
                            case SDLK_q: return KEYUP_Q;
                            case SDLK_w: return KEYUP_W;
                            case SDLK_e: return KEYUP_E;
                            case SDLK_r:  return KEYUP_R;
                            case SDLK_ESCAPE: return KEYUP_ESC;
                            case SDLK_SPACE: return KEYUP_SPACE;
                            case SDLK_LCTRL: return KEYUP_CTRL;
                            case SDLK_a: return KEYUP_A;
                            case SDLK_b: return KEYUP_B;

                        }
                        break;
                    }
                    case SDL_QUIT: return QUIT;
                }
            }
            return NOTHING;
        }
    };
}