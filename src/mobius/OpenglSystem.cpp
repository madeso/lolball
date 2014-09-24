#include "OpenglSystem.hpp"
#include "SDL_opengl.h"
#include "DataPointer.hpp"
#include "Resolution.hpp"

#include <iostream>

OpenglSystem::OpenglSystem() : System("OpenGL") {
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	DataPointer<Resolution> resolution("resolution");
	glViewport(0, 0, resolution.get().width, resolution.get().height);

	std::cout << "Open GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Open GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Open GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	{
		int mtxu;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &mtxu);
		std::cout << "Max multi-texturess: " <<  mtxu << std::endl;
	}
	setDisplay2d();
	checkOpengl();
}