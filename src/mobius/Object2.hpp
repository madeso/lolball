#ifndef OBJECT2D_HPP
#define OBJECT2D_HPP

class World2;

class Object2 {
public:
	Object2() : mWorld(0) {}
	virtual ~Object2() {}

	// return true to stay alaive
	virtual bool step(float pTime) { return true; }
	virtual void render(float pTime) = 0;

	void setWorld(World2* pWorld) { mWorld=pWorld; }
protected:
	World2* mWorld;
};

#endif