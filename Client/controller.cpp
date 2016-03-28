//
//  Controller.cpp
//  NetController
//
//  Created by Lewis Fox on 10/1/15.
//  Copyright © 2015 LRFLEW. All rights reserved.
//

#include "Controller.hpp"
#include <iostream>

static_assert(SDL_CONTROLLER_BUTTON_MAX == 15, "Error: Unexpected number of Controller Buttons");
static_assert(SDL_CONTROLLER_AXIS_MAX == 6, "Error: Unexpected number of Controller Axes");
static_assert(SDL_BYTEORDER == SDL_LIL_ENDIAN, "Error: Incorrect Endianness");

Controller::Controller(int joystick) {
    sdlc = SDL_GameControllerOpen(joystick);
    if (sdlc == NULL)
        logAndCrashSDL("Unable to open controller");
    this->setSDL(sdlc);
    rdata[0] = 0;
}

Controller::~Controller() {
    SDL_GameControllerClose(sdlc);
}

void *Controller::data() {
    return rdata;
}

const void *Controller::cdata() {
    return rdata;
}

void Controller::setSDL(SDL_GameController *sdlc) {
    for (int i=0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        buttons[i] = SDL_GameControllerGetButton(sdlc, static_cast<SDL_GameControllerButton>(i));
    for (int i=0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
        axes[i] = SDL_GameControllerGetAxis(sdlc, static_cast<SDL_GameControllerAxis>(i));
}

void Controller::handleEvent(SDL_Event &event) {
    switch (event.type) {
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            buttons[event.cbutton.button] = event.cbutton.state;
            break;
        case SDL_CONTROLLERAXISMOTION:
            axes[event.caxis.axis] = event.caxis.value;
            break;
        default:
            break;
    }
}

std::ostream &operator<<(std::ostream &os, Controller &c) {
    os << "-- Axes --" << std::endl;
    for (int i=0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
        os << SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(i)) << ": " << +c.axes[i] << std::endl;
    os << "-- Buttons --";
    for (int i=0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        os << std::endl << SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(i)) << ": " << +c.buttons[i];
    return os;
}
