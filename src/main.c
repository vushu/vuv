#include <SDL2/SDL.h>
#include <stdio.h>
#include "SDL_timer.h"
#include "SDL_video.h"
#include "vuv/application.h"
#include "vuv/errors.h"
#include "vuv/vuv.h"

int main(int argc, char* args[]) {

    vuv_application* app = vuv_application_init("Vuv Game", 600, 300);

    if (app != NULL){
        SDL_Delay(2000);
        vuv_destroy_application(app);
    }
    else
        printf("WHAT");


    return 0;


}
