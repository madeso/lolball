#ifndef MATERIAL_HPP
#define MATERIAL_HPP

struct AbstractMaterial;

#include <memory>
#include "vec3.hpp"
#include "Image.hpp"

class Material {
public:
	Material();

	void beginMaterial(bool pDoLightning, bool pDoTextures, float pAlpha) const;
	void endMaterial(bool pDoLightning, bool pDoTextures) const ;
	void setMaterial(const AbstractMaterial& pMaterial);
	void setMaterial(const Material& pMaterial);

	float getAlpha() const;
private:
	void sendMaterialToOpengl(bool pDoLightning, float pAlpha) const;
	std::auto_ptr<Image> mImage;
	vec3 mAmbient;
	vec3 mDiffuse;
	vec3 mSpecular;
	vec3 mEmissive;
	float mShininess;
	float mAlpha;
};

#endif