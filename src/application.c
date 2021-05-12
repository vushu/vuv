#include "vuv/application.h"
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "vuv/application_states.h"
#include "vuv/errors.h"
#include "vuv/input.h"
#include "vuv/render.h"
#include "vuv/window.h"
#include <stdlib.h>


#define VUV_FRAME_PER_SEC 1000/60

int vuv_application_init(vuv_application *app) {
    int result;
    if ((result = vuv_render_setup_sdl())) {
        app->state = VUV_APPLICATION_RUNNING;
        result = vuv_window_create(app->window->title, app->window->width, app->window->height, app);
    } else
        free(app);

    return result;
}

vuv_application *vuv_application_create(char *title, int width, int height) {
    vuv_application *app = malloc(sizeof(vuv_application));
    app->window = malloc(sizeof(vuv_window_data));
    app->render = malloc(sizeof(vuv_render_data));
    app->input = malloc(sizeof(vuv_input));
    app->window->title = title;
    app->window->width = width;
    app->window->height = height;
    return app;
}

void vuv_application_destroy(vuv_application *vuv_app) {

    if (vuv_app != NULL && vuv_app->context != NULL) {

        SDL_GL_DeleteContext(vuv_app->context->gl_context);
        SDL_DestroyWindow(vuv_app->context->sdl_window);

        vuv_render_free(vuv_app->render);

        free(vuv_app->context);
        free(vuv_app->window);
        free(vuv_app->input);
        free(vuv_app);

        SDL_Quit();
    }
}

void vuv_application_game_loop(vuv_application *app) {
    SDL_Delay(VUV_FRAME_PER_SEC);
    vuv_input_listen(app);
    vuv_render_clear(app->render);
    int i;
    for (i = 0; i < 10; i++) {
        vuv_render_draw_quad(app->render);
    }
    vuv_render_end_batch(app->render);

    SDL_GL_SwapWindow(app->context->sdl_window);
}
