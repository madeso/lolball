#ifndef CONTAINER_POINTER_HPP
#define CONTAINER_POINTER_HPP

#include <string>

class Container;

class ContainerPointer {
public:
	ContainerPointer(const std::string& pName);
	virtual ~ContainerPointer();
	virtual void onChange() = 0;
	Container* getOwner();
private:
	Container* mContainer;
};

#endif