#include "SDLSystem.hpp"

#include "SDL_opengl.h"
#include <iostream>
#include "Exceptions.hpp"
#include "Resolution.hpp"
#include "Game.hpp"
#include "Key.hpp"
#include <cassert>
#include "DataPointer.hpp"
#include "Axis.hpp"

#include "StringUtils.hpp"
#include <fstream>
#include <sstream>

bool stringBeginsWidth(const std::string& pString, const std::string& pStart) {
	return pString.substr(0, pStart.length())==pStart;
}

void printToFile(std::ofstream& file, const std::string& include, std::vector<std::string>& extensions, std::vector<std::string>& banList) {
	bool firstExtension = true;
	unsigned int currentExtensionIndex = 0;
	for( std::vector<std::string>::iterator extension=extensions.begin();
			extension != extensions.end();
			++extension) {
		std::string theExtension = (*extension);
		if( stringBeginsWidth(theExtension, include) ) {
			if( ! firstExtension ) {
				file << "<br>" << std::endl;
			}
			file << (*extension);
			firstExtension = false;
			++currentExtensionIndex;
		}
		else {
			banList.push_back(theExtension);
		}
	}
	if( firstExtension ) {
		file << "None";
	}
}

void print(std::ofstream& file, const std::string& title, std::vector<std::string>& extensions) {
	file << "<b>" << title << "!</b><br>" << std::endl;
	bool firstExtension = true;
	unsigned int currentExtensionIndex = 0;
	for( std::vector<std::string>::iterator extension=extensions.begin();
			extension != extensions.end();
			++extension) {
		std::string theExtension = (*extension);
		if( ! firstExtension ) {
			file << "<br>" << std::endl;
		}
		file << (*extension);
		firstExtension = false;
		++currentExtensionIndex;
	}
	if( firstExtension ) {
		file << "None";
	}
}

void print(std::ofstream& file, const std::string& include, std::vector<std::string>& extensions, std::vector<std::string>& banList) {
	file << "<b>" << include << "!</b><br>" << std::endl;
	printToFile(file, include, extensions, banList);
	extensions = banList;
	banList.clear();
}

void printCapsFile() {
	std::ofstream file("caps.html");
	file << "<html><head><title>System Opengl Capabilities</title></head><body>"<< std::endl;

	file << "<b>Open GL Version:</b> "	<< glGetString(GL_VERSION) << "<br>" << std::endl;
	file << "<b>Open GL Vendor:</b> " 	<< glGetString(GL_VENDOR) << "<br>" << std::endl;
	file << "<b>Open GL Renderer:</b> "	<< glGetString(GL_RENDERER) << "<br>" << std::endl;
	file << "<b>Open GL Extensions:</b> " << "<br>" << std::endl;
			
	std::ostringstream extensionString;
	extensionString << glGetString(GL_EXTENSIONS);
	std::vector<std::string> extensions;
	splitString(" ", extensionString.str(), &extensions);
	std::vector<std::string> banList;

	file << "<table border=1> <tr> <td>" << std::endl;
			print(file, "GL_ARB", extensions, banList);
	file << "</td> <td>" << std::endl;
			print(file, "GL_EXT", extensions, banList);
	file << "</td> <td>" << std::endl;
			print(file, "GL_ATI", extensions, banList);
	file << "</td> <td>" << std::endl;
			print(file, "GL_NV", extensions, banList);
	file << "</td> <td>" << std::endl;
			print(file, "other", extensions);
	file << "</td> </tr> </table>" << std::endl;

	file << "</body></html>"<< std::endl;
}

void printAttribute(SDL_GLattr pAttribute, const std::string& pName) {
	int value =0;
	if( SDL_GL_GetAttribute(pAttribute, &value)==0 ) {
		std::cout << pName << ": " << value << std::endl;
	}
	else {
		std::cout << "Failed to get " << pName << std::endl;
	}
}

void SDL_handleInitError(const std::string& pError) {
	throw InitException("SDL " + pError + SDL_GetError() );
}

SDLSystem::SDLSystem(Game* pGame, const std::string& pGameName) : System("SDL"), mGame(pGame), stick(0), joy_dx(0), joy_dy(0) {
	assert(pGame);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
		SDL_handleInitError("failed on init");
	}

	DataPointer<bool> fullscreen("fullscreen");

	int flags;
	if( fullscreen.get() )	flags = SDL_OPENGL | SDL_ANYFORMAT | SDL_FULLSCREEN;
	else					flags = SDL_OPENGL | SDL_ANYFORMAT;

	std::cout << "SDL setting GL attributes" << std::endl;
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );

	std::cout << "SDL: setting title" << std::endl;
	SDL_WM_SetCaption(pGameName.c_str(), "");

	DataPointer<Resolution> resolution("resolution");

	std::cout << "SDL: setting video mode" << std::endl;
	if( !SDL_SetVideoMode(resolution.get().width, resolution.get().height, 32, flags) ) {
		SDL_handleInitError("failed to set videomode");
	}

#define ATTRIBUTE(a)	printAttribute(a, #a)
	ATTRIBUTE(SDL_GL_RED_SIZE);
	ATTRIBUTE(SDL_GL_GREEN_SIZE);
	ATTRIBUTE(SDL_GL_BLUE_SIZE);
	ATTRIBUTE(SDL_GL_ALPHA_SIZE);
	ATTRIBUTE(SDL_GL_BUFFER_SIZE);
	ATTRIBUTE(SDL_GL_DOUBLEBUFFER);
	ATTRIBUTE(SDL_GL_DEPTH_SIZE);
	ATTRIBUTE(SDL_GL_STENCIL_SIZE);
	ATTRIBUTE(SDL_GL_ACCUM_RED_SIZE);
	ATTRIBUTE(SDL_GL_ACCUM_GREEN_SIZE);
	ATTRIBUTE(SDL_GL_ACCUM_BLUE_SIZE);
	ATTRIBUTE(SDL_GL_ACCUM_ALPHA_SIZE);
	ATTRIBUTE(SDL_GL_STEREO);
	ATTRIBUTE(SDL_GL_MULTISAMPLEBUFFERS);
	ATTRIBUTE(SDL_GL_MULTISAMPLESAMPLES);
	ATTRIBUTE(SDL_GL_SWAP_CONTROL);
