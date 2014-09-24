#include "Material.hpp"

#include "AbstractMaterial.hpp"
#include "Image.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

#include "StringUtils.hpp"

Material::Material() : mAmbient(0.2f, 0.2f, 0.2f), mDiffuse(0.8f, 0.8f, 0.8f), mSpecular(1.0f, 1.0f, 1.0f), mEmissive(0.0f, 0.0f, 0.0f), mShininess(0.0f), mAlpha(1.0f) {
}

float Material::getAlpha() const {
	return mAlpha;
}

void Material::setMaterial(const Material& pMaterial) {
	mAmbient = pMaterial.mAmbient;
	mDiffuse = pMaterial.mDiffuse;
	mSpecular = pMaterial.mSpecular;
	mEmissive = pMaterial.mEmissive;
	mShininess = pMaterial.mShininess;
	mAlpha = pMaterial.mAlpha;
	if( pMaterial.mImage.get() ) {
		mImage.reset( new Image(*pMaterial.mImage) );
	}
	else {
		mImage.reset( );
	}
}

void Material::endMaterial(bool pDoLightning, bool pDoTextures) const {
	const bool hasTexture = mImage.get()!=0;
	if( pDoTextures ) {
		if( !hasTexture ) {
			glEnable(GL_TEXTURE_2D);
		}
	}
	else {
		glEnable(GL_TEXTURE_2D);
	}
}

void Material::beginMaterial(bool pDoLightning, bool pDoTextures, float pAlpha) const {
	const bool hasTexture = mImage.get()!=0;
	if( pDoTextures ) {
		if(!hasTexture) {
			glDisable(GL_TEXTURE_2D);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, mImage->getId());
		}
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}
	sendMaterialToOpengl(pDoLightning, pAlpha);
}

void Material::sendMaterialToOpengl(bool pDoLightning, float pAlpha) const {
	const float alpha = mAlpha * pAlpha;
	if( pDoLightning ) {
		float ambientArray[4] = { mAmbient.getX(), mAmbient.getY(), mAmbient.getZ(), alpha };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);

		float diffuseArray[4] = { mDiffuse.getX(), mDiffuse.getY(), mDiffuse.getZ(), alpha };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);

		float specularArray[4] = { mSpecular.getX(), mSpecular.getY(), mSpecular.getZ(), alpha };
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);

		float emissionArray[4] = { mEmissive.getX(), mEmissive.getY(), mEmissive.getZ(), alpha };
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);

		glMaterialf(GL_FRONT, GL_SHININESS, mShininess);
	}
	else {
		glColor4f(mDiffuse.getX(), mDiffuse.getY(), mDiffuse.getZ(), alpha);
	}
}

void Material::setMaterial(const AbstractMaterial& pMaterial) {
	mAmbient = pMaterial.ambient;
	mDiffuse = pMaterial.diffuse;
	mSpecular = pMaterial.specular;
	mEmissive = pMaterial.emissive;
	mShininess = pMaterial.shininess;
	mAlpha = pMaterial.alpha;
	if( "" != trim(pMaterial.texture) ) {
		mImage.reset( new Image(pMaterial.texture) );
	}
}