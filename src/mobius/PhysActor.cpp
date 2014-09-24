#include "PhysActor.hpp"

PhysActor::PhysActor(const std::string& pName, const vec3& pPosition, const Quaternion& pRotation, dWorldID pWorld, dSpaceID pSpace, bool pDoRotation, bool pAutoDisable)
	: Actor(pName, pPosition, pRotation, true), mWorld(pWorld), mSpace(pSpace), mDoRotation(pDoRotation) {
		mBody = dBodyCreate(mWorld);
		dBodySetPosition(mBody, pPosition.getX(), pPosition.getY(), pPosition.getZ());
		dQuaternion rot;
		rot[0] = pRotation.getW(); rot[1] = pRotation.getX();
		rot[2] = pRotation.getY(); rot[3] = pRotation.getZ();
		dBodySetQuaternion(mBody, rot);
		dBodySetData(mBody, this);
		const int autoDisable = (pAutoDisable)?1:0;
		dBodySetAutoDisableFlag(mBody, autoDisable);
}

void PhysActor::teleport(const vec3& pNewPosition, const Quaternion& pNewOrientation){
	Actor::teleport(pNewPosition, pNewOrientation);
	dBodySetPosition(mBody, pNewPosition.getX(), pNewPosition.getY(), pNewPosition.getZ());
	dBodySetForce (mBody, 0, 0, 0);
	dBodySetLinearVel(mBody, 0, 0, 0);
	dQuaternion rot;
	rot[0] = pNewOrientation.getW(); rot[1] = pNewOrientation.getX();
	rot[2] = pNewOrientation.getY(); rot[3] = pNewOrientation.getZ();
	dBodySetQuaternion(mBody, rot);
}
void PhysActor::move(const vec3& pNewPosition) {
	if( !math::equal(pNewPosition.getLength(), 0.0f) ) {
		dBodyAddForce(mBody, pNewPosition.getX(), pNewPosition.getY(), pNewPosition.getZ());
	}
}
void PhysActor::moveTo(const vec3& pNewPosition) {
	move(getPosition()-pNewPosition);
}
void PhysActor::update(float pTime) {
	dBodySetForce (mBody, 0, 0, 0);
	Actor::update(pTime);
	const dReal* pos = dBodyGetPosition(mBody);
	
	if( mDoRotation ) {
		const dReal* rot = dBodyGetQuaternion(mBody);
		Actor::rotateTo( Quaternion(rot[0], rot[1], rot[2], rot[0]) );
	}

	Actor::moveTo(vec3(pos[0], pos[1], pos[2]));
}

void PhysActor::buildSphere(float pRadius, float pDensity) {
	dMass mass;
	dMassSetSphere(&mass, pDensity, pRadius);
	mGeom = dCreateSphere(mSpace, pRadius);
	dGeomSetBody(mGeom, mBody);
	dBodySetMass(mBody, &mass);
}

void PhysActor::buildBox(float pSizeX, float pSizeY, float pSizeZ, float pDensity) {
	dMass mass;
	dMassSetBox(&mass, pDensity, pSizeX, pSizeY, pSizeZ);
	mGeom = dCreateBox(mSpace, pSizeX, pSizeY, pSizeZ);
	dGeomSetBody(mGeom, mBody);
	dBodySetMass(mBody, &mass);
}

vec3 PhysActor::getSpeed() const {
	const dReal* speed = dBodyGetLinearVel(mBody);
	return vec3(speed[0], speed[1], speed[2]);
}

void PhysActor::addImpulse(const vec3& pImpulse, real pTime) {
	real impulse[4];
	dWorldImpulseToForce (mWorld, pTime, pImpulse.getX(), pImpulse.getY(), pImpulse.getZ(), impulse);
	dBodyAddForce (mBody, impulse[0], impulse[1], impulse[2]);
}

struct RayCollisionData {
	bool collided;
	dGeomID ray;
	dGeomID ignoreMe;
	real minDepth;
	real distance;
};

