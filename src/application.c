#include "vuv/application.h"
#include "vuv/application_states.h"
#include "vuv/input.h"
#include "vuv/log.h"
#include "vuv/render.h"
#include "vuv/window.h"
#include <stdlib.h>


#define VUV_FRAME_PER_SEC 1000 / 60

int vuv_application_init(vuv_application *app) {
    int result;
    if ((result = vuv_render_setup_sdl())) {
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

void game_loop(vuv_application *app) {

    uint64_t start = SDL_GetPerformanceCounter();
    vuv_input_listen(app);

    vuv_render_clear(app->render);

    app->game_loop_callback(app);


    Uint64 end = SDL_GetPerformanceCounter();

    float dt = (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;

    /*vuv_render_nk_draw();*/
//    printf("sdfasd%f", VUV_FRAME_PER_SEC);
    SDL_Delay(floor(16.666f - dt));
    SDL_GL_SwapWindow(app->context->sdl_window);
}

void vuv_application_run(vuv_application *app) {
    app->state = VUV_APPLICATION_RUNNING;
    if (!app->game_loop_callback) {
        /*SDL_LOG("hello");*/
        /*SDL_LOG_ERR()*/
        /*SDL_LogMessage(SDL_LOG_CATEGORY_ERROR,1,"No gameloop callback");*/
        /*vuv_log(int line, const char *filename, SDL_LogPriority priority, ...)*/
        app->state = VUV_APPLICATION_STOPPED;
    }

    while (app->state) {
        game_loop(app);
    }


    vuv_application_destroy(app);
}

