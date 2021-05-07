#include "vuv/window.h"
#include "SDL_video.h"
#include "vuv/application.h"
#include "vuv/errors.h"
#include <stdio.h>
#include <glad/glad.h>

int vuv_window_create(char* title, int width, int height, vuv_context* vuv_ctx) {
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        return VUV_FAIL;
    }

    vuv_ctx = malloc(sizeof(struct vuv_ctx));
    vuv_ctx->sdl_window = window;
    vuv_ctx->gl_context = SDL_GL_CreateContext(vuv_ctx->sdl_window);

    /*if (vuv_ctx->gl_context) {*/
    /*return VUV_FAIL;*/
    /*}*/
    if (gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress) != 0) {
        printf("FAILED to LOAD GLAD");
        return VUV_FAIL;
    }
    printf("glad loaded!\n");
    return VUV_OK;
}
