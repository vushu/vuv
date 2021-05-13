#include "SDL_timer.h"
#include "SDL_video.h"
#include "vuv/application.h"
#include "vuv/application_states.h"
#include "vuv/errors.h"
#include "vuv/input.h"
#include "vuv/vuv.h"
#include <SDL2/SDL.h>
#include <stdio.h>

vuv_application *app;

void callback_key_down(void *data, SDL_Event event) {
    vuv_application *app = (vuv_application *) data;
    if (event.key.keysym.sym == SDLK_ESCAPE) {
//        app->state = VUV_APPLICATION_STOPPED;
    }
}

void callback_key_up(void *data, SDL_Event event) {
    vuv_application *app = (vuv_application *) data;
    if (event.key.keysym.sym == SDLK_ESCAPE) {
        app->state = VUV_APPLICATION_STOPPED;
    }
}

int main(int argc, char *args[]) {

    app = vuv_application_create("Vuv Game", 1920, 1080);

    if (vuv_application_init(app) == 1) {
        app->input->key_down = &callback_key_down;
        app->input->key_up = &callback_key_up;

        while (app->state) {
            vuv_application_game_loop(app);
        }

    }
    vuv_application_destroy(app);

    return 0;
}
