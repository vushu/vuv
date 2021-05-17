#include "vuv/application.h"
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "vuv/application_states.h"
#include "vuv/input.h"
#include "vuv/render.h"
#include "vuv/window.h"
#include <stdlib.h>
//#include "vuv/nuklear_sdl_gles2.h"
#include "vuv/nuklear.h"


#define VUV_FRAME_PER_SEC 1000 / 60

int vuv_application_init(vuv_application* app)
{
    int result;
    if ((result = vuv_render_setup_sdl())) {
        app->state = VUV_APPLICATION_RUNNING;
        result = vuv_window_create(app->window->title, app->window->width, app->window->height, app);
    } else
        free(app);

    return result;
}

vuv_application* vuv_application_create(char* title, int width, int height)
{
    vuv_application* app = malloc(sizeof(vuv_application));
    app->window = malloc(sizeof(vuv_window_data));
    app->render = malloc(sizeof(vuv_render_data));
    app->input = malloc(sizeof(vuv_input));
    app->window->title = title;
    app->window->width = width;
    app->window->height = height;
    return app;
}

void vuv_application_destroy(vuv_application* vuv_app)
{
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

void game_loop(vuv_application* app)
{
    SDL_Delay(VUV_FRAME_PER_SEC);
    vuv_input_listen(app);

    vec2 position = { 10.0f, 45.0f };
    vec2 size = { 200, 200 };

    vuv_render_clear(app->render);

    app->game_loop_callback(app);

    vec4 color = { 1, 1, 0, 1 };

    for (int i = 0; i < 1000; i++) {
        vuv_render_draw_quad(app->render, position, size, color);
    }

    vuv_render_end_begin_flush_batch(app->render);

    vuv_render_nk_draw();
    SDL_GL_SwapWindow(app->context->sdl_window);
}

void vuv_application_run(vuv_application* app)
{
    if (!app->game_loop_callback) {
        SDL_Log("No gameloop callback");
        app->state = VUV_APPLICATION_STOPPED;
    }

    while (app->state) {
        game_loop(app);
    }

    vuv_application_destroy(app);
}

