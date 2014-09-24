#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <string>
#include <list>

class ContainerPointer;

class Container {
public:
	Container(const std::string& pName);
	virtual ~Container();
	void handleOnChange();
	void bind(ContainerPointer* pContainerPointer);
	void unbind(ContainerPointer* pContainerPointer);
private:
	const std::string mName;
	typedef std::list<ContainerPointer*> ContainerPointerList;
	ContainerPointerList mContainerPointers;
};

#endif