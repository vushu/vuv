#include <SDL2/SDL.h>

typedef struct vu_ctx {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext* glContext;

}vu_context;

typedef struct vu_nk_ctx  {

}vu_nk_context;


vu_context* vu_init_app(vu_context* ctx, int width, int height);

void vu_destroy(vu_context* ctx);

void vu_quit();
