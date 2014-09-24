#include "LoadedImage.hpp"
#include "IL/il.h"
#include "SDL_opengl.h"
#include <iostream>
#include <cassert>
#include "physfs.h"
#include "Exceptions.hpp"
#include <sstream>
#include "OpenGL.hpp"

using namespace std;

/** @todo convert from one data type to another to avoid warnings */
LoadedImage::LoadedImage(const string& pFileName)
				: m_name( pFileName ),
				  m_id(0),
				  m_width(0),
				  m_height(0), mUsage(0) {
		cout << "Loading image: " << m_name << endl;
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);
		{
			if( PHYSFS_exists(pFileName.c_str()) ) {
				PHYSFS_file* file = PHYSFS_openRead(pFileName.c_str());
				PHYSFS_sint64 size = PHYSFS_fileLength(file);

				char* buf = new char[size];
				PHYSFS_sint64 bytes_read = PHYSFS_read(file, buf, 1, size);
				PHYSFS_close(file);
				const ILboolean imageLoaded = ilLoadL(IL_TYPE_UNKNOWN, buf, size);

				if( IL_FALSE == imageLoaded ) {
					throw BadDataException(pFileName);
				}
				delete[] buf;
			}
			else {
				// throw something please
				throw MissingResourceException(pFileName);
			}
		}
		unsigned int tex=0;
		glGenTextures(1, &tex);
	    glBindTexture(GL_TEXTURE_2D, tex);
	    m_width = ilGetInteger(IL_IMAGE_WIDTH);
	    m_height = ilGetInteger(IL_IMAGE_HEIGHT);
		checkOpengl();
		if( true ) {
			int i = 1;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			checkOpengl();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			checkOpengl();
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_width, m_height, // ilGetInteger(IL_IMAGE_FORMAT)
				ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			checkOpengl();
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width,
	    		m_height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		}
		ilDeleteImages(1, &ImageName);
		m_id = tex;
		checkOpengl();
		cout << "Loading done.." << endl;
	}
	
	unsigned int
	LoadedImage::getHeight() const {
		return m_height;
	}
	
	unsigned int
	LoadedImage::getWidth() const {
		return m_width;
	}
	
	LoadedImage::~LoadedImage() {
		cout << "Removing image: " << m_name << endl;
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
	
	unsigned int 
	LoadedImage::getId() const {
		return m_id;
	}

	void imageCreate() {
		cout << "Launching DevIl" << endl;
		ilInit();
		if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
			std::stringstream sstream;
			sstream << "Devil version is different, compiled for: " << IL_VERSION << " driver: " << ilGetInteger(IL_VERSION_NUM);
			throw InitException(sstream.str());
		}
		cout << "Devil version, compiled for: " << IL_VERSION << " driver: " << ilGetInteger(IL_VERSION_NUM) << endl;
	}

	void LoadedImage::increaseUsage() {
		++mUsage;
	}
	void LoadedImage::decreaseUsage() {
		assert( mUsage != 0 );
		--mUsage;
	}
	unsigned int LoadedImage::getUsage() const {
		return mUsage;
	}