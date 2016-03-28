//
//  sdlhelper.h
//  Noise
//
//  Created by Lewis Fox on 4/1/15.
//  Copyright (c) 2015 LRFLEW. All rights reserved.
//

#ifndef __Noise__utilities__
#define __Noise__utilities__

#include <SDL.h>
#include <string>

void SDL_LogApp(SDL_LogPriority priority, const char* fmt, ...);
void logAndCrashSDL(std::string error);

#endif /* defined(__Noise__utilities__) */
