#ifndef CONTAINER_MANAGER_HPP
#define CONTAINER_MANAGER_HPP

#include <string>
#include <map>

class Container;

class ContainerManager {
public:
	ContainerManager();
	~ContainerManager();
	void registerContainer(const std::string& pName, Container* pContainer);
	void unregisterContainer(const std::string& pName);
	Container* getContainer(const std::string& pName);
	bool has(const std::string& pName) const;

	static ContainerManager* getInstancePtr();
	static ContainerManager& getInstance();
private:
	Container* findContainer(const std::string& pName);

	typedef std::pair<std::string, Container*> StringContainerPair;
	typedef std::map<std::string, Container*> StringContainerMap;
	StringContainerMap mContainers;

	static ContainerManager* sInstance;
};

#endif