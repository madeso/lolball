#include "ContainerManager.hpp"

#include "Container.hpp"
#include <cassert>
#include <iostream>

ContainerManager::ContainerManager() {
	assert(!sInstance);
	sInstance = this;
}

ContainerManager::~ContainerManager() {
	assert(mContainers.empty());
	assert(sInstance);
	sInstance = 0;
}

ContainerManager* ContainerManager::getInstancePtr() {
	return sInstance;
}
ContainerManager& ContainerManager::getInstance() {
	assert(getInstancePtr());
	return *getInstancePtr();
}

void ContainerManager::registerContainer(const std::string& pName, Container* pContainer) {
	assert(pContainer);
	assert(!has(pName) );
	mContainers.insert(StringContainerPair(pName, pContainer) );
	std::cout << "container [" << pName << "] registered" << std::endl;
}

void ContainerManager::unregisterContainer(const std::string& pName) {
	assert( has(pName) );
	mContainers.erase(pName);
	std::cout << "container [" << pName << "] unregistered" << std::endl;
}

Container* ContainerManager::getContainer(const std::string& pName) {
	Container* container = findContainer(pName);
	assert(container);
	return container;
}

bool ContainerManager::has(const std::string& pName) const {
	StringContainerMap::const_iterator result = mContainers.find(pName);
	return result != mContainers.end();
}

Container* ContainerManager::findContainer(const std::string& pName) {
	StringContainerMap::iterator result = mContainers.find(pName);
	if( result == mContainers.end() ) {
		return 0;
	}
	else {
		return result->second;
	}
}

ContainerManager* ContainerManager::sInstance = 0;