#include "FontManager.hpp"
#include "Font.hpp"
#include <cassert>

FontManager* FontManager::sInstance = 0;

FontManager::FontManager() : mStandardFont("std") {
	// keep singleton interface
	assert(!sInstance);
	sInstance = this;
}
FontManager::~FontManager() {
	removeAllFonts();
	// keep singleton interface
	assert(sInstance);
	sInstance = 0;
}

void FontManager::removeAllFonts() {
	for(StringFontMap::iterator stringFontInterator = mFonts.begin();
		stringFontInterator != mFonts.end();
		++stringFontInterator) {
			Font* font = stringFontInterator->second;
			assert(font);
			delete font;
	}
	// all entries are now deleted, make sure we don't try to use them anymore
	mFonts.clear();
}

void FontManager::loadFont(const std::string& pAlias, const std::string& pFile, float size) {
	assert( !hasFont(pAlias) );
	mFonts.insert( StringFontPair(pAlias, new Font(pFile, size) ) );
}

Font& FontManager::getFont(const std::string& pAlias) {
	StringFontMap::const_iterator foundFont = mFonts.find(pAlias);
	assert( foundFont != mFonts.end() );
	Font* font = foundFont->second;
	assert(font);
	return *font;
}

bool FontManager::hasFont(const std::string& pAlias) const {
	StringFontMap::const_iterator foundFont = mFonts.find(pAlias);
	return ( foundFont != mFonts.end() );
}

Font& FontManager::getStandardFont() {
	return getFont( mStandardFont );
}

void FontManager::setStandardFont(const std::string& pAlias) {
	assert( hasFont(pAlias) );
	mStandardFont = pAlias;
}

FontManager* FontManager::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}

FontManager& FontManager::getInstance() {
	return *( getInstancePtr() );
}