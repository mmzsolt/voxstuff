#include "Eigen/Core"
#include "vox.hpp"
#include <iostream>
#include "SDL.h"

//#define GLEW_STATIC
#include <GL/glew.h>

SDL_Window *mainWindow;

SDL_Renderer* renderer;

// taken from http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	mainWindow = SDL_CreateWindow("vox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		512, 512, SDL_WINDOW_OPENGL);

	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

}

void Run()
{
	bool loop = true;

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_r:
					// Cover with red and update
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderClear(renderer);
					break;
				case SDLK_g:
					// Cover with green and update
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					SDL_RenderClear(renderer);
					break;
				case SDLK_b:
					// Cover with blue and update
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_RenderClear(renderer);
					break;
				default:
					break;
				}
			}

			SDL_RenderPresent(renderer);
		}
	}
}

void Cleanup()
{
	SDL_DestroyRenderer(renderer);
	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

int SDL_main(int argc, char *argv[])
{
	Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
	Eigen::Vector4f vec(1.0f, 2.0f, 3.0f, 4.0f);
	vec = 2 * mat * vec;
	std::cout << vec;

	if (!Init())
	{
		std::cout << "Could not init\n";
		return 1;
	}

	Run();

	Cleanup();

	return 0;
}
