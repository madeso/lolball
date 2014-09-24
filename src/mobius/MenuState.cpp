#include "MenuState.hpp"
#include "FontManager.hpp"
#include "Font.hpp"
#include <cassert>
#include "Object2.hpp"
#include "vec2.hpp"
#include "OpenGL.hpp"
#include "Key.hpp"
#include "World2.hpp"
#include "Axis.hpp"
#include <string>
#include "Exceptions.hpp"

#include "StateStack.hpp"
#include "sdl.h"
#include "SDL_opengl.h"

#include "tinyxml.h"
#include "File.hpp"

#include "Image.hpp"

#include "Resolution.hpp"
#include "DataPointer.hpp"

// yuck yuck, ugly hacky
#include "GameState.hpp"

#include "boost/shared_ptr.hpp"

enum ImguiState {
	IMGUI_UNDEFINED,
	IMGUI_STEP,
	IMGUI_RENDER
};

typedef long ImguiIndex;

void renderRect(const vec2& pPosition, const vec2& pSize, const vec3& pColor, const Image& pImage) {
	glBindTexture(GL_TEXTURE_2D, pImage.getId());
	glColor4f(pColor.getX(), pColor.getY(), pColor.getZ(), 1);
	vec2 mTextureStart(0, 0);
	vec2 mTextureEnd(1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(mTextureStart.getX(), mTextureStart.getY());	glVertex(pPosition);
		glTexCoord2f(mTextureStart.getX(), mTextureEnd.getY());		glVertex(pPosition + vec2(0, pSize.getY()) );
		glTexCoord2f(mTextureEnd.getX(), mTextureEnd.getY());		glVertex(pPosition + pSize );
		glTexCoord2f(mTextureEnd.getX(), mTextureStart.getY());		glVertex(pPosition + vec2(pSize.getX(), 0) );
	glEnd();
}

class Imgui : public Object2 {
public:
	Imgui() : mFont(FontManager::getInstance().getStandardFont()), mState(IMGUI_UNDEFINED), mChecked("checked.png"), mUnchecked("unchecked.png"), mSliderBkg("sliderbkg.png"), mSliderKnob("sliderknob.png"), mCursorActive(false), mCursorPosition(0,0), mCursorIsDown(false), mHot(0), mActive(0), mFocus(0), mKeyEntered(KS_UNDEFINED), mLastWidget(0) {
	}
	virtual void doImgui() = 0;
	bool step(real pTime) {
		mState = IMGUI_STEP;
		mTime = pTime;
		mHot = 0;
		mFocusExist = false;
		doImgui();
		if( mCursorIsDown ) {
			if( mActive == 0 ) {
				mActive = -1;
			}
		}
		else {
			mActive = 0;
		}
		if( mKeyEntered==K_UP || mKeyEntered==K_DOWN ) {
			mFocus = 0;
		}
		if( !mFocusExist ) {
			mFocus = 0;
		}
		mKeyEntered = KS_UNDEFINED;
		return true;
	}
	void render(real pTime) {
		mState = IMGUI_RENDER;
		mTime = pTime;
		doImgui();
#ifdef _DEBUG
		FPRINT(mFont, 0, 0.0f, "hot: " << mHot);
		FPRINT(mFont, 0, 0.05f, "active: " << mActive);
		FPRINT(mFont, 0, 0.1f, "down: " << mCursorIsDown);
		FPRINT(mFont, 0, 0.15f, "focus: " << mFocus);
#endif
	}
	void setCursorPosition(const vec2& pCursorPosition) {
		mCursorPosition = pCursorPosition;
		mCursorActive = true;
	}
	void enterKey(Key pKey) {
		mKeyEntered = pKey;
	}
	void setCursorDown(bool pIsDown) {
		mCursorIsDown = pIsDown;
	}
	bool doCheck(ImguiIndex pIndex, const vec2& pPosition, bool* ioChecked) {
		assert( pIndex != 0 );
		assert( ioChecked );
		const vec2 size(0.05f, 0.05f);
		const bool inside = regionHit(pPosition, size);

		bool changed = false;

		if( mState == IMGUI_STEP ) {
			if( inside ) {
				mHot = pIndex;
				if (mActive == 0 && mCursorIsDown) {
					mActive = pIndex;
				}
			}

			if( !mCursorActive ) {
				if( mFocus == 0 ) mFocus = pIndex;
				if( mFocus == pIndex ) {
					mFocusExist = true;
					switch( mKeyEntered ) {
						case K_UP:
						case K_DOWN:
							mFocus = 0;
							if( mKeyEntered==K_UP ) {
								mFocus = mLastWidget;
							}
							mKeyEntered = KS_UNDEFINED;
							break;
						case K_RETURN:
							changed = true;
							break;
					}
				}
			}

			if (!mCursorIsDown && 
			  mHot == pIndex && mActive == pIndex) changed = true;

			if( changed ) {
				*ioChecked = !(*ioChecked);
			}
		}
		else if( mState == IMGUI_RENDER ) {
			vec3 color = getSuggestedColor(pIndex);

			if( *ioChecked ) {
				renderRect(pPosition, size, color, mChecked);
			}
			else {
				renderRect(pPosition, size, color, mUnchecked);
			}
		}

		mLastWidget = pIndex;
		return changed;
	}
	bool doButton(ImguiIndex pIndex, const vec2& pPosition, const std::string& pLabel) {
		assert( pIndex != 0 );
		const float SHADDOW_DISPLACEMENT = 0.004f;
		const vec2 size(mFont.getWidth(pLabel), mFont.getHeight(pLabel));
		const bool inside = regionHit(pPosition, size);

		bool clicked = false;

		if( mState == IMGUI_STEP ) {
			if( inside ) {
				mHot = pIndex;
				if (mActive == 0 && mCursorIsDown) {
					mActive = pIndex;
				}
			}

			if( !mCursorActive ) {
				if( mFocus == 0 ) mFocus = pIndex;
				if( mFocus == pIndex ) {
					mFocusExist = true;
					switch( mKeyEntered ) {
						case K_UP:
						case K_DOWN:
							mFocus = 0;
							if( mKeyEntered==K_UP ) {
								mFocus = mLastWidget;
							}
							mKeyEntered = KS_UNDEFINED;
							break;
						case K_RETURN:
							clicked = true;
							break;
					}
				}
			}

			if (!mCursorIsDown && 
			  mHot == pIndex && mActive == pIndex) clicked = true;
		}
		else if( mState == IMGUI_RENDER ) {
			vec3 color = getSuggestedColor(pIndex);

			FPRINT(mFont, pPosition.getX() + SHADDOW_DISPLACEMENT*2, pPosition.getY()+ SHADDOW_DISPLACEMENT, FontColor(vec3(0,0,0), 0.2f) << pLabel);
			FPRINT(mFont, pPosition.getX(), pPosition.getY(), FontColor(color, 1.0f) << pLabel);
		}

		mLastWidget = pIndex;
		return clicked;
	}

	bool doSlider(ImguiIndex pIndex, const vec2& pPosition, const vec2& pSize, real* ioValue) {
		assert( pIndex != 0 );
		assert( ioValue );
		const bool inside = regionHit(pPosition, pSize);

		bool changed = false;

		if( mState == IMGUI_STEP ) {
			if( inside ) {
				mHot = pIndex;
				if (mActive == 0 && mCursorIsDown) {
					mActive = pIndex;
				}
			}

			if( !mCursorActive ) {
				if( mFocus == 0 ) mFocus = pIndex;
				if( mFocus == pIndex ) {
					mFocusExist = true;
					switch( mKeyEntered ) {
						case K_UP:
						case K_DOWN:
							mFocus = 0;
							if( mKeyEntered==K_UP ) {
								mFocus = mLastWidget;
							}
							mKeyEntered = KS_UNDEFINED;
							break;
						case K_LEFT:
							changed = true;
							*ioValue = *ioValue - 0.1f, 0.0f;
							break;
						case K_RIGHT:
							changed = true;
							*ioValue = *ioValue + 0.1f;
							break;
					}
				}
			}

			if (mHot == pIndex && mActive == pIndex)  {
					changed = true;
					//			slider value at left of the cursor           center of the knob     scale it to 0-1 range
					*ioValue = (mCursorPosition.getX() - pPosition.getX() - (pSize.getY()/2.0f)) / (pSize.getX()-pSize.getY());
			}

			// if we have changed it, make sure it has a valid value range
			if( changed ) {
				*ioValue = math::limitRange(0, *ioValue, 1);
			}
		}
		else if( mState == IMGUI_RENDER ) {
			vec3 color = getSuggestedColor(pIndex);

			renderRect(pPosition, pSize, color, mSliderBkg);
			const real knobSize = pSize.getY();
			renderRect(pPosition + vec2((pSize.getX()-knobSize)*(*ioValue), 0), vec2(knobSize, knobSize), color, mSliderKnob);
		}

		mLastWidget = pIndex;
		return changed;
	}
protected:
	bool regionHit(const vec2& pPosition, const vec2& pSize) {
		if (mCursorPosition.getX() <  pPosition.getX() ||
			mCursorPosition.getY() <  pPosition.getY() ||
			mCursorPosition.getX() >= pPosition.getX() + pSize.getX() ||
			mCursorPosition.getY() >= pPosition.getY() + pSize.getY() )
		return false;
		return true;
	}

	float getTime() const {
		return mTime;
	}
	bool isUpdating() const {
		return mState == IMGUI_STEP;
	}
private:
	vec3 getSuggestedColor(ImguiIndex pIndex) {
		vec3 color(1, 1, 1);

		const bool isHot = mHot == pIndex;
		const bool isActive = mActive == pIndex;
		const bool reactToHot = mActive==pIndex || mActive==0;
		const bool hasFocus = mFocus == pIndex;

		if( mCursorActive ) {
			if( isHot && reactToHot ) {
				if( isActive ) {
					color = vec3(0.7f, 0.7f, 0.7f);
				}
				else {
					color = vec3(0.5f, 0.5f, 0.5f);
				}
			}
			else {
				if( isActive ) {
					color = vec3(0.5f, 0.5f, 0.5f);
				}
				else {
					color = vec3(0.4f, 0.4f, 0.4f);
				}
			}
		}
		else {
			if( hasFocus ) {
				color = vec3(0.7f, 0.7f, 0.7f);
			}
			else {
				color = vec3(0.4f, 0.4f, 0.4f);
			}
		}
		return color;
	}
	Font& mFont;
	Image mChecked;
	Image mUnchecked;
	Image mSliderBkg;
	Image mSliderKnob;
	ImguiState mState;
	bool mCursorActive;
	vec2 mCursorPosition;
	bool mCursorIsDown;
	ImguiIndex mHot;
	ImguiIndex mActive;
	ImguiIndex mFocus;
	ImguiIndex mLastWidget;
	Key mKeyEntered;
	float mTime;
	bool mFocusExist;
};

class Button {
public:
	Button(Imgui* pImgui, const vec2& pPosition, ImguiIndex pIndex, const std::string& pLabel) : mImgui(pImgui), mPosition(pPosition), mIndex(pIndex), mLabel(pLabel) {
		assert( pImgui );
		assert( pIndex != 0 );
	}

	bool operator()() {
		return operator()(mLabel);
	}
	bool operator()(const std::string& pOverriddenLabel) {
		assert(mImgui);
		return mImgui->doButton(mIndex, mPosition, pOverriddenLabel);
	}
private:
	Imgui* mImgui;
	const vec2 mPosition;
	const ImguiIndex mIndex;
	const std::string mLabel;
};

class Check {
public:
	Check(Imgui* pImgui, const vec2& pPosition, ImguiIndex pIndex) : mImgui(pImgui), mPosition(pPosition), mIndex(pIndex) {
		assert( pImgui );
		assert( pIndex != 0 );
	}

	bool operator()(bool* ioData) {
		assert(mImgui);
		assert(ioData);
		return mImgui->doCheck(mIndex, mPosition, ioData);
	}
private:
	Imgui* mImgui;
	const vec2 mPosition;
	const ImguiIndex mIndex;
};


class Slider {
public:
	Slider(Imgui* pImgui, const vec2& pPosition, const vec2& pSize, ImguiIndex pIndex) : mImgui(pImgui), mPosition(pPosition), mSize(pSize), mIndex(pIndex) {
		assert( pImgui );
		assert( pIndex != 0 );
	}

	bool operator()(real* ioData) {
		assert(mImgui);
		assert(ioData);
		return mImgui->doSlider(mIndex, mPosition, mSize, ioData);
	}
private:
	Imgui* mImgui;
	const vec2 mPosition;
	const vec2 mSize;
	const ImguiIndex mIndex;
};


class Label {
public:
	Label(const std::string& pText, const vec2& pPosition) : mFont(FontManager::getInstance().getStandardFont()), mText(pText), mPosition(pPosition) {
	}
	void doLabel() {
		FPRINT(mFont, mPosition.getX(), mPosition.getY(), FontColor(vec3(0.5f, 0.5f, 0.5f), 1) << mText);
	}
private:
	Font& mFont;
	const std::string mText;
	const vec2 mPosition;
};

class BasicImage {
public:
	BasicImage(const std::string& pFileName,
			   const vec2& pLocation,
			   const vec2& pSize,
			   const vec2& pMovement,
			   const vec2& pTextureStart,
			   const vec2& pTextureEnd) : mImage(pFileName),
										  mLocation(pLocation),
										  mSize(pSize),
										  mMovement(pMovement),
										  mTextureStart(pTextureStart),
										  mTextureEnd(pTextureEnd),
										  mTextureDisplacement(0,0) {
	}
	void step(real pTime) {
		mTextureDisplacement += mMovement * pTime;
		// limmit to some standard
		mTextureDisplacement.setX( math::wrapRange(0, mTextureDisplacement.getX(), 1) );
		mTextureDisplacement.setY( math::wrapRange(0, mTextureDisplacement.getY(), 1) );
	}
	void runImage() {
		glBindTexture(GL_TEXTURE_2D, mImage.getId());
		glMatrixMode( GL_TEXTURE );
		glTranslatef(mTextureDisplacement.getX(), mTextureDisplacement.getY(), 0);
		glMatrixMode( GL_MODELVIEW );
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(mTextureStart.getX(), mTextureStart.getY());	glVertex(mLocation);
			glTexCoord2f(mTextureStart.getX(), mTextureEnd.getY());		glVertex(mLocation + vec2(0, mSize.getY()) );
			glTexCoord2f(mTextureEnd.getX(), mTextureEnd.getY());		glVertex(mLocation + mSize );
			glTexCoord2f(mTextureEnd.getX(), mTextureStart.getY());		glVertex(mLocation + vec2(mSize.getX(), 0) );
		glEnd();
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		glMatrixMode( GL_MODELVIEW );
	}
private:
	Image mImage;
	const vec2 mLocation;
	const vec2 mSize;
	const vec2 mMovement;
	const vec2 mTextureStart;
	const vec2 mTextureEnd;
	vec2 mTextureDisplacement;
};

class ImguiObject;

class Background {
public:
	Background(ImguiObject* pOwner) : mOwner(pOwner) {
	}
	void operator()();

	void renderBackground() {
		for(ImageList::iterator im=mImages.begin(); im != mImages.end(); ++im) {
			(*im)->runImage();
		}
		for(LabelList::iterator lbl=mLabels.begin(); lbl != mLabels.end(); ++lbl) {
			(*lbl)->doLabel();
		}
	}

	void stepBackground(real pTime) {
		for(ImageList::iterator im=mImages.begin(); im != mImages.end(); ++im) {
			(*im)->step(pTime);
		}
	}

	void addImage( BasicImage* pImage) {
		assert(pImage);
		mImages.push_back(pImage);
	}
	void addLabel(Label* pLabel) {
		assert(pLabel);
		mLabels.push_back(pLabel);
	}
private:
	typedef std::list<BasicImage*> ImageList;
	typedef std::list<Label*> LabelList;
	LabelList mLabels;
	ImageList mImages;
	ImguiObject* mOwner;
};

vec2 getVector(TiXmlElement* pElement, const char* pNameX, const char* pNameY, real pDefaultValue=0) {
	TiXmlElement* data = pElement; //pElement->FirstChildElement("position");
	if( !data ) return vec2(pDefaultValue,pDefaultValue);
	float x=pDefaultValue, y=pDefaultValue, temp=pDefaultValue;
#define ATTRIBUTE(name, store) if( TIXML_SUCCESS==data->QueryFloatAttribute(name, &temp) ) store=temp
	ATTRIBUTE(pNameX, x);
	ATTRIBUTE(pNameY, y);
#undef ATTRIBUTE
	return vec2(x, y);
}

class ImguiObject : public Imgui {
public:
	void doBackground(Background& pBackground) {
		if( isUpdating() ) {
			pBackground.stepBackground(getTime());
		}
		else {
			pBackground.renderBackground();
		}
	}
protected:
	ImguiObject(const std::string& pFileName) {
		std::istringstream stream;
		loadFileIntoString(pFileName, &stream);
		TiXmlDocument doc;
		doc.Parse(stream.str().c_str());
		TiXmlHandle docHandle( &doc );
		ImguiIndex index = 1;
		for(TiXmlElement* entity = docHandle.FirstChild("menu").FirstChild("button").ToElement(); entity; entity = entity->NextSiblingElement("button") ) {
			const char* namePointer = entity->Attribute("name");
			const std::string name = namePointer?namePointer:"";
			const char* labelPointer = entity->Attribute("label");
			const std::string label = labelPointer?labelPointer:"";
			const vec2 location = getVector(entity, "x", "y");
			buildButton(name, location, index, label);
			++index;
		}
		for(TiXmlElement* entity = docHandle.FirstChild("menu").FirstChild("check").ToElement(); entity; entity = entity->NextSiblingElement("check") ) {
			const char* namePointer = entity->Attribute("name");
			const std::string name = namePointer?namePointer:"";
			const vec2 location = getVector(entity, "x", "y");
			buildCheck(name, location, index);
			++index;
		}
		for(TiXmlElement* entity = docHandle.FirstChild("menu").FirstChild("slider").ToElement(); entity; entity = entity->NextSiblingElement("slider") ) {
			const char* namePointer = entity->Attribute("name");
			const std::string name = namePointer?namePointer:"";
			const vec2 location = getVector(entity, "x", "y");
			const vec2 size = getVector(entity, "w", "h");
			buildSlider(name, location, size, index);
			++index;
		}
		for(TiXmlElement* entity = docHandle.FirstChild("menu").FirstChild("background").ToElement(); entity; entity = entity->NextSiblingElement("background") ) {
			const char* namePointer = entity->Attribute("name");
			const std::string name = namePointer?namePointer:"";
			Background* b = buildBackground(name);
			for( TiXmlElement* child = entity->FirstChildElement("image"); child; child = child->NextSiblingElement("image") ) {
				const char* filePointer = child->Attribute("file");
				const std::string file = filePointer?filePointer:"";
				const vec2 location =		getVector(child, "x", "y", 0);
				const vec2 size =			getVector(child, "w", "h", 1);
				const vec2 movement =		getVector(child, "movex", "movey", 0);
				const vec2 textureStart =	getVector(child, "texsx", "texsy", 0);
				const vec2 textureEnd =		getVector(child, "texex", "texey", 1);
				b->addImage( new BasicImage(file, location, size, movement, textureStart, textureEnd) );
			}
			for( TiXmlElement* child = entity->FirstChildElement("label"); child; child = child->NextSiblingElement("label") ) {
				const char* textPointer = child->Attribute("text");
				const std::string text = textPointer?textPointer:"";
				const vec2 location =		getVector(child, "x", "y", 0);
				b->addLabel( new Label(text, location) );
			}
		}
	}
	Button& getButton(const std::string& pName) {
		StringButtonMap::iterator res = mButtons.find(pName);
		if( res == mButtons.end() ) {
			std::ostringstream str;
			str << "Failed to find button named: " << pName;
			throw InitException(str.str());
		}
		return *(res->second);
	}
	Check& getCheck(const std::string& pName) {
		StringCheckMap::iterator res = mCheck.find(pName);
		if( res == mCheck.end() ) {
			std::ostringstream str;
			str << "Failed to find check named: " << pName;
			throw InitException(str.str());
		}
		return *(res->second);
	}
	Slider& getSlider(const std::string& pName) {
		StringSliderMap::iterator res = mSliders.find(pName);
		if( res == mSliders.end() ) {
			std::ostringstream str;
			str << "Failed to find slider named: " << pName;
			throw InitException(str.str());
		}
		return *(res->second);
	}
	Background& getBackground(const std::string& pName) {
		StringBackgroundMap::iterator res = mBackgrounds.find(pName);
		if( res == mBackgrounds.end() ) {
			std::ostringstream str;
			str << "Failed to find background named: " << pName;
			throw InitException(str.str());
		}
		return *(res->second);
	}
private:
	void buildButton(const std::string& pName, const vec2& pPosition, ImguiIndex pIndex, const std::string& pLabel) {
		ButtonPtr button(new Button(this, pPosition, pIndex, pLabel));
		mButtons.insert( StringButtonPair(pName, button ) );
	}
	void buildCheck(const std::string& pName, const vec2& pPosition, ImguiIndex pIndex) {
		CheckPtr check(new Check(this, pPosition, pIndex));
		mCheck.insert( StringCheckPair(pName, check ) );
	}
	void buildSlider(const std::string& pName, const vec2& pPosition, const vec2& pSize, ImguiIndex pIndex) {
		SliderPtr slider(new Slider(this, pPosition, pSize, pIndex));
		mSliders.insert( StringSliderPair(pName, slider ) );
	}

	Background* buildBackground(const std::string& pName) {
		Background* bkg = new Background(this);
		BackgroundPtr ptr(bkg);
		mBackgrounds.insert( StringBackgroundPair(pName, ptr ) );
		return bkg;
	}

	typedef boost::shared_ptr<Button> ButtonPtr;
	typedef std::map<std::string,  ButtonPtr > StringButtonMap;
	typedef std::pair<std::string, ButtonPtr > StringButtonPair;

	typedef boost::shared_ptr<Check> CheckPtr;
	typedef std::map<std::string,  CheckPtr > StringCheckMap;
	typedef std::pair<std::string, CheckPtr > StringCheckPair;

	typedef boost::shared_ptr<Slider> SliderPtr;
	typedef std::map<std::string,  SliderPtr> StringSliderMap;
	typedef std::pair<std::string, SliderPtr > StringSliderPair;

	typedef boost::shared_ptr<Background> BackgroundPtr;
	typedef std::map<std::string,  BackgroundPtr > StringBackgroundMap;
	typedef std::pair<std::string, BackgroundPtr > StringBackgroundPair;

	StringButtonMap mButtons;
	StringCheckMap mCheck;
	StringSliderMap mSliders;
	StringBackgroundMap mBackgrounds;
};

void Background::operator()() {
	mOwner->doBackground(*this);
}

class MyMenu : public ImguiObject {
public:
	MyMenu() : ImguiObject("meny.xml"),
			mMenuState(MS_MAIN),

			mMainBkg(getBackground("main")),
			mMainNewGame(getButton("new")),
			mMainContinueGame(getButton("continue")),
			mMainControls(getButton("controls")),
			mMainOptions(getButton("options")),
			mMainCredits(getButton("credits")),
			mMainQuitGame(getButton("quit")),
			mMainAbortGame(getButton("abort")),

			mQuitBkg(getBackground("quit")),
			mQuitYes(getButton("yes")),
			mQuitNo(getButton("no")),

			mControlsBkg(getBackground("controls")),
			mControlsBack(getButton("controls.back")),

			mCreditsBkg(getBackground("credits")),
			mCreditsBack(getButton("credits.back")),

			mOptionsBkg(getBackground("options")),
			mOptionsFullscreen( getCheck("fullscreen") ),
			mOptionsSensitivity( getSlider("sensitivity") ),
			mOptionsVolume( getSlider("master") ),
			mOptionsResolution(getButton("resolution")),
			mOptionsBack(getButton("options.back")) {
	}
	void doImgui() {
		switch( mMenuState ) {
		case MS_MAIN:
			mMainBkg();
			if( GameState::getInstance().hasWorld() ) {
				if( mMainContinueGame() ) {
					StateStack::getInstance().enableState("game");
				}
			}
			else {
				if( mMainNewGame() ) {
					GameState::getInstance().doLoadWorld("level01");
					GameState::getInstance().resetScore();
					StateStack::getInstance().enableState("game");
				}
			}
			
			if( mMainControls() ) {
				mMenuState = MS_CONTROLS;
			}
			if( mMainOptions() ) {
				mMenuState = MS_OPTIONS;
			}
			if( mMainCredits() ) {
				mMenuState = MS_CREDITS;
			}
			if( GameState::getInstance().hasWorld() ) {
				if( mMainAbortGame() ) {
					GameState::getInstance().unloadLevel();
				}
			}
			else {
				if( mMainQuitGame() ) {
					mMenuState = MS_QUIT;
				}
			}
			break;
		case MS_CONTROLS:
			mControlsBkg();
			if( mControlsBack() ) {
				mMenuState = MS_MAIN;
			}
			break;
		case MS_CREDITS:
			mCreditsBkg();
			if( mCreditsBack() ) {
				mMenuState = MS_MAIN;
			}
			break;
		case MS_OPTIONS:
			mOptionsBkg();
			{
				std::ostringstream ostr;
				DataPointer<Resolution> resolution("saveResolution");
				ostr << resolution.get().width << "x" << resolution.get().height;

				if( mOptionsResolution(ostr.str()) ) {
					const int MAX = 5;
					const static Resolution possibleResolutions[MAX] = {
						Resolution(640, 480),
						Resolution(800, 600),
						Resolution(1024, 768),
						Resolution(1152, 864),
						Resolution(1280, 1024)
					};
					int index = -1;
					for(int i=0; i< MAX; ++i) {
						if( resolution.get() == possibleResolutions[i] ) {
							index = i;
							break;
						}
					}
					index++;
					if( index == MAX ) {
						index = 0;
					}

					resolution.set(possibleResolutions[index]);
				}
			}
			{
				DataPointer<bool> fullscreen("fullscreen");
				bool full = fullscreen.get();
				if( mOptionsFullscreen(&full) ) {
					fullscreen.set(full);
				}
			}
			{
				DataPointer<real> sensitivity("sensitivity");
				const real scale = 20.0f;
				real sens = sensitivity.get() / scale;
				if( mOptionsSensitivity(&sens) ) {
					sensitivity.set( sens * scale);
				}
			}
			{
				DataPointer<real> master("master");
				real vol = master.get();
				if( mOptionsVolume(&vol) ) {
					master.set(vol);
				}
			}
			if( mOptionsBack() ) {
				mMenuState = MS_MAIN;
			}
			break;
		case MS_QUIT:
			mQuitBkg();
			if( mQuitNo() ) {
				mMenuState = MS_MAIN;
			}
			if( mQuitYes() ) {
				StateStack::getInstance().disableState("menu");
			}
			break;
		default:
			assert(0 || "Bad state");
		}
		
		
	}
private:
	enum MenuState {
		MS_MAIN,
		MS_CONTROLS,
		MS_OPTIONS,
		MS_CREDITS,
		MS_QUIT
	};
	MenuState mMenuState;

	// main
	Background& mMainBkg;
	Button& mMainNewGame;
	Button& mMainContinueGame;
	Button& mMainControls;
	Button& mMainOptions;
	Button& mMainCredits;
	Button& mMainQuitGame;
	Button& mMainAbortGame;

	// quit
	Background& mQuitBkg;
	Button& mQuitYes;
	Button& mQuitNo;

	// controls
	Background& mControlsBkg;
	Button& mControlsBack;

	// credits
	Background& mCreditsBkg;
	Button& mCreditsBack;

	// options
	Background& mOptionsBkg;
	Check& mOptionsFullscreen;
	Slider& mOptionsSensitivity;
	Slider& mOptionsVolume;
	Button& mOptionsResolution;
	Button& mOptionsBack;
};

class Cursor : public Object2 {
public:
	Cursor() : Object2(), mFont(FontManager::getInstance().getStandardFont()), mPosition(0.5f,0.5f) {
	}

	void move(const vec2& pMovement) {
		mPosition += pMovement;
		mPosition.setX( math::limitRange(0, mPosition.getX(), 1) );
		mPosition.setY( math::limitRange(0, mPosition.getY(), 1) );
	}

	const vec2& getPositon() const {
		return mPosition;
	}

	bool step(real pTime) {
		return true;
	}

	void render(real pTime) {
		FPRINT(mFont, mPosition.getX(), mPosition.getY(), FontColor(vec3(0,0,0), 1) << "^");
	}
private:
	Font& mFont;
	vec2 mPosition;
};

struct MenuState::MenuStatePimpl {
	MenuStatePimpl(MenuState* pMenuState) : cursorAdded(false), cursor(0), cursorMovement(0,0) {
		menu = new MyMenu();
		overlay.addObject( menu );
	}

	void step(float pTime) {
		if( !cursorAdded ) {
			if( cursorMovement.getLength() > 0.3f ) {
				cursorAdded = true;
				cursor = new Cursor();
				overlay.addObject( cursor );
			}
		}
		else {
			assert(cursor);
			cursor->move( vec2(cursorMovement.getX(), -cursorMovement.getY()) );
			menu->setCursorPosition(cursor->getPositon());
		}
		overlay.step(pTime);
		cursorMovement = vec2(0,0);
	}
	void render(float pTime) {
		overlay.render(pTime);
	}
	void onKey(Key pKey, bool pIsDown) {
		switch(pKey) {
		case KM_LEFT:
			menu->setCursorDown(pIsDown);
			break;
		case K_UP:
		case K_DOWN:
		case K_LEFT:
		case K_RIGHT:
		case K_RETURN:
			if( pIsDown ) {
				menu->enterKey(pKey);
			}
			break;
		case KJ_JUMP:
			if( pIsDown ) {
				menu->enterKey(K_RETURN);
			}
			break;
		}
	}
	void onAxis(Axis pAxis, real pValue) {
		static int joyXState = 0;
		static int joyYState = 0;
		int newJoyXState = joyXState;
		int newJoyYState = joyYState;
		const float LIM = 0.7f;
		switch( pAxis ) {
			case AXIS_MOUSE_X:
				cursorMovement.setX( cursorMovement.getX() + pValue);
				break;
			case AXIS_MOUSE_Y:
				cursorMovement.setY( cursorMovement.getY() + pValue);
				break;
			case AXIS_JOY_X:
				if( pValue > LIM ) {
					newJoyXState = 1;
				}
				else if( pValue < -LIM ) {
					newJoyXState = -1;
				}
				else {
					newJoyXState = 0;
				}
				break;
			case AXIS_JOY_Y:
				if( pValue > LIM ) {
					newJoyYState = 1;
				}
				else if( pValue < -LIM ) {
					newJoyYState = -1;
				}
				else {
					newJoyYState = 0;
				}
				break;
		}
		if( newJoyXState != joyXState) {
			joyXState = newJoyXState;
			if( newJoyXState==1 ) {
				menu->enterKey(K_RIGHT);
			}
			else if( newJoyXState == -1) {
				menu->enterKey(K_LEFT);
			}
		}
		if( newJoyYState != joyYState) {
			joyYState = newJoyYState;
			if( newJoyYState==1 ) {
				menu->enterKey(K_UP);
			}
			else if( newJoyYState == -1) {
				menu->enterKey(K_DOWN);
			}
		}
	}

	Cursor* cursor;
	MyMenu* menu;
	bool cursorAdded;
	vec2 cursorMovement;
	World2 overlay;
};

MenuState::MenuState() : State("menu"), mPimpl() {
	mPimpl.reset( new MenuStatePimpl(this) );
}
MenuState::~MenuState() {
}

void MenuState::onStep(float pTime, unsigned int pDepth) {
	mPimpl->step(pTime);
}
void MenuState::onFrame(float pTime, unsigned int pDepth) {
}
void MenuState::onRender(float pTime, unsigned int pDepth) {
	mPimpl->render(pTime);
}
void MenuState::onKey(Key pKey, bool pIsDown) {
	mPimpl->onKey(pKey, pIsDown);
}
void MenuState::onAxis(Axis pAxis, real pValue) {
	mPimpl->onAxis(pAxis, pValue);
}
void MenuState::onTop() {
}
void MenuState::onNoMoreTop() {
}