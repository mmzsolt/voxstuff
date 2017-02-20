#include "Eigen/Core"
#include "vox.hpp"
#include <iostream>
#include "SDL.h"
#include "SDL_mouse.h"

#include <GL/glew.h>
#include "gfx/scene.hpp"
#include "gfx/object.hpp"
#include "gfx/node.hpp"
#include "gfx/camera.hpp"
#include "vox.hpp"

SDL_Window *mainWindow;

SDL_GLContext mainContext;

gfx::Scene scene;
gfx::NodePtr node1;
gfx::NodePtr node2;
float cameraSpeed = 0.03f;

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

	SDL_SetRelativeMouseMode(SDL_TRUE);

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
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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
				case SDLK_w:
					scene.getPrimaryCamera()->move({ 0.0f, 0.0f, -1.0f * cameraSpeed });
					break;
				case SDLK_s:
					scene.getPrimaryCamera()->move({ 0.0f, 0.0f, 1.0f * cameraSpeed });
					break;
				case SDLK_a:
					scene.getPrimaryCamera()->move({ -1.0f * cameraSpeed, 0.0f, 0.0f });
					break;
				case SDLK_d:
					scene.getPrimaryCamera()->move({ 1.0f * cameraSpeed, 0.0f, 0.0f });
					break;
				}
			}
		
			if (event.type == SDL_MOUSEMOTION)
			{
				Eigen::Quaternionf quat;
				quat = Eigen::AngleAxisf(0.003f * event.motion.xrel, gfx::Camera::Up);
				quat = quat * Eigen::AngleAxisf(0.003f * event.motion.yrel, Eigen::Vector3f(1.0f, 0.0f, 0.0f));
				scene.getPrimaryCamera()->rotate(quat);
			}
			
		}

		node1->rotateAroundX(0.01f);
		node1->rotateAroundZ(0.01f);
		node2->rotateAroundX(0.02f);
		node2->rotateAroundZ(0.02f);
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

	gfx::RenderParams params1;
	params1.setShader(shader);
	params1.setColor({ 0.9f, 0.2f, 0.7f });
	node1 = gfx::createNode();
	node1->setPosition({ -0.1f, 0.0f, -10.0f });
	params1.setNode(node1);
	scene.addRenderParamsObjectPair(params1, obj);

	obj = gfx::createCube();

	gfx::RenderParams params2;
	params2.setShader(shader);
	params2.setColor({ 0.5f, 0.7f, 0.4f });
	node2 = gfx::createNode();
	node2->setPosition({ 0.1f, 0.0f, -10.0f });
	params2.setNode(node2);
	scene.addRenderParamsObjectPair(params2, obj);

	auto camera = gfx::createCamera();
	camera->setPosition({ 0.0f, 0.0f, 0.0f });
	camera->setTarget({ 0.0f, 0.0f, 1.0f });
	scene.setPrimaryCamera(camera);

	auto voxels = data::loadVoxel("../data/vox/monu0.vox");
	auto voxmesh = data::convertVoxelsToMeshNaively(voxels.first[0], voxels.second);
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
