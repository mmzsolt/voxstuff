#include "Eigen/Core"
#include "vox.hpp"
#include <iostream>
#include "SDL.h"

SDL_Window *mainWindow;

int SDL_main(int argc, char *argv[])
{
	Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
	Eigen::Vector4f vec(1.0f, 2.0f, 3.0f, 4.0f);
	vec = 2 * mat * vec;
	std::cout << vec;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	mainWindow = SDL_CreateWindow("vox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		512, 512, SDL_WINDOW_OPENGL);

	SDL_Quit();

	return 0;
}