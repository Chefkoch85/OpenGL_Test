#include "Errors.h"

#include <cstdlib>

#include <conio.h>

#include <iostream>
#include <SDL\SDL.h>

//Prints out an error message and exits the game
void fatalError(std::string errorString) {
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit...";

    SDL_Quit();

	_getch();
    exit(69);
}