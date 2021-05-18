#include <stdio.h>

#include "vuv/vuv.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION

#include "vuv/nuklear.h"
#include "vuv/nuklear_sdl_gl3.h"


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


void test_nuklear(vuv_application *data) {
    vuv_application *app = (vuv_application *) data;
    struct nk_context *ctx = app->context->nk_context;

    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 200, 200),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                 NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        nk_menubar_begin(ctx);
        nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
        nk_layout_row_push(ctx, 45);
        if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT);
            nk_menu_end(ctx);
        }
        nk_layout_row_push(ctx, 45);
        if (nk_menu_begin_label(ctx, "EDIT", NK_TEXT_LEFT, nk_vec2(120, 200))) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_menu_item_label(ctx, "COPY", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "CUT", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "PASTE", NK_TEXT_LEFT);
            nk_menu_end(ctx);
        }
        nk_layout_row_end(ctx);
        nk_menubar_end(ctx);

        enum {
            EASY, HARD
        };
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button"))
            fprintf(stdout, "button pressed\n");
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);

}

void callback_game_loop(void *app) {
    test_nuklear((vuv_application *) app);
}

int main(int argc, char *args[]) {

    app = vuv_application_create("Vuv Game", 1920, 1080);

    if (vuv_application_init(app) == 1) {
        app->input->key_down = &callback_key_down;
        app->input->key_up = &callback_key_up;
        app->game_loop_callback = &callback_game_loop;
        app->context->nk_context = nk_sdl_init(app->context->sdl_window);
        {
            struct nk_font_atlas *atlas;
            nk_sdl_font_stash_begin(&atlas);
            nk_sdl_font_stash_end();
        }
        vuv_application_run(app);

        return 0;
    }
}
