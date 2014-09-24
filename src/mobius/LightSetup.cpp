#include "LightSetup.hpp"
#include <cassert>

#include "PointLightData.hpp"
#include "SpotLightData.hpp"
#include "DirectionalLightData.hpp"
#include "CommonLightAttributes.hpp"
#include "OpenGL.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

LightModelData::LightModelData() : ambient(0, 0, 0) {
}

LightSetup::LightSetup(const LightModelData& pLightModelData) : mCurrentLight(0) {
	// setup the general light model
	float ambientArray[4] = { pLightModelData.ambient.getX(), pLightModelData.ambient.getY(), pLightModelData.ambient.getZ(), 1.0f };
		//{0.1f, 0.1f, 0.1f, 1.0f };
	checkOpengl();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientArray);
	checkOpengl();
}
LightSetup::~LightSetup() {
	for(unsigned int light=0; light<mCurrentLight; ++light) {
		glDisable(GL_LIGHT0 + light);
	}
	checkOpengl();
}

void LightSetup::handle(const PointLightData& pPointLight) {
	const unsigned int light = getNewLight();
	setupAttributes(light, pPointLight);
	handlePosition(light, pPointLight.position);

	glLighti(light, GL_SPOT_EXPONENT, 0);
	checkOpengl();
	glLighti(light, GL_SPOT_CUTOFF, 180);
	checkOpengl();
}
void LightSetup::handle(const SpotLightData& pSpotLight) {
	const unsigned int light = getNewLight();
	setupAttributes(light, pSpotLight);
	handlePosition(light, pSpotLight.position);

	// handle direction
	float directionArray[3] = { pSpotLight.direction.getX(), pSpotLight.direction.getY(), pSpotLight.direction.getZ() };
	glLightfv(light, GL_SPOT_DIRECTION, directionArray);

	glLightf(light, GL_SPOT_EXPONENT, pSpotLight.exponent);
	glLightf(light, GL_SPOT_CUTOFF, pSpotLight.cutoff);
}
void LightSetup::handle(const DirectionalLightData& pDirectionalLight) {
	const unsigned int light = getNewLight();
	setupAttributes(light, pDirectionalLight);
	// handle direction
	float directionArray[4] = { pDirectionalLight.direction.getX(), pDirectionalLight.direction.getY(), pDirectionalLight.direction.getZ(), 0.0f };
	glLightfv(light, GL_POSITION, directionArray);
}

void LightSetup::setupAttributes( unsigned int pLight, const CommonLightAttributes& pLightAttributes ) const {
	// handle colors
	float ambientArray[4] = { pLightAttributes.ambient.getX(), pLightAttributes.ambient.getY(), pLightAttributes.ambient.getZ(), 1.0f };
	glLightfv(pLight, GL_AMBIENT, ambientArray);

	float diffuseArray[4] = { pLightAttributes.diffuse.getX(), pLightAttributes.diffuse.getY(), pLightAttributes.diffuse.getZ(), 1.0f };
	glLightfv(pLight, GL_DIFFUSE, diffuseArray);

	float specularArray[4] = { pLightAttributes.specular.getX(), pLightAttributes.specular.getY(), pLightAttributes.specular.getZ(), 1.0f };
	glLightfv(pLight, GL_SPECULAR, specularArray);

	// handle attenuation
	glLightf(pLight, GL_CONSTANT_ATTENUATION, pLightAttributes.attenuation.constant);
	glLightf(pLight, GL_LINEAR_ATTENUATION, pLightAttributes.attenuation.linear);
	glLightf(pLight, GL_QUADRATIC_ATTENUATION, pLightAttributes.attenuation.quadratic);
	checkOpengl();
}
void LightSetup::handlePosition(unsigned int pLight, const vec3& pPosition) const {
	float positionArray[4] = { pPosition.getX(), pPosition.getY(), pPosition.getZ(), 1.0f };
	glLightfv(pLight, GL_POSITION, positionArray);
	checkOpengl();
}

unsigned int LightSetup::getNewLight() {
	const unsigned int requestedLight = mCurrentLight;
	assert( requestedLight < 8 );
	++ mCurrentLight;
	// according to the gl spec
	const unsigned int light = GL_LIGHT0 + requestedLight;
	glEnable(light);
	checkOpengl();
	return light;
}