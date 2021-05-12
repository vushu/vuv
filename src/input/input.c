#include "vuv/input.h"
#include "SDL_events.h"
#include "vuv/application_states.h"
#include "vuv/datatypes.h"
#include "vuv/input_keys.h"
#include <stdbool.h>

void vuv_input_listen(vuv_application *app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app->state = VUV_APPLICATION_STOPPED;
                break;
            case SDL_KEYDOWN:
                if (app->input->key_down)
                    app->input->key_down(app, event);
                break;
            case SDL_KEYUP:
                if (app->input->key_up)
                    app->input->key_up(app, event);
                break;
        }
    }
}

bool vuv_input_key_escape(vuv_application *app) {
    return app->context->event->key.keysym.sym == SDLK_ESCAPE;
}

bool vuv_input_key_up(vuv_application *app) {
    return app->context->event->type == SDL_KEYUP;
}

bool vuv_input_key_down(vuv_application *app) {
    return app->context->event->type == SDL_KEYDOWN;
}
