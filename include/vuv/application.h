#pragma once
#include <SDL2/SDL.h>

typedef struct vuv_ctx {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext* glContext;

}vuv_context;

typedef struct vuv_app {
    vuv_context* vuv_context;
}vuv_application;

int vuv_init_application(char* title, int width, int height, vuv_application* vu_app);

void vuv_destroy_application(vuv_application* vu_app);


