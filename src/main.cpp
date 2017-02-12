#include "Eigen/Core"
#include "vox.hpp"
#include <iostream>
#include "SDL.h"

#include <GL/glew.h>
#include "gfx/scene.hpp"
#include "gfx/object.hpp"
#include "gfx/node.hpp"
#include "gfx/camera.hpp"

SDL_Window *mainWindow;

SDL_GLContext mainContext;

gfx::Scene scene;
gfx::NodePtr node;

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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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


	mainContext = SDL_GL_CreateContext(mainWindow);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	return true;
}

void Run()
{
	bool loop = true;

	while (loop)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
				}
			}
		}

		node->setPosition({ 0.0f, 0.0f, -4.0f });
		node->rotateAroundX(0.01f);
		node->rotateAroundZ(0.01f);
		scene.render();

		SDL_GL_SwapWindow(mainWindow);
	}
}

void Cleanup()
{
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyWindow(mainWindow);

	SDL_Quit();
}

void Load()
{
	auto shader = gfx::createShader("../data/shaders/simple.vertex", "../data/shaders/simple.fragment");
	auto obj = gfx::createCube();
	obj->setShader(shader);
	node = gfx::createNode();
	node->setPosition({ -5.0f, 0.0f, -20.0f });
	scene.addNodeAndX(node, obj);

	obj = gfx::createCube();
	obj->setShader(shader);
	node = gfx::createNode();
	scene.addNodeAndX(node, obj);
}

int SDL_main(int argc, char *argv[])
{
	if (!Init())
	{
		std::cout << "Could not init\n";
		return 1;
	}

	PrintSDL_GL_Attributes();

	Load();

	Run();

	Cleanup();

	return 0;
}
