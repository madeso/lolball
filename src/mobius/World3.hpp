#ifndef WORLD3_HPP
#define WORLD3_HPP

class Object3;
enum RenderMode;
class Camera3;
class Component3;

#include <string>
#include <list>
#include <map>
#include <vector>

#include "LightSetup.hpp"
#include "vec3.hpp"
#include "Quaternion.hpp"

#include "ode/ode.h"
#include "tinyxml.h"

class World3 {
public:
	World3();
	~World3();

	void update(bool pDoPhysics, float pTime);
	void render(float pTime);
	void add(Object3* pObject);

	void updateOnlyDelete();

	Object3& getObject(const std::string& pName);
	const Object3& getObject(const std::string& pName) const;

	Object3* getObjectPtr(const std::string& pName);
	const Object3* getObjectPtr(const std::string& pName) const;

	Object3* getObjectNoValidation(const std::string& pName);
	const Object3* getObjectNoValidation(const std::string& pName) const;
	bool hasObject(const std::string& pName) const;

	bool areAboutToRemove(const std::string& pName) const;
	void removeObject(const std::string& pName);
	void removeAllObjects();
	void selectCamera(const std::string& pCameraName);
	void getNames(std::vector<std::string>* oNames) const;

	void load(TiXmlDocument& pDocument);

	dWorldID getCollisionWorld();
	dSpaceID getSpace();
	dJointGroupID getContactGroup();

	const Camera3& getCamera() const;

	void setTimeMultiplier(real pNewMultiplier);
private:
	typedef std::list<const Component3*> ComponentList;
	typedef std::list<Object3*> ObjectList;
	typedef std::map<std::string, Object3*> StringObjectMap;
	typedef std::pair<std::string, Object3*> StringObjectPair;

	StringObjectMap mObjectsToDelete;

	StringObjectMap mObjects;

	static void insertIntoContainer(ComponentList* pComponentContainer, const Object3* pObject);
	void renderList(ComponentList& pList, real pTime, RenderMode pRenderMode) const;

	ComponentList mStaticToCamera;
	ComponentList mDependingOnCameraPosition;
	ObjectList mCulledLights;
	ComponentList mCulledNonTransparentObjects;
	ComponentList mCulledTransparentObjects;
	Camera3* mCamera;

	LightModelData mLightModelData;
	int mEditorMode;

	dWorldID mWorld;
	dSpaceID mSpace;
	dJointGroupID mContactGroup;

	void initPhysics();
	void killPhysics();

	real mTimeMultiplier;
};

vec3 getPosition(TiXmlElement* pElement);
Quaternion getRotation(TiXmlElement* pElement);

#endif