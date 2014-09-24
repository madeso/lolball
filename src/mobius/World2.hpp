#ifndef WORLD2D_HPP
#define WORLD2D_HPP

class Object2;

#include <list>

class World2 {
public:
	World2();
	~World2();

	void addObject(Object2* pObject);
	void render(float pTime);
	void step(float pTime);

	void removeAllObjects();
private:
	typedef std::list<Object2*> ObjectList;
	ObjectList mObjects;
};

#endif