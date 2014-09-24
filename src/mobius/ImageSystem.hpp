#ifndef IMAGE_SYSTEM_HPP
#define IMAGE_SYSTEM_HPP

#include "System.hpp"
#include <string>
#include <memory>

#include "ImageManager.hpp"

class ImageSystem : public System {
public:
	ImageSystem();
	~ImageSystem();
private:
	std::auto_ptr<ImageManager> imageManager;
};


#endif