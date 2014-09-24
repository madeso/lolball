#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <map>
#include <string>
class Font; // Font.hpp

class FontManager {
public:
	FontManager();
	~FontManager();

	void loadFont(const std::string& pAlias, const std::string& pFile, float size);
	Font& getFont(const std::string& pAlias);

	Font& getStandardFont();
	void setStandardFont(const std::string& pAlias);

	bool hasFont(const std::string& pAlias) const ;

	// singleton
	static FontManager* getInstancePtr();
	static FontManager& getInstance();
private:
	void removeAllFonts();
	// alias - font
	typedef std::map<std::string, Font*> StringFontMap;
	typedef std::pair<std::string, Font*> StringFontPair;

	StringFontMap mFonts;
	std::string mStandardFont;

	static FontManager* sInstance;
};

#endif