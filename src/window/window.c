#include "vuv/window.h"
#include "vuv/errors.h"
#include "vuv/render.h"

int vuv_window_create(char* title, int width, int height, vuv_application * app)
{
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        return VUV_WINDOW_FAILED;
    }

    app->context = malloc(sizeof(vuv_context));
    app->context->sdl_window = window;
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    if (gl_context == NULL) {
        app->context->gl_context = gl_context;
        return VUV_OPENGL_FAILED;
    }

    return vuv_render_init(app->render);
}
