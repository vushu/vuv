#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"


vu_context* vu_init_app(vu_context* ctx, int width, int height) {

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        ctx = malloc(sizeof(vu_context));
        ctx->window = SDL_CreateWindow("VuGame",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
        ctx->glContext = SDL_GL_CreateContext(ctx->window);
    }
    return ctx;
}

/*void vu_draw(vu_context* ctx) {*/

/*}*/
void vu_destroy(vu_context* ctx) {
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}

void vu_quit() {
    SDL_Quit();
}
