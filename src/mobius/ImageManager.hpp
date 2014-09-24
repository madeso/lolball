#ifndef IMAGE_MANAGER_HPP
#define IMAGE_MANAGER_HPP

#include <string>
#include <map>

class LoadedImage;

class ImageManager {
public:
	ImageManager();
	~ImageManager();

	LoadedImage* loadImage(const std::string& pFileName);
	bool hasImage(const std::string& pFileName) const;
	void removeUnusedImages();
	void forceRemoveImages();

	static ImageManager* getInstancePtr();
	static ImageManager& getInstance();
private:
	LoadedImage* addImage(const std::string& pFileName, LoadedImage* pImage);
	
	static ImageManager* sInstance;
	
	typedef std::map<std::string, LoadedImage*> StringImageMap;
	typedef std::pair<std::string, LoadedImage*> StringImagePair;

	StringImageMap mImages;
};

#endif