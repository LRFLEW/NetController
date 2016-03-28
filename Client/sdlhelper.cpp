//
//  sdlhelper.c
//  Noise
//
//  Created by Lewis Fox on 4/1/15.
//  Copyright (c) 2015 LRFLEW. All rights reserved.
//

#include "sdlhelper.hpp"
#include <iostream>
#include <cstdlib>

void SDL_LogApp(SDL_LogPriority priority, const char* fmt, ...) {
    va_list ap;
    
    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, priority, fmt, ap);
    va_end(ap);
}

void logAndCrashSDL(std::string error) {
    std::string msg = SDL_GetError();
    std::cerr << error << ": " << msg << std::endl;
    SDL_Quit();
    std::exit(-1);
}
