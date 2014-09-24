#include "System.hpp"
#include <cassert>
#include <iostream>

System::System(const std::string& pName) : mName(pName) {
	std::cout << pName << "-system: initializing" << std::endl;
}
System::~System() {
	std::cout << mName << "-system: uninitialized" << std::endl;
}

/*Initializer::Initializer() {
}
Initializer::~Initializer() {
	unitilizeEverything();
}

void Initializer::init(System* pSystem) {
	mSystems.push_front(pSystem);
}
void Initializer::unitilizeEverything() {
	for(SystemList::iterator systemIterator=mSystems.begin(); systemIterator!=mSystems.end(); ++systemIterator) {
		System* system = *systemIterator;
		assert(system);
		delete system;
	}
	mSystems.clear();
}*/