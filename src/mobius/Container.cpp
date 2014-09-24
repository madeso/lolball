#include "Container.hpp"

#include <cassert>
#include "ContainerPointer.hpp"
#include "ContainerManager.hpp"

Container::Container(const std::string& pName) : mName(pName) {
	ContainerManager::getInstance().registerContainer(mName, this);
}

Container::~Container() {
	assert( mContainerPointers.empty() );
	ContainerManager::getInstance().unregisterContainer(mName);
}

void Container::handleOnChange() {
	for(ContainerPointerList::iterator containerPointerIterator=mContainerPointers.begin();
		containerPointerIterator != mContainerPointers.end();
		++containerPointerIterator) {
			ContainerPointer* containerPointer = *containerPointerIterator;
			containerPointer->onChange();
	}
}

void Container::bind(ContainerPointer* pContainerPointer) {
	assert(pContainerPointer);
	mContainerPointers.push_back(pContainerPointer);
}
void Container::unbind(ContainerPointer* pContainerPointer) {
	mContainerPointers.remove(pContainerPointer);
}