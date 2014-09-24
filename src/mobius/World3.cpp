#include "World3.hpp"

#include "Object3.hpp"
#include "Camera3.hpp"
#include "Component3.hpp"
#include "OpenGL.hpp"
#include <cassert>
#include "LightSetup.hpp"

#include "MeshData.hpp"
#include "MeshDataManager.hpp"
#include "MeshFile.hpp"

// for level loading
#include <sstream>
#include "File.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

#include "PhysActor.hpp"

#include "StringUtils.hpp"
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>

World3::World3() : mCamera(0), mTimeMultiplier(1.0f) {
	initPhysics();
}

World3::~World3() {
	removeAllObjects();
	killPhysics();
}

void World3::getNames(std::vector<std::string>* oNames) const {
	assert(oNames);
	oNames->clear();
	for(StringObjectMap::const_iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator ) {
		oNames->push_back(objectIterator->first);
	}
}

void nearCallback(void* data, dGeomID o1, dGeomID o2) {
	const int MAX_CONTACTS = 40;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contacts[MAX_CONTACTS];
	// setup contacts
	for(size_t contactIndex=0;contactIndex<MAX_CONTACTS; ++contactIndex) {
		contacts[contactIndex].surface.mode = dContactBounce | dContactSoftCFM;
		contacts[contactIndex].surface.mu = dInfinity;
		contacts[contactIndex].surface.mu2 = 0;
		contacts[contactIndex].surface.bounce = 0.01f;
		contacts[contactIndex].surface.bounce_vel = 0.1f;
		contacts[contactIndex].surface.soft_cfm = 0.01f;
	}

	int numColliders = dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact) );
	if( numColliders ) {
		for(int collideIndex=0; collideIndex<numColliders; ++collideIndex) {
			/** @todo convert to an explicit_cast to be more readable*/
			World3* world = (World3*) (data);
			dJointID joint = dJointCreateContact(world->getCollisionWorld(), world->getContactGroup(), &(contacts[collideIndex]));
			dJointAttach(joint, b1, b2);
		}
	}
}
void World3::setTimeMultiplier(real pNewMultiplier) {
	mTimeMultiplier = pNewMultiplier;
}
dWorldID World3::getCollisionWorld() {
	return mWorld;
}
dJointGroupID World3::getContactGroup() {
	return mContactGroup;
}
dSpaceID World3::getSpace() {
	return mSpace;
}
void World3::update(bool pDoPhysics, float pTime) {
	updateOnlyDelete();

	// step ode
	if( pDoPhysics ) {
		dSpaceCollide(mSpace, this, &nearCallback);
		dWorldSetQuickStepNumIterations(mWorld, 50);
		dWorldQuickStep(mWorld, pTime * mTimeMultiplier);
		dJointGroupEmpty(mContactGroup);
	}

	// update stuff that needs to be updates
	for(StringObjectMap::reverse_iterator objectIterator=mObjects.rbegin(); objectIterator!=mObjects.rend(); ++objectIterator ) {
		Object3* object = objectIterator->second;
		assert(object);
		if( object->isDynamic() ) {
			object->update(pTime * mTimeMultiplier);
		}
	}
}

void World3::updateOnlyDelete() {
	// delete objects that need to be deleted
	for(StringObjectMap::iterator objectIterator=mObjectsToDelete.begin(); objectIterator!=mObjectsToDelete.end(); ++objectIterator ) {
		const std::string name = objectIterator->first;
		Object3* object = objectIterator->second;
		delete object;
	}
	mObjectsToDelete.clear(); // make sure the map is empty
}

/** @todo optimize me */
void World3::insertIntoContainer(ComponentList* pComponentContainer, const Object3* pObject) {
	for(Object3::ComponentList::const_iterator componentIterator = pObject->begin(); componentIterator != pObject->end(); ++componentIterator) {
		const Component3* component = *componentIterator;
		// this function design(and world3 for that matter) doesn't handle camera objects components that are transparent
		assert( !component->isTransparent() );
		pComponentContainer->push_back(component);
	}
}

