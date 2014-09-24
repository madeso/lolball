#include "Object3.hpp"
#include "Component3.hpp"
#include "MeshFile.hpp"
#include <cassert>
#include "ObjectMeshComponent.hpp"
#include "MeshDataManager.hpp"
#include "World3.hpp"

Object3::Object3(const std::string& pName, bool pIsLight, bool pIsVisible, bool pIsDynamic, CameraRelation pCameraRelation) : mName(pName), mIsLight(pIsLight), mIsVisible(pIsVisible), mIsDynamic(pIsDynamic), mCameraRelation(pCameraRelation), mCurrentMeshFile(0), mTransperency(1.0f), mWorld(0){
}
void Object3::setFile(MeshFile* pMeshFile) {
	assert(0 == mCurrentMeshFile);
	mCurrentMeshFile = pMeshFile;
	mCurrentMeshFile->increaseUsage();
}

void Object3::hide() {
	mIsVisible = false;

}
void Object3::show() {
	mIsVisible = true;
}

void Object3::freeze() {
	mIsDynamic = false;
}
void Object3::unfreeze() {
	mIsDynamic = true;
}

void Object3::light_enable() {
	mIsLight = true;
}
void Object3::light_disable() {
	mIsLight = false;
}

bool Object3::isLight() const {
	return mIsLight;
}
bool Object3::isVisible() const {
	return mIsVisible;
}
bool Object3::isDynamic() const {
	return mIsDynamic;
}

const std::string& Object3::getName() const {
	return mName;
}

const CameraRelation Object3::getCameraRelation() const {
	return mCameraRelation;
}

void Object3::addComponent(Component3* pNewComponent) {
	assert(pNewComponent);
	mComponents.push_back(pNewComponent);
}
Object3::~Object3() {
	for(ComponentList::iterator componentIterator = mComponents.begin(); componentIterator != mComponents.end(); ++componentIterator) {
		Component3* component = *componentIterator;
		assert(component);
		delete component;
	}
	mComponents.clear();

	if( mCurrentMeshFile ) {
		mCurrentMeshFile->decreaseUsage();
		mCurrentMeshFile = 0;
	}
}
Object3::ComponentList::const_iterator Object3::begin() const {
	return mComponents.begin();
}
Object3::ComponentList::const_iterator Object3::end() const {
	return mComponents.end();
}

void Object3::setTransperency(float pNewTransperency) {
	assert(pNewTransperency < 1.0f);
	assert(pNewTransperency > 0.0f);
	mTransperency = pNewTransperency;
}

float Object3::getTransperency() const {
	return mTransperency;
}

void Object3::loadComponentsFromFile(const std::string &pFileName) {
	MeshFile* file = MeshDataManager::getInstance().getFile(pFileName);
	setFile(file);
	for(MeshFile::MeshDataList::const_iterator meshIterator=file->begin(); meshIterator!=file->end(); ++meshIterator) {
		MeshData* data = *meshIterator;
		addComponent( new ObjectMeshComponent(this, data) );
	}
}

World3& Object3::getWorld() {
	assert(mWorld);
	return *mWorld;
}
void Object3::selectWorld(World3* pWorld) {
	assert(!mWorld);
	assert(pWorld);
	mWorld = pWorld;
}
void Object3::clearWorld() {
	mWorld = 0;
}

void Object3::removeMeFromMyWorld() {
	// this' wont call delete until next update, so we are safe
	getWorld().removeObject(mName);
}