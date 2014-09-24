#include "World2.hpp"
#include "Object2.hpp"
#include "OpenGL.hpp"
#include <cassert>

#include "sdl.h"
#include "SDL_opengl.h"

World2::World2() {
}
World2::~World2() {
	removeAllObjects();
}

void World2::addObject(Object2* pObject) {
	assert(pObject);
	mObjects.push_back(pObject);
	pObject->setWorld(this);
}
void World2::removeAllObjects() {
	for(ObjectList::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator) {
		Object2* object = *objectIterator;
		assert(object);
		delete object;
	}
	mObjects.clear();
}
void World2::render(float pTime) {
	setDisplay2d();
	for(ObjectList::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator) {
		Object2* object = *objectIterator;
		assert(object);
		glPushMatrix();
		object->render(pTime);
		glPopMatrix();
	}
}

void World2::step(float pTime) {
	ObjectList toRemove;
	for(ObjectList::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator) {
		Object2* object = *objectIterator;
		assert(object);
		if(! object->step(pTime) ) {
			toRemove.push_back(object);
		}
	}
	for(ObjectList::iterator objectIterator=toRemove.begin(); objectIterator!=toRemove.end(); ++objectIterator) {
		Object2* object = *objectIterator;
		assert(object);
		mObjects.remove(object);
		delete object;
	}
}