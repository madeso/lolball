#include "ContainerPointer.hpp"

#include <cassert>
#include "Container.hpp"
#include "ContainerManager.hpp"

ContainerPointer::ContainerPointer(const std::string& pName) {
	mContainer = ContainerManager::getInstance().getContainer(pName);
	assert(mContainer);
	mContainer->bind(this);
}

ContainerPointer::~ContainerPointer() {
	assert(mContainer);
	mContainer->unbind(this);
}

Container* ContainerPointer::getOwner() {
	return mContainer;
}