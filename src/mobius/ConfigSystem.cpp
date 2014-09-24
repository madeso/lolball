#include "ConfigSystem.hpp"

#include "tinyxml.h"
#include "Resolution.hpp"

ConfigSystem::ConfigSystem(const std::string& pConfigPath) : System("config"), resolution("resolution", Resolution(800, 600)), saveResolution("saveResolution", Resolution(800, 600)), fullscreen("fullscreen", false), debug("debug", false), sensitivity("sensitivity", 1.0f), master("master", 1.0f),
					xaxis("xaxis", 0),
					yaxis("yaxis", 1),
					saxis("saxis", 2),
					jumpbutton("jumpbutton", 0),
					restartbutton("restartbutton", 1),
					pausebutton("pausebutton", 7),
					menubutton("menubutton", 6),
					stick("stick", 0),
					considerMouse("considerMouse", true),
					considerKeyboard("considerKeyboard", true),
					considerJoystick("considerJoystick", true),
					mPath(pConfigPath) {
	load();
}

ConfigSystem::~ConfigSystem() {
	save();
}

void ConfigSystem::load() {
	TiXmlDocument doc(mPath);
	if( doc.LoadFile() ) {
		TiXmlHandle docHandle( &doc );
		TiXmlElement* resolutionElement = docHandle.FirstChild("config").FirstChild("resolution").Element();
		if( resolutionElement ) {
			int resolutionWidth = 800;
			int resolutionHeight = 600;
			if( resolutionElement->QueryIntAttribute("width", &resolutionWidth)==TIXML_SUCCESS
				&& resolutionElement->QueryIntAttribute("height", &resolutionHeight)==TIXML_SUCCESS ) {
					resolution.set(Resolution(resolutionWidth, resolutionHeight) );
					saveResolution.set(Resolution(resolutionWidth, resolutionHeight) );
			}
		}
		TiXmlElement* fullscreenElement = docHandle.FirstChild("config").FirstChild("fullscreen").Element();
		if( fullscreenElement ) {
			int fullscreenValue = 0;
			if( fullscreenElement->QueryIntAttribute("value", &fullscreenValue)==TIXML_SUCCESS ) {
				fullscreen.set( ( fullscreenValue==1 ) );
			}
		}

		TiXmlElement* sensitivityElement = docHandle.FirstChild("config").FirstChild("sensitivity").Element();
		if( sensitivityElement ) {
			double sensitivityValue = 0;
			if( sensitivityElement->QueryDoubleAttribute("value", &sensitivityValue)==TIXML_SUCCESS ) {
				sensitivity.set( sensitivityValue );
			}
		}

		TiXmlElement* masterElement = docHandle.FirstChild("config").FirstChild("master").Element();
		if( masterElement ) {
			double masterValue = 0;
			if( masterElement->QueryDoubleAttribute("value", &masterValue)==TIXML_SUCCESS ) {
				master.set( masterValue );
			}
		}

		TiXmlElement* debugElement = docHandle.FirstChild("config").FirstChild("debug").Element();
		if( debugElement ) {
			int debugValue = 0;
			if( debugElement->QueryIntAttribute("value", &debugValue)==TIXML_SUCCESS ) {
				debug.set( ( debugValue==1 ) );
			}
		}

		TiXmlElement* element;
		element = docHandle.FirstChild("config").FirstChild("xaxis").Element();
		if( element ) {
			int value = 0;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				xaxis.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("yaxis").Element();
		if( element ) {
			int value = 0;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				yaxis.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("saxis").Element();
		if( element ) {
			int value = 0;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				saxis.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("jumpbutton").Element();
		if( element ) {
			int value = 0;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				jumpbutton.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("restartbutton").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				restartbutton.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("pausebutton").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				pausebutton.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("menubutton").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				menubutton.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("stick").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				stick.set( value );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("considerJoystick").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				considerJoystick.set( value!=0 );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("considerMouse").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				considerMouse.set( value!=0 );
			}
		}

		element = docHandle.FirstChild("config").FirstChild("considerKeyboard").Element();
		if( element ) {
			int value = 1;
			if( element->QueryIntAttribute("value", &value)==TIXML_SUCCESS ) {
				considerKeyboard.set( value!=0 );
			}
		}
	}
	else {
		std::cout << "Failed to load config file" << std::endl;
	}
}
void ConfigSystem::save() {
	TiXmlDocument doc;
	doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
	TiXmlElement* config = new TiXmlElement("config");
	TiXmlElement* element = 0;

	element = new TiXmlElement("resolution");
	element->SetAttribute("width", saveResolution.get().width);
	element->SetAttribute("height", saveResolution.get().height);
	config->LinkEndChild(element);

	element = new TiXmlElement("fullscreen");
	element->SetAttribute("value", fullscreen.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("sensitivity");
	element->SetDoubleAttribute("value", sensitivity.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("master");
	element->SetDoubleAttribute("value", master.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("debug");
	element->SetAttribute("value", debug.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("xaxis");
	element->SetAttribute("value", xaxis.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("yaxis");
	element->SetAttribute("value", yaxis.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("saxis");
	element->SetAttribute("value", saxis.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("jumpbutton");
	element->SetAttribute("value", jumpbutton.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("restartbutton");
	element->SetAttribute("value", restartbutton.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("pausebutton");
	element->SetAttribute("value", pausebutton.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("menubutton");
	element->SetAttribute("value", menubutton.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("stick");
	element->SetAttribute("value", stick.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("considerJoystick");
	element->SetAttribute("value", considerJoystick.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("considerKeyboard");
	element->SetAttribute("value", considerKeyboard.get());
	config->LinkEndChild(element);

	element = new TiXmlElement("considerMouse");
	element->SetAttribute("value", considerMouse.get());
	config->LinkEndChild(element);

	doc.LinkEndChild(config);
	doc.SaveFile(mPath);
}