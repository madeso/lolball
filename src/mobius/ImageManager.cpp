#include "ImageManager.hpp"

#include "LoadedImage.hpp"
#include <cassert>


ImageManager::ImageManager() {
	assert(!sInstance);
	sInstance = this;
}
ImageManager::~ImageManager() {
	assert(sInstance);
	sInstance = 0;
	// all images should be freede up and removed
	assert( mImages.size() == 0 && "call removeUnusedImages() before destructing this");
}

LoadedImage* ImageManager::loadImage(const std::string& pFileName) {
	StringImageMap::iterator result = mImages.find(pFileName);
	if( result != mImages.end() ) {
		LoadedImage* image = result->second;
		assert(image);
		return image;
	}

	return addImage( pFileName, new LoadedImage(pFileName) );
}
LoadedImage* ImageManager::addImage(const std::string& pFileName, LoadedImage* pImage) {
	assert(pImage);
	assert( !hasImage(pFileName) );
	mImages.insert( StringImagePair(pFileName, pImage) );
	return pImage;
}
bool ImageManager::hasImage(const std::string& pFileName) const {
	StringImageMap::const_iterator result = mImages.find(pFileName);
	return ( result != mImages.end() );
}
void ImageManager::removeUnusedImages() {
	StringImageMap toRemove;
	for( StringImageMap::iterator imageIterator=mImages.begin(); imageIterator != mImages.end(); ++imageIterator) {
		LoadedImage* image = imageIterator->second;
		assert(image);
		if( 0 == image->getUsage() ) {
			toRemove.insert( StringImagePair(imageIterator->first, image) );
		}
	}
	for( StringImageMap::iterator imageIterator=toRemove.begin(); imageIterator != toRemove.end(); ++imageIterator) {
		LoadedImage* image = imageIterator->second;
		assert(image);
		mImages.erase(imageIterator->first);
		delete image;
	}
}

void ImageManager::forceRemoveImages() {
	StringImageMap toRemove;
	for( StringImageMap::iterator imageIterator=mImages.begin(); imageIterator != mImages.end(); ++imageIterator) {
		LoadedImage* image = imageIterator->second;
		assert(image);
		delete image;
	}
	mImages.clear();
}

ImageManager* ImageManager::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}
ImageManager& ImageManager::getInstance() {
	return *getInstancePtr();
}

ImageManager* ImageManager::sInstance = 0;