void rayNearCallback (void *pData, dGeomID o1, dGeomID o2) {
	RayCollisionData* data = (RayCollisionData*)pData;
	const int MAX_CONTACTS = 40;
	if( o1!=data->ray && o2!=data->ray ) return; //must be the ray
	if( o1==data->ignoreMe || o2==data->ignoreMe ) return; // cannot be the player
	dContact contacts[MAX_CONTACTS];
	// setup contacts
	for(size_t contactIndex=0;contactIndex<MAX_CONTACTS; ++contactIndex) {
		contacts[contactIndex].surface.mode = dContactBounce | dContactSoftCFM;
		contacts[contactIndex].surface.mu = dInfinity;
		contacts[contactIndex].surface.mu2 = 0;
		contacts[contactIndex].surface.bounce = 0.01f;
		contacts[contactIndex].surface.bounce_vel = 0.1f;
		contacts[contactIndex].surface.soft_cfm = 0.01f;
		contacts[contactIndex].geom.depth = -1;
	}

	int numColliders = dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact) );
	if( numColliders ) {
		for(int collideIndex=0; collideIndex<numColliders; ++collideIndex) {
			if( contacts[collideIndex].geom.depth < data->minDepth &&
				contacts[collideIndex].geom.depth > 0.0f ) {
				data->collided = true;
			}
		}
	}
}

void rayNearCallbackDistance (void *pData, dGeomID o1, dGeomID o2) {
	RayCollisionData* data = (RayCollisionData*)pData;
	const int MAX_CONTACTS = 40;
	if( o1!=data->ray && o2!=data->ray ) return; //must be the ray
	if( o1==data->ignoreMe || o2==data->ignoreMe ) return; // cannot be the player
	dContact contacts[MAX_CONTACTS];
	// setup contacts
	for(size_t contactIndex=0;contactIndex<MAX_CONTACTS; ++contactIndex) {
		contacts[contactIndex].surface.mode = dContactBounce | dContactSoftCFM;
		contacts[contactIndex].surface.mu = dInfinity;
		contacts[contactIndex].surface.mu2 = 0;
		contacts[contactIndex].surface.bounce = 0.01f;
		contacts[contactIndex].surface.bounce_vel = 0.1f;
		contacts[contactIndex].surface.soft_cfm = 0.01f;
		contacts[contactIndex].geom.depth = -1;
	}

	int numColliders = dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact) );
	if( numColliders ) {
		for(int collideIndex=0; collideIndex<numColliders; ++collideIndex) {
			if( contacts[collideIndex].geom.depth > 0.0f ) {
				const real dist = contacts[collideIndex].geom.depth;
				if( dist < data->distance ) {
					data->distance = dist;
				}
			}
		}
	}
}

real PhysActor::distanceToWorld(const vec3& pDirection) {
	const real len = pDirection.getLength();
	dGeomID ray = dCreateRay (mSpace, len);
	const vec3 pos = getPosition();
	const vec3 dir = pDirection.getNormalized();
	dGeomRaySet (ray, pos.getX(), pos.getY(), pos.getZ(), dir.getX(), dir.getY(), dir.getZ());
	RayCollisionData data;
	data.ray = ray;
	data.collided = false;
	data.ignoreMe = mGeom; // ignore player geom ffs!
	data.distance = len;
	dSpaceCollide(mSpace, &data, &rayNearCallbackDistance);
	dSpaceRemove(mSpace, ray);
	return data.distance;
}

bool PhysActor::collision(const vec3& pDirection) {
	dGeomID ray = dCreateRay (mSpace, pDirection.getLength());
	const vec3 pos = getPosition();
	const vec3 dir = pDirection.getNormalized();
	dGeomRaySet (ray, pos.getX(), pos.getY(), pos.getZ(), dir.getX(), dir.getY(), dir.getZ());
	RayCollisionData data;
	data.ray = ray;
	data.collided = false;
	data.minDepth = pDirection.getLength();
	data.ignoreMe = mGeom; // ignore player geom ffs!
	dSpaceCollide(mSpace, &data, &rayNearCallback);
	dSpaceRemove(mSpace, ray);
	return data.collided;
}
void PhysActor::freeze() {
	Object3::freeze();
	dBodyDisable(mBody);
}
void PhysActor::unfreeze() {
	Object3::unfreeze();
	dBodyEnable(mBody);
}