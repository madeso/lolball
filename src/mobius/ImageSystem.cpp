#include "ImageSystem.hpp"

#include "LoadedImage.hpp"
#include "OpenGL.hpp"

ImageSystem::ImageSystem() : System("image") {
	imageCreate();
	checkOpengl();
	imageManager.reset( new ImageManager() );
}
ImageSystem::~ImageSystem() {
	imageManager->forceRemoveImages();
	imageManager.reset();
}