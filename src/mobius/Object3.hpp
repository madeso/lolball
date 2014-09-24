#ifndef OBJECT3_HPP
#define OBJECT3_HPP

#include <string>
#include <list>
class Component3;
class World3;

enum CameraRelation {
	CR_INDEPENDENT_POSITION_AND_ROTATION,
	CR_INDEPENDENT_ROTATION,
	CR_DEPENDENT_ON_CAMERA,
};

class LightSetup;

class MeshFile;

class Object3 {
public:
	Object3(const std::string& pName, bool pIsLight, bool pIsVisible, bool pIsDynamic, CameraRelation pCameraRelation);
	virtual ~Object3();

	// light objecs gives light - handled through setupLight()
	// dynamic objects can move - handled through update()
	// visible objects are visible - handled through setupRender() and render()
	// trasparent objects are rendered (if it is visible) rendered after everybody else

	bool isLight() const;
	bool isVisible() const;
	bool isDynamic() const;

	void hide();
	void show();

	virtual void freeze();
	virtual void unfreeze();

	void light_enable();
	void light_disable();

	virtual void update(float pTime) {}

	virtual void prepareForRendering(float pTime) {} // called when this object should prepare for rendering

	virtual void setupLight(LightSetup* pLightSetup) const {}

	const std::string& getName() const;

	const CameraRelation getCameraRelation() const;

	typedef std::list<Component3*> ComponentList;
	ComponentList::const_iterator begin() const;
	ComponentList::const_iterator end() const;

	virtual void debugDraw() const {};

	void setTransperency(float pNewTransperency);
	float getTransperency() const;

	void selectWorld(World3* pWorld);
	void clearWorld();
protected:
	virtual void loadComponentsFromFile(const std::string& pFileName);
	void addComponent(Component3* pNewComponent);
	void setFile(MeshFile* pMeshFile);
	World3& getWorld();
	void removeMeFromMyWorld();
private:
	const std::string mName;
	bool mIsLight;
	bool mIsVisible;
	bool mIsDynamic;
	CameraRelation mCameraRelation;
	ComponentList mComponents;
	MeshFile* mCurrentMeshFile;
	World3* mWorld;

	float mTransperency;
};

#endif