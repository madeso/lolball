#ifndef ACTOR_COMPONENT_HPP
#define ACTOR_COMPONENT_HPP

#include "Component3.hpp"

class Actor;

class ActorComponent : public Component3 {
public:
	ActorComponent(Actor* pActor);
	virtual void onRender(const World3& pWorld, real pTime, RenderMode& pRenderMode) const = 0;
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const;
private:
	Actor* mActor;
};

#endif