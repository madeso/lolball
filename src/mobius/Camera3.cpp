#include "Camera3.hpp"

Camera3::Camera3(const std::string& pName,
				 const vec3& pPosition,
				 const Quaternion& pOrientation)
				 : Actor(pName,
						 pPosition,
						 pOrientation,
#ifdef DEBUG
						 true
#else
						 false
#endif
) {
#ifdef DEBUG
	// load or build camera mesh
#endif
}