#include "vuv/application.h"
#include "vuv/errors.h"
#include "SDL.h"
#include "SDL_video.h"
#include "vuv/render.h"
#include "vuv/window.h"
#include <stdlib.h>


int vuv_init_application(char* title, int width, int height, vuv_application* vuv_app) {

    if (vuv_setup_sdl() == EXIT_FAILURE) {
        return VUV_OK;
    }
    vuv_app = malloc(sizeof(vuv_application));
    vuv_app->vuv_context = vuv_create_window(title, width, height);
    return VUV_FAIL;
}

void vuv_destroy_application(vuv_application* vu_app) {
    SDL_GL_DeleteContext(vu_app->vuv_context->glContext);
    SDL_DestroyWindow(vu_app->vuv_context->window);
    SDL_Quit();
}
