//
//  main.c
//  NetController
//
//  Created by Lewis Fox on 10/1/15.
//  Copyright © 2015 LRFLEW. All rights reserved.
//

#include "sdlhelper.hpp"
#include "controller.hpp"
#include <iostream>
#include <cstdlib>

#ifdef USE_BOOST_THREAD
#include <boost/thread.hpp>
#define SLEEP boost
#else
#include <thread>
#define SLEEP std
#endif

#ifdef ASIO_STANDALONE
#include <asio.hpp>
#define ASIO asio
#define ERROR asio
#else
#include <boost/asio.hpp>
#define ASIO boost::asio
#define ERROR boost::system
#endif

int main(int argc, const char * argv[]) {
    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS))
        logAndCrashSDL("SDL_Init");
    
    if (SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt") < 0)
        std::cout << "Error loading gamecontrollerdb.txt: " << SDL_GetError() << std::endl;
    
    int controllercount = SDL_NumJoysticks();
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <Client IP> <Joystick Number>" << std::endl << std::endl;
        std::cout << "Avaiable Controllers:" << std::endl;
        if (controllercount == 0)
            std::cout << "No Controllers Found" << std::endl;
        else for (int i = 0; i < controllercount; ++i) {
            const char *jname, *gname;
            
            jname = SDL_JoystickNameForIndex(i);
            if (SDL_IsGameController(i))
                gname = SDL_GameControllerNameForIndex(i);
            else
                gname = "N/A";
            std::cout << i << ": " << jname << " (" << gname << ")" << std::endl;
        }
        return argc > 1;
    }
    
    for (const char *c = argv[2]; *c != '\0'; ++c) {
        if (*c < '0' || *c > '9') {
            std::cout << "Error: Invalid number " << argv[2] << std::endl;
            return 1;
        }
    }
    
    int controllerid = std::atoi(argv[2]);
    if (controllerid >= controllercount) {
        std::cerr << "Error: Unknown Joystick " << argv[2] << std::endl;
        exit(1);
    } else if (!SDL_IsGameController(controllerid)) {
        std::cerr << "Error: Joystick " << argv[2] << " does not have a mapping" << std::endl;
    }
    
    std::cout << "Using Controller " << controllerid << ": " << SDL_JoystickNameForIndex(controllerid);
    std::cout << " (" << SDL_GameControllerNameForIndex(controllerid) << ")" << std::endl;
    
    ASIO::io_service io_service;
    ASIO::ip::udp::resolver resolver(io_service);
    ASIO::ip::udp::resolver::query query(ASIO::ip::udp::v4(), argv[1], "1238");
    ASIO::ip::udp::endpoint receiver_endpoint;
    try {
        receiver_endpoint = *resolver.resolve(query);
    } catch (ERROR::system_error &e) {
        std::cerr << "Unable to Resolve IP: " << e.what() << std::endl;
        SDL_Quit();
        std::exit(-1);
    }
    ASIO::ip::udp::socket socket(io_service);
    socket.connect(receiver_endpoint);
    
    {
        Controller c(controllerid);
        ASIO::const_buffers_1 cbuf = ASIO::buffer(c.cdata(), CONTROLLER_SIZE);
        
        std::cout << "Starting Controller Stream" << std::endl;
        
        bool run = true;
        SLEEP::chrono::time_point<SLEEP::chrono::steady_clock> time = SLEEP::chrono::steady_clock::now();
        while (run) {
            SDL_Event event;
            while (run && SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        run = false;
                        break;
                    default:
                        c.handleEvent(event);
                        break;
                }
            }
            
            if (run) try {
                socket.send(cbuf);
                SLEEP::this_thread::sleep_until(time += SLEEP::chrono::milliseconds(20));
            } catch (ERROR::system_error &e) {
                std::cerr << "Unable to Send Message: " << e.what() << std::endl;
                SDL_Quit();
                exit(-1);
            }
        }
    }
    
    SDL_Quit();
    return 0;
}
