#include <SDL2/SDL.h>
/*#include "./game/window.h"*/
#include "SDL_timer.h"
#include "vuv/application.h"
#include "vuv/vuv.h"

int main(int argc, char* args[]) {

    vuv_application* vuv_app;
    if (vuv_init_application("Vuv Game", 600, 300, vuv_app)){
        SDL_Delay(3000);
        vuv_destroy_application(vuv_app);
    }

    return 0;


}
