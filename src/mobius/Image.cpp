#include "Image.hpp"
#include "LoadedImage.hpp"
#include "ImageManager.hpp"
#include <cassert>

Image::Image(const std::string& pFileName ) : mImage(0) {
	mImage = ImageManager::getInstance().loadImage(pFileName);
	assert(mImage);
	mImage->increaseUsage();
}
Image::Image( const Image& pOtherImage ) : mImage(pOtherImage.mImage) {
	assert(mImage);
	mImage->increaseUsage();
}
Image::~Image() {
	assert(mImage);
	mImage->decreaseUsage();
}

unsigned int Image::getId() const {
	assert(mImage);
	return mImage->getId();
}

unsigned int Image::getHeight() const {
	assert(mImage);
	return mImage->getHeight();
}
unsigned int Image::getWidth() const {
	assert(mImage);
	return mImage->getWidth();
}