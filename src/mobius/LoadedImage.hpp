#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <string>
	
class LoadedImage {
public:
	LoadedImage(const std::string& p_file );
	~LoadedImage();
		
	unsigned int getId() const;
	
	unsigned int getHeight() const;
	unsigned int getWidth() const;

	void increaseUsage();
	void decreaseUsage();
	unsigned int getUsage() const;
private:
	std::string m_name;
	unsigned int m_id;
	unsigned int m_width;
	unsigned int m_height;

	unsigned int mUsage;
};

void imageCreate();

#endif /*IMAGE_HPP_*/
