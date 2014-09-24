#ifndef WEAK_LINK_SYSTEM_HPP
#define WEAK_LINK_SYSTEM_HPP

#include "System.hpp"
#include <string>

#include "ContainerManager.hpp"

class WeakLinkSystem : public System {
public:
	WeakLinkSystem();
private:
	ContainerManager containerMgr;
};

#endif