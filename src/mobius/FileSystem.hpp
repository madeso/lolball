#ifndef FILE_SYSTEM_HPP
#define FILE_SYSYEM_HPP

#include "System.hpp"
#include <string>

#include "physfs.h"
class FileSystem : public System {
public:
	FileSystem(const std::string& pCompanyName, const std::string& pGameName, char* args0);
	~FileSystem();
};

#endif