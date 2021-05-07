#include "vuv/application.h"
#include "vuv/errors.h"
#include "SDL.h"
#include "SDL_video.h"
#include "vuv/render.h"
#include "vuv/window.h"
#include <stdlib.h>


vuv_application* vuv_application_init(char* title, int width, int height) {
    if (vuv_setup_sdl() == VUV_OK) {
        vuv_application* app = malloc(sizeof(vuv_application));
        if (vuv_window_create(title, width, height, app->context) == VUV_OK) {
            return app;
        }
    }

    return NULL;
}

void vuv_destroy_application(vuv_application* vuv_app) {

    if (vuv_app != NULL && vuv_app->context != NULL){
        SDL_GL_DeleteContext(vuv_app->context->gl_context);
        SDL_DestroyWindow(vuv_app->context->sdl_window);
        free(vuv_app->context);
        free(vuv_app);
        SDL_Quit();
    }
}
