#include "File.hpp"
#include <cassert>
#include "physfs.h"
#include "Exceptions.hpp"

void loadFileIntoString(const std::string& pFileName, std::istringstream* pStream) {
	assert(pStream);
	if( PHYSFS_exists(pFileName.c_str()) ) {
		PHYSFS_file* file = PHYSFS_openRead(pFileName.c_str());
		PHYSFS_sint64 size = PHYSFS_fileLength(file);
		char* buf = new char[size+1];
		PHYSFS_sint64 bytes_read = PHYSFS_read (file, buf, 1, size);
		PHYSFS_close(file);
		buf[size] = 0;
		pStream->clear();
		pStream->str(buf);
		delete[] buf;
	}
	else {
		throw MissingResourceException(pFileName);
	}
}

void saveStringIntoFile(const std::string& pFileName, const std::string& pString) {
	PHYSFS_file* file = PHYSFS_openWrite(pFileName.c_str());
	PHYSFS_write(file, pString.c_str(), sizeof(char), pString.length());
	PHYSFS_close(file);
}