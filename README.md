# NetController

This repository contains a client and UE4 plugin that allows a controller connected to the client device to control a UE4 game running on another device. This was originally created to allow use of a 360 controller on an iPhone for use with a Google Cardboard. However, this can be used with any controller with an SDL2 Game Controller mapping and any client device that can run a UE4 game.

##How to use

The client requires SDL2 and Boost's ASIO library. Additionally, the compilation script requires CMake and pkg-config to run. You can optionally use the standalone version of ASIO using `-DASIO_STANDALONE=YES` and use Boost's thread library instead of the standard library's using `-DUSE_BOOST_THREAD=YES` when running the `cmake` command. Compile using `cmake . && make` and run with `./NetController` for usage information. You should also download the `gamecontrollerdb.txt` file from the [SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB) repository and place it in the same folder as the binary file.

The easiest way to install the plugin is to add it to a "Plugins" folder along side your .uproject file. If the project is setup for C++ compilation already, then the plugin should compile the next time you open the project. If the project is setup to be blueprint-only, you may need to first use it with a C++ enabled project. With the C++ project, build for any platforms you plan on using it with, and then copy the plugin folder to the blueprint project.
