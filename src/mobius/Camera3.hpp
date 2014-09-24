#ifndef CAMERA3_HPP
#define CAMERA3_HPP

#include "Actor.hpp"

class Camera3 : public Actor {
public:
	Camera3(const std::string& pName, const vec3& pPosition, const Quaternion& pOrientation);
};

#endif