void World3::render(float pTime) {
	checkOpengl();
	// preupdate and cull objects
	mStaticToCamera.clear();
	mDependingOnCameraPosition.clear();
	mCulledLights.clear();
	mCulledTransparentObjects.clear();
	mCulledNonTransparentObjects.clear();
	for(StringObjectMap::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator ) {
		Object3* object = objectIterator->second;
		assert(object);
		const bool cull = true; // temporary, const bool helps to docutment bool call
		bool prepareObject = false;

		// a object can be both light and visible(thought that may look ugly)

		// let's handle lights first
		if( object->isLight() ) {
			// light objects must exist in world in current implementation
			assert( object->getCameraRelation()==CR_INDEPENDENT_POSITION_AND_ROTATION );
			if( cull ) {
				mCulledLights.push_back(object);
				prepareObject = true;
			}
		}

		// and then visible object - we will also not render the current camera since that may(will) look ugly
		if( object->isVisible() && object!=mCamera ) {
			if( object->getCameraRelation()==CR_INDEPENDENT_POSITION_AND_ROTATION ) {
				if( cull ) {
					prepareObject = true;
					// loop through all components of this object and put transparent in one container and non-transparent in another
					for(Object3::ComponentList::const_iterator componentIterator = object->begin(); componentIterator != object->end(); ++componentIterator) {
						const Component3* component = *componentIterator;
						if( component->isTransparent() ) {
							mCulledTransparentObjects.push_back(component);
						}
						else {
							mCulledNonTransparentObjects.push_back(component);
						}
					}
				}
			}
			else {
				if( object->getCameraRelation()==CR_INDEPENDENT_ROTATION) {
					insertIntoContainer(&mDependingOnCameraPosition, object);

					prepareObject = true;
				}
				else {
					assert(object->getCameraRelation()==CR_DEPENDENT_ON_CAMERA); // check for implementation error
					insertIntoContainer(&mStaticToCamera, object);
					prepareObject = true;
				}
			}
		}

		if( prepareObject || object==mCamera ) {
			object->prepareForRendering(pTime);
		}
	}

	setDisplay3d();
	checkOpengl();
	renderList(mStaticToCamera, pTime, RM_NO_LIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	assert(mCamera); // assert if we have no valid camera
	glRotateInverse(mCamera->getRotationObject()); // rotate camera
	renderList(mDependingOnCameraPosition, pTime, RM_NO_LIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	checkOpengl();
	glTranslateInverse(mCamera->getPositionObject());// move camera
	checkOpengl();
	glEnable(GL_LIGHTING);
	{
		checkOpengl();
		LightSetup lightSetup(mLightModelData);;
		for(ObjectList::const_iterator objectIterator=mCulledLights.begin(); objectIterator!=mCulledLights.end(); ++objectIterator) {
			const Object3* object = *objectIterator;
			glPushMatrix();
			checkOpengl();
			object->setupLight(&lightSetup);
			glPopMatrix();
			checkOpengl();
		}
		//renderList(mCulledNonTransparentObjects, RM_DEPTH_BUFFER_ONLY);
		renderList(mCulledNonTransparentObjects, pTime, RM_COLOR_AND_DEPTH);
		// transparent objects can't occlude each other, but can be occlude by other objects
		// so we disable writing on the depth buffer
		glDepthMask(GL_FALSE);
		// render this list back to front to have perfect transperency, but for
		// the moment perfect transperency isn't needed so for the time beeing
		// let's ignore perfect transperency and focus on matters that need our attention
			renderList(mCulledTransparentObjects, pTime, RM_COLOR_BUFFER_ONLY);
		glDepthMask(GL_TRUE);
#ifdef _DEBUG
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);
			for(StringObjectMap::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator ) {
				Object3* object = objectIterator->second;
				assert(object);
				glPushMatrix();
				object->debugDraw();
				glPopMatrix();
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
#endif
	}
	glDisable(GL_LIGHTING);
}

void World3::renderList(ComponentList& pList, real pTime, RenderMode pRenderMode) const {
	for(ComponentList::const_iterator objectIterator=pList.begin(); objectIterator!=pList.end(); ++objectIterator) {
		const Component3* component = *objectIterator;
		glPushMatrix();
		component->beginMaterial(pRenderMode);
		component->render(*this, pTime, pRenderMode);
		component->endMaterial(pRenderMode);
		glPopMatrix();
	}
}

void World3::add(Object3* pObject) {
	assert(pObject);
	assert( !hasObject(pObject->getName()) );
	pObject->selectWorld(this);
	mObjects.insert( StringObjectPair(pObject->getName(), pObject) );
}

void World3::removeObject(const std::string& pName) {
	assert( !areAboutToRemove(pName) );
	Object3* object = getObjectPtr(pName);
	assert( object != mCamera );
	mObjects.erase( pName );
	mObjectsToDelete.insert( StringObjectPair(pName, object ) );
}

bool World3::areAboutToRemove(const std::string& pName) const {
	StringObjectMap::const_iterator result = mObjectsToDelete.find(pName);
	return result != mObjectsToDelete.end();
}

void World3::removeAllObjects() {
	for(StringObjectMap::iterator objectIterator=mObjects.begin(); objectIterator!=mObjects.end(); ++objectIterator ) {
		Object3* object = objectIterator->second;
		assert(object);
		delete object;
	}
	mCamera = 0; // we don't have any camera left
	mObjects.clear();
	updateOnlyDelete();
	killPhysics();
	initPhysics();
}

void World3::initPhysics() {
	mWorld = dWorldCreate();
	mSpace = dHashSpaceCreate(0);
	//mSpace = dSimpleSpaceCreate(0);
	mContactGroup = dJointGroupCreate(0);

	dWorldSetGravity(mWorld, 0, -3.0, 0);
	dWorldSetERP(mWorld, 0.2f);
	dWorldSetCFM(mWorld, 1e-5f);
	dWorldSetContactMaxCorrectingVel(mWorld, 0.9f);
	dWorldSetContactSurfaceLayer(mWorld, 0.001f);
	dWorldSetAutoDisableFlag(mWorld, 1);
}
void World3::killPhysics() {
	dJointGroupDestroy(mContactGroup);
	dSpaceDestroy(mSpace);
	dWorldDestroy(mWorld);
}

Object3& World3::getObject(const std::string& pName) {
	return *getObjectPtr(pName);
}
const Object3& World3::getObject(const std::string& pName) const {
	return *getObjectPtr(pName);
}

Object3* World3::getObjectPtr(const std::string& pName) {
	Object3* object = getObjectNoValidation(pName);
	if( !object ) {
		assert(object);
	}
	return object;
}
const Object3* World3::getObjectPtr(const std::string& pName) const {
	const Object3* object = getObjectNoValidation(pName);
	assert(object);
	return object;
}

Object3* World3::getObjectNoValidation(const std::string& pName) {
	StringObjectMap::iterator result = mObjects.find(pName);
	if( result == mObjects.end() ) return 0;
	return result->second;
}
const Object3* World3::getObjectNoValidation(const std::string& pName) const {
	StringObjectMap::const_iterator result = mObjects.find(pName);
	if( result == mObjects.end() ) return 0;
	return result->second;
}
bool World3::hasObject(const std::string& pName) const {
	const Object3* object = getObjectNoValidation(pName);
	if( object ) return true;
	else return false;
}

void World3::selectCamera(const std::string& pCameraName) {
	Object3* object = getObjectPtr(pCameraName);
	mCamera = dynamic_cast<Camera3*>(object);
	assert(mCamera);
}

class WorldObject : public Object3 {
public:
	WorldObject(const std::string& pName, const vec3& pPosition, const Quaternion& pRotation, const std::string& pFile, dSpaceID pSpace)
		: Object3(pName, false, true, false, CR_INDEPENDENT_POSITION_AND_ROTATION), mPosition(pPosition), mRotation(pRotation) {
		loadComponentsFromFile(pFile, pSpace);
	}

	void loadComponentsFromFile(const std::string& pFile, dSpaceID pSpace);

	const vec3& getPosition() const {
		return mPosition;
	}

	const Quaternion& getRotation() const {
		return mRotation;
	}
private:
	vec3 mPosition;
	Quaternion mRotation;
};

class WorldComponent : public Component3 {
public:
	WorldComponent(WorldObject* pObject, const MeshData* pData, dSpaceID pSpace) : Component3(pObject), mObject(pObject), mData(pData) {
		assert(pObject);
		assert(pData);
		setMaterial(pData->getMaterial());
		buildCollisionMesh(pSpace);
	}
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
		assert(mObject);
		glTranslate(mObject->getPosition());
		glRotate(mObject->getRotation());
		assert(mData);
		mData->render();
	}
	//void render(RenderMode pRenderMode) const {
	void buildCollisionMesh(dSpaceID pSpace) {
		// convert vertices for ode
		const size_t vertexCount = mData->getVertexCount();
		float (*vertices)[3] = new float[vertexCount][3];
		for( size_t vertexIndex=0; vertexIndex<vertexCount; ++vertexIndex) {
			const vec3 base = mData->getVertex(vertexIndex);
			const vec3 vertex = mObject->getPosition() +
				mObject->getRotation().rotateVectorAroundOrigin( base.getNormalized() )*base.getLength();
			vertices[vertexIndex][0] = vertex.getX();
			vertices[vertexIndex][1] = vertex.getY();
			vertices[vertexIndex][2] = vertex.getZ();
		}

		// convert faces for ode
		const size_t faceCount = mData->getFaceCount();
		int (*faces)[3] = new int[faceCount][3];
		for( size_t faceIndex=0; faceIndex<faceCount; ++faceIndex) {
			faces[faceIndex][0] = mData->getFace(faceIndex).vertex[0].vertex;
			faces[faceIndex][1] = mData->getFace(faceIndex).vertex[1].vertex;
			faces[faceIndex][2] = mData->getFace(faceIndex).vertex[2].vertex;
		}

		dTriMeshDataID data = dGeomTriMeshDataCreate();
		//dGeomTriMeshDataBuildSimple(data, (dReal*)&vertices[0], vertexCount, (int*)&faces[0], faceCount*3);
		// dGeomTriMeshDataBuildSimple assumes there is 4 reals per vertex, I use 3. So I have to use the specialized version
		// of BuildSimple() called BuildSingle(), however this needs to change to BuildDouble if I change the precision.
		/** @todo fixme if precision change to double */
		dGeomTriMeshDataBuildSingle(data,
			&vertices[0], 3 * sizeof(float), vertexCount,
			(int*)&faces[0], faceCount*3, 3 * sizeof(int));
		dGeomID TriMesh = dCreateTriMesh(pSpace, data, 0, 0, 0);
	}
	void debugRender() {
	}
private:
	WorldObject* mObject;
	const MeshData* mData;
};

class WorldPhysBox : public PhysActor {
public:
	WorldPhysBox(const std::string& name, const vec3& position, const Quaternion& rotation, const std::string& file, real weight, real x, real y, real z, dWorldID pWorld, dSpaceID pSpace)
		: PhysActor(name, position, rotation, pWorld, pSpace, true, true) {
		loadComponentsFromFile( file );
		buildBox(x, y, z, weight * 0.0025f);
	}

	void doUpdate(float pTime) {
	}
};

void WorldObject::loadComponentsFromFile(const std::string& pFileName, dSpaceID pSpace) {
	MeshFile* file = MeshDataManager::getInstance().getFile(pFileName);
	setFile(file);
	assert(file);
	for(MeshFile::MeshDataList::const_iterator meshIterator=file->begin(); meshIterator!=file->end(); ++meshIterator) {
		MeshData* data = *meshIterator;
		addComponent( new WorldComponent(this, data, pSpace) );
	}
}

class SkySphere : public Object3 {
public:
	SkySphere(const std::string& pName, const std::string& pFile) : Object3(pName, false, true, false, CR_INDEPENDENT_ROTATION) {
		loadComponentsFromFile(pFile);
	}
};



vec3 getPosition(TiXmlElement* pElement) {
	TiXmlElement* data = pElement->FirstChildElement("position");
	if( !data ) return vec3(0,0,0);
	float x=0, y=0, z=0, temp=0;
#define ATTRIBUTE(name) if( TIXML_SUCCESS==data->QueryFloatAttribute(#name, &temp) ) name=temp
	ATTRIBUTE(x);
	ATTRIBUTE(y);
	ATTRIBUTE(z);
	return vec3(x, y,z);
}
Quaternion getRotation(TiXmlElement* pElement) {
	TiXmlElement* data = pElement->FirstChildElement("rotation");
	if( !data ) return Quaternion();
	real x=0, y=0, z=0, w=1, temp=0;
#define ATTRIBUTE(name) if( TIXML_SUCCESS==data->QueryFloatAttribute(#name, &temp) ) name=temp
	ATTRIBUTE(x);
	ATTRIBUTE(y);
	ATTRIBUTE(z);
	ATTRIBUTE(w);
	return Quaternion(x, y,z, w);
}


void World3::load(TiXmlDocument& pDocument) {
	TiXmlHandle docHandle( &pDocument );
	for(TiXmlElement* entity = docHandle.FirstChild("level").FirstChild("entity").ToElement(); entity; entity = entity->NextSiblingElement("entity") ) {
		const char* namePointer = entity->Attribute("name");
		const std::string name = namePointer?namePointer:"";
		const char* typePointer = entity->Attribute("type");
		const std::string type = typePointer?typePointer:"";
		const vec3 position = getPosition(entity);
		const Quaternion rotation = getRotation(entity);

		if (name=="sky") {
			add( new SkySphere(name, type + ".obj") );
		}
		else {
			if( type.length() > 0 ) {
				if( type[0] == '_' ) {
					std::vector<std::string> data;
					splitString("_", type, &data);
					if( data.size() == 6 && data[1]=="box" ) {
						real weight = boost::lexical_cast<real>( data[0] );
						real x = boost::lexical_cast<real>( data[2] );
						real y = boost::lexical_cast<real>( data[3] );
						real z = boost::lexical_cast<real>( data[4] );
						const std::string file = data[5];
						add( new WorldPhysBox(name, position, rotation, file+".obj", weight, x, y, z, mWorld, mSpace) );
					}
				}
				else {
					add( new WorldObject(name, position, rotation, type + ".obj", mSpace) );
				}
			}
		}
	}
}


const Camera3& World3::getCamera() const {
	assert(mCamera);
	return *mCamera;
}
