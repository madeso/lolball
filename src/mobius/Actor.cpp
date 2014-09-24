#include "Actor.hpp"
#include "OpenGl.hpp"

#include "MeshDataManager.hpp"
#include "MeshFile.hpp"
#include "ActorMeshComponent.hpp"

Actor::Actor(const std::string& pName, const vec3& pPosition, const Quaternion& pRotation, bool pRender)
	: Object3(pName, false, pRender, true, CR_INDEPENDENT_POSITION_AND_ROTATION), mRotation(pRotation), mPosition(pPosition){
}
void Actor::teleport(const vec3& pNewPosition, const Quaternion& pNewRotation) {
	mPosition.teleport( pNewPosition );
	mRotation.teleport( pNewRotation );
}
void Actor::move(const vec3& pNewPosition) {
	mPosition.setNewPosition( mPosition.getPosition() + pNewPosition );
}
void Actor::moveTo(const vec3& pNewPosition) {
	mPosition.setNewPosition(pNewPosition);
}
void Actor::increaseRotation(const Quaternion& pRotation) {
	mRotation.increaseRotation(pRotation);
}
void Actor::lookAt(const vec3& pTo, const vec3& pUp) {
	mRotation.lookAt(mPosition.getPosition(), pTo, pUp);
}
void Actor::rotateTo(const Quaternion& pRotation) {
	mRotation.setRotation(pRotation);
}

const vec3& Actor::getPosition() const {
	return mPosition.getPosition();
}
const Quaternion& Actor::getRotation() const {
	return mRotation.get();
}

void Actor::update(float pTime) {
	mRotation.step();
	mPosition.step();
	doUpdate(pTime);
}

void Actor::prepareForRendering(float pTime) {
	mPosition.preparePosition(pTime);
	mRotation.prepareRotation(pTime);
}

void Actor::setupLight(LightSetup* pLightSetup) const {
	glTranslate(mPosition);
	glRotate(mRotation);
	doLight(pLightSetup);
}

const Position3& Actor::getPositionObject() const {
	return mPosition;
}
const Rotation& Actor::getRotationObject() const {
	return mRotation;
}

#include "sdl.h"
#include "SDL_opengl.h"

void Actor::debugDraw() const {
	const float lineLength = 1.5f;
	glTranslate(mPosition);
	//glRotate(mRotation);
	glLineWidth(3.0f);
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getUp() * lineLength );
			glColor3f(0,1,0);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getRight() * lineLength );
			glColor3f(0,0,1);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getIn() * lineLength );
		glEnd();/*
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getUp() * -lineLength );
			glColor3f(0,1,0);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getRight() * -lineLength );
			glColor3f(0,0,1);
			glVertex( op::vec3::origo );
			glVertex( mRotation.getCurrentRotation().getIn() * -lineLength );
		glEnd();*/
	glLineWidth(1.0f);
}

void Actor::loadComponentsFromFile(const std::string& pFileName) {
	MeshFile* file = MeshDataManager::getInstance().getFile(pFileName);
	setFile(file);
	for(MeshFile::MeshDataList::const_iterator meshIterator=file->begin(); meshIterator!=file->end(); ++meshIterator) {
		MeshData* data = *meshIterator;
		addComponent( new ActorMeshComponent(this, data) );
	}
}