#include <SDL2/SDL.h>
#include "./game/window.h"
#include "SDL_timer.h"

int main(int argc, char* args[]) {

    vu_context ctx;
    vu_init_app(&ctx,600, 300);
    SDL_Delay(3000);
    vu_destroy(&ctx);
    return 0;


}
