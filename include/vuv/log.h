#pragma once
#include <SDL2/SDL.h>

#define __FILE_NAME__ __FILE__
#define SDL_LOG_LOCATION(...)     SDLLog(__LINE__, __FILE_NAME__,  SDL_LOG_PRIORITY_INFO, ##__VA_ARGS__)
#define SDL_LOG(...)              SDLLog(      -1,       NULL,  SDL_LOG_PRIORITY_INFO, ##__VA_ARGS__)
#define SDL_LOG_ERR_LOCATION(...) SDLLog(__LINE__, __FILE_NAME__, SDL_LOG_PRIORITY_ERROR, ##__VA_ARGS__)
#define SDL_LOG_ERR(...)          SDLLog(     -1 ,       NULL, SDL_LOG_PRIORITY_ERROR, ##__VA_ARGS__)


void SDLLog(int line, const char* filename, SDL_LogPriority priority, ...);
