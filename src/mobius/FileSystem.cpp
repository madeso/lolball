#include "FileSystem.hpp"

#include "physfs.h"
#include "Exceptions.hpp"
#include <iostream>

void physfs_handeleInitError(const std::string& pDescription) {
	throw InitException("PhysFS: " + pDescription + ": " + PHYSFS_getLastError());
}

FileSystem::FileSystem(const std::string& pCompanyName, const std::string& pGameName, char* args0) : System("file") {
	int ok = 0;
	ok = PHYSFS_init(args0);
	if( ok==0 ) {
		physfs_handeleInitError("Error on init");
	}

	ok = PHYSFS_addToSearchPath("../", 1);
	if( ok==0 ) {
		physfs_handeleInitError("Error on adding root");
	}
	ok = PHYSFS_addToSearchPath("../images/", 1);
	if( ok==0 ) {
		physfs_handeleInitError("Error on adding images");
	}
	ok = PHYSFS_addToSearchPath("../worldobjects/", 1);
	if( ok==0 ) {
		physfs_handeleInitError("Error on adding world objects");
	}
	ok = PHYSFS_addToSearchPath("../worlds/", 1);
	if( ok==0 ) {
		physfs_handeleInitError("Error on adding worlds");
	}
	ok = PHYSFS_setSaneConfig(pCompanyName.c_str(), pGameName.c_str(), 0, 0, 0);
	if( ok==0 ) {
		physfs_handeleInitError("Error on setting sane config");
	}
}

FileSystem::~FileSystem() {
	int ok = PHYSFS_deinit();
	if( ok==0 ) {
		std::cout << "PhysFS error on exit: " << PHYSFS_getLastError() << std::endl;
	}
}