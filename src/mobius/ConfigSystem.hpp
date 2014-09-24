#ifndef CONFIG_SYSTEM_HPP
#define CONFIG_SYSTEM_HPP

#include "Resolution.hpp"
#include "Data.hpp"
#include "math.hpp"

#include "System.hpp"
#include <string>

class ConfigSystem : public System {
public:
	ConfigSystem(const std::string& pConfigPath);
	~ConfigSystem();

	void load();
	void save();
private:
	Data<Resolution> resolution;
	Data<Resolution> saveResolution;
	Data<bool> fullscreen;
	Data<bool> debug;
	Data<real> sensitivity;
	Data<real> master;

	Data<int> xaxis;
	Data<int> yaxis;
	Data<int> saxis;
	Data<int> jumpbutton;
	Data<int> restartbutton;
	Data<int> pausebutton;
	Data<int> menubutton;

	Data<int> stick;

	Data<bool> considerMouse;
	Data<bool> considerKeyboard;
	Data<bool> considerJoystick;

	const std::string mPath;
};

#endif