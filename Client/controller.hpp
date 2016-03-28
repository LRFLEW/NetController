//
//  Controller.hpp
//  NetController
//
//  Created by Lewis Fox on 10/1/15.
//  Copyright © 2015 LRFLEW. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include "sdlhelper.hpp"
#include <iostream>

#define CONTROLLER_SIZE ((SDL_CONTROLLER_AXIS_MAX * 2) + SDL_CONTROLLER_BUTTON_MAX + 1) /* 28 */

class Controller {
public:
    Controller(Controller const &c) = delete;
    void operator=(Controller const &c) = delete;
    Controller(int joystick);
    ~Controller();
    void handleEvent(SDL_Event &event);
    void *data();
    const void *cdata();
    friend std::ostream &operator<<(std::ostream &os, Controller &c);
    
private:
    uint8_t rdata[CONTROLLER_SIZE];
    uint8_t *const buttons = (uint8_t*) &rdata[1];
    int16_t *const axes = (int16_t*) &rdata[SDL_CONTROLLER_BUTTON_MAX + 1 /* 16 */];
    SDL_GameController *sdlc;
    
    void setSDL(SDL_GameController *sdlc);
};

#endif /* Controller_hpp */
