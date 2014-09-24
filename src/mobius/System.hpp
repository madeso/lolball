#ifndef INITIALIZER_HPP
#define INITIALIZER_HPP

#include <string>
#include <list>

class System {
public:
	System(const std::string& pName);
	/*virtual*/ ~System();
private:
	const std::string mName;
};

/*class Initializer {
public:
	Initializer();
	~Initializer();

	void init(System* pSystem);
	void unitilizeEverything();
private:
	typedef std::list<System*> SystemList;
	SystemList mSystems;
};*/

#endif;