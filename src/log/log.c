#include <stdio.h>
#include <vuv/log.h>

void SDLLog(int line, const char* filename, SDL_LogPriority priority, ...)
{
    va_list args;
    va_start(args, line);

    // consumes filename
    va_arg(args, const char*);
    if (line != -1) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, "%s - line %d", filename, line);
    }

    // consumes priority parameter
    va_arg(args, SDL_LogPriority);

    const char *error = SDL_GetError();
    if (*error != '\0') {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, "%s", error);
        SDL_ClearError();
    }

    const char* format = va_arg(args, const char*);
    if (format) {
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, priority, format, args);
        va_end(args);
    }
}
