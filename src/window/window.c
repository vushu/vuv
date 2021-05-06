#include "vuv/window.h"
#include "SDL_video.h"

vuv_context* vuv_create_window(char* title, int width, int height) {
    vuv_context* vuv_ctx = malloc(sizeof(vuv_context));
    vuv_ctx->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    vuv_ctx->glContext = SDL_GL_CreateContext(vuv_ctx->window);
    return vuv_ctx;
}
