#include "Exceptions.hpp"
#include <iostream>
#include <sstream>

#include "sdl.h"

#ifndef _DEBUG

#if defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN 
#include "windows.h" 
#pragma message("including windows")
#endif

void displayExcdeption(const std::string& pException) {
#if defined(__WIN32__)
        MessageBoxA(NULL, pException.c_str(), "Error! Error! Error!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#pragma message("displaying exceptions with a message box")
#else
        std::cerr << "Exception:\n";
        std::cerr << e.getFullDescription().c_str() << "\n";
		#pragma message("displaying exceptions to cerr")
#endif
}

#pragma message("handling exceptions")
void handleExceptions() {
	std::ostringstream str;
	try {
		throw;
	}
	catch(BadDataException& pException) {
		str << "Data in the file: " << pException.what() << " had a bad format" << std::endl;
	}
	catch(MissingResourceException& pException) {
		str << "Missing file: " << pException.what() << std::endl;
	}
	catch(InitException& pException) {
		str << "Failed to init due to: " << pException.what() << std::endl;
	}
	catch( std::string& pCause) {
		str << pCause;
	}
	catch( char* pCause ) {
		str << pCause;
	}
	catch( ... ) {
		str << "Unknown cause";
	}

	displayExcdeption(str.str());
}
#else
#pragma message("throwing exceptions to the ide")
void handleExceptions() {
	throw;
}
#endif