#undef ATTRIBUTE

#ifndef _DEBUG
	printCapsFile();
#endif

	if( DataPointer<bool>("considerJoystick").get() ) {
		if( SDL_NumJoysticks() > 0 ) {
			DataPointer<int> stickIndex("stick");
			stick = SDL_JoystickOpen(stickIndex.get());
			if( stick == 0 ) {
				SDL_handleInitError("Failed to get joystick");
			}
		}
	}
	else {
		stick = 0;
	}

	SDL_ShowCursor( 0 );
}
SDLSystem::~SDLSystem() {
	if( stick ) {
		SDL_JoystickClose( stick );
		stick = 0;
	}
	SDL_ShowCursor( 1 );
	SDL_Quit();
}
dword SDLSystem::getTicks() {
	return SDL_GetTicks();
}

void SDLSystem::centerCursor() {
	DataPointer<Resolution> resolution("resolution");
	const int w = resolution.get().width/2;
	const int h = resolution.get().height/2;
	SDL_WarpMouse(w, h);
}

// return false to quit, true to contine on
bool SDLSystem::handleEvents() {
	bool continueRunning = true;
	SDL_Event event;
	DataPointer<Resolution> resolution("resolution");

	while( SDL_PollEvent(&event) ) {
		switch(event.type) {
			case SDL_JOYAXISMOTION:
				{
					DataPointer<int> xaxis("xaxis");
					DataPointer<int> yaxis("yaxis");
					DataPointer<int> saxis("saxis");
					const float value = event.jaxis.value / 32767.0f;
					if( event.jaxis.axis == xaxis.get() ) {
						joy_dx = value;
					}
					else if( event.jaxis.axis == yaxis.get() ) {
						joy_dy = value;
					}
					else if( event.jaxis.axis == saxis.get() ) {
						bool keyDown = math::abs(value) > 0.1f;
						static bool oldKeyDown = false;
						if( keyDown != oldKeyDown ) {
							oldKeyDown = keyDown;
							mGame->handleKey(KJ_SLOWMO, keyDown);
						}
					}
				}
				break;
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				{
					DataPointer<int> jumpbutton("jumpbutton");
					DataPointer<int> restartbutton("restartbutton");
					DataPointer<int> pausebutton("pausebutton");
					DataPointer<int> menubutton("menubutton");
					bool keyDown = (event.type == SDL_JOYBUTTONDOWN);
					if( event.jbutton.button == jumpbutton.get() ) {
						mGame->handleKey(KJ_JUMP, keyDown);
					}
					else if( event.jbutton.button == restartbutton.get() ) {
						mGame->handleKey(KJ_RESTART, keyDown);
					}
					else if( event.jbutton.button == pausebutton.get() ) {
						mGame->handleKey(KJ_PAUSE, keyDown);
					}
					else if( event.jbutton.button == menubutton.get() ) {
						mGame->handleKey(KJ_MENU, keyDown);
					}
				}
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				if( DataPointer<bool>("considerKeyboard").get() ) {
					SDL_KeyboardEvent* key=(SDL_KeyboardEvent*) &(event);
					bool keyDown = (key->type == SDL_KEYDOWN);
					Key keyCode = sdlKeyCodeToKey(key->keysym.sym);
					if( keyCode != KS_UNDEFINED) {
						mGame->handleKey(keyCode, keyDown);
					}
#ifdef _DEBUG
					switch( key->keysym.sym ) {
						case SDLK_F1:
						if( keyDown ) {
							continueRunning = false;
						}
						break;
					}
#endif
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if( DataPointer<bool>("considerMouse").get() ) {
					SDL_MouseButtonEvent* key = (SDL_MouseButtonEvent*) &(event);
					bool keyDown = (key->type == SDL_MOUSEBUTTONDOWN);
					Key keyCode = KS_UNDEFINED;
					switch( key->button ) {
						case SDL_BUTTON_LEFT:
							keyCode = KM_LEFT;
							break;
						case SDL_BUTTON_MIDDLE:
							keyCode = KM_MIDDLE;
							break;
						case SDL_BUTTON_RIGHT:
							keyCode = KM_RIGHT;
							break;
					}
					if( keyCode != KS_UNDEFINED) {
						mGame->handleKey(keyCode, keyDown);
					}
				}
				break;
			default:
				break;
		}
	}

	if( stick ) {
		mGame->handleAxis(AXIS_JOY_X, joy_dx);
		mGame->handleAxis(AXIS_JOY_Y, joy_dy*-1);
	}

	const int w = resolution.get().width/2;
	const int h = resolution.get().height/2;

	if( DataPointer<bool>("considerMouse").get() ) {
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		const int max = ::std::max(resolution.get().width, resolution.get().height);
		const real halfMax = max/2.0f;
		if( (x-w)!= 0 )
			mGame->handleAxis(AXIS_MOUSE_X, (x-w)/halfMax);
		if( (h-y)!= 0 )
			mGame->handleAxis(AXIS_MOUSE_Y, (h-y)/halfMax);
	}
	SDL_WarpMouse(w, h);

	return continueRunning;
};