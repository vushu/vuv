#pragma once
#include <SDL2/SDL.h>

typedef struct vuv_ctx {
    SDL_Window* sdl_window;
    //SDL_Renderer* renderer;
    SDL_GLContext gl_context;

} vuv_context;

typedef struct vuv_app {
    vuv_context* context;
} vuv_application;

vuv_application* vuv_application_init(char* title, int width, int height);

void vuv_destroy_application(vuv_application* vuv_app);


