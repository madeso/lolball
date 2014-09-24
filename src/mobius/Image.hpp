#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

class LoadedImage;

class Image {
public:
	Image(const std::string& pFileName );
	Image( const Image& pOtherImage );
	~Image();
		
	unsigned int getId() const;
	
	unsigned int getHeight() const;
	unsigned int getWidth() const;
private:
	LoadedImage* mImage;
};

#endif