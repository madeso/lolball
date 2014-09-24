#ifndef LIGHT_SETUP_HPP
#define LIGHT_SETUP_HPP

struct PointLightData;
struct SpotLightData;
struct DirectionalLightData;

struct CommonLightAttributes;
//class vec3
#include "vec3.hpp"

struct LightModelData {
	LightModelData();
	vec3 ambient;
};

class LightSetup {
public:
	LightSetup(const LightModelData& pLightModelData);
	~LightSetup();

	void handle(const PointLightData& pPointLight);
	void handle(const SpotLightData& pSpotLight);
	void handle(const DirectionalLightData& pDirectionalLight);
private:
	void setupAttributes( unsigned int pLight, const CommonLightAttributes& pLightAttributes ) const ;
	void handlePosition(unsigned int pLight, const vec3& pPosition) const;
	unsigned int getNewLight();

	unsigned int mCurrentLight;
};

#endif