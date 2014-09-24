#include "GameState.hpp"
#include "FontManager.hpp"
#include "Font.hpp"
#include <cassert>

#include "DataPointer.hpp"

#include "Object2.hpp"
#include "ActorComponent.hpp"
#include "LightSetup.hpp"
#include "PointLightData.hpp"
#include "DirectionalLightData.hpp"
#include "SpotLightData.hpp"
#include "Actor.hpp"
#include "Camera3.hpp"

#include "OpenGL.hpp"

#include "Key.hpp"
#include "math.hpp"

#include "PhysActor.hpp"
#include "ParticleComponent3.hpp"

#include "AbstractMaterial.hpp"

#include "Plane.hpp"
#include "StateStack.hpp"

// for setprecision
#include <iomanip>
#include "vec2.hpp"
#include "File.hpp"

#include "Axis.hpp"

#include "Exceptions.hpp"

#ifdef HEART_SFX
// youck youck, but since this game has a bad sound design anyway, this'll have to do :)
#include "FmodSystem.hpp"
#endif

#include <algorithm>
#include <functional>

struct MyParticle : public ParticleBase<MyParticle> {
	MyParticle() : velocity(0,0,0) {
	}
	vec3 velocity;
	real age;
	real terminalAge;
	real maxSize;
};

class MyParticleSystem : public ParticleComponent3<MyParticleSystem, MyParticle, PointEmitter<MyParticle> > {
public:
	MyParticleSystem(Actor* pOwner) : ParticleComponent3(800, PointEmitter<MyParticle>(pOwner, mRandom), pOwner), mTimeToLetParticlesGo(0.0f) {
		mRandom.randomize();
		AbstractMaterial material;
		material.texture = "ptexture.png";
		material.alpha = 0.1f;
		setMaterial(material);
	}
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
		renderParticlesAsBillboards(pWorld, pTime);
	}
	void step(real pTime) {
		updateParticles(pTime);
		mTimeToLetParticlesGo -= pTime;
		int particlesToSpawn = 0;
		while( mTimeToLetParticlesGo < 0.0f ) {
			mTimeToLetParticlesGo += 0.01f;
			++particlesToSpawn;
		}
		addParticles(particlesToSpawn, FCA_TAKE_OLDEST);
	}
	void setupParticle(MyParticle* pParticle) {
		assert(pParticle);
		pParticle->terminalAge = mRandom.randomFloat()*2.0f + 2.5f;
		pParticle->age = 0;
		pParticle->futureColor = 
			pParticle->color = vec3(mRandom.randomFloat(), 0,0);//vec3(mRandom.randomFloat(), mRandom.randomFloat(), mRandom.randomFloat());
		pParticle->maxSize = mRandom.randomFloat() * 0.4f + 0.2f;
		pParticle->velocity = vec3(mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign()) * 0.05f;
		pParticle->futureSize =
			pParticle->size = 0;
		pParticle->futureAlpha = 
			pParticle->alpha = 0.0f;
	}
	bool updateParticle(MyParticle* pParticle, real pTime) const {
		const vec3 fromColor(1, 1, 1);
		const vec3 toColor(0, 0, 1);
		bool alive = false;
		pParticle->age += pTime;
		if( pParticle->age < pParticle->terminalAge ) {
			const real lifeInPercent = pParticle->age / pParticle->terminalAge;
			pParticle->alpha = pParticle->futureAlpha;
			pParticle->futureAlpha = 1 - lifeInPercent;
			pParticle->position = pParticle->futurePosition;
			pParticle->futurePosition += pParticle->velocity*pTime;
			pParticle->color = pParticle->futureColor;
			pParticle->futureColor = op::vec3::getLinearInterpolation(fromColor,
				math::limitRange(0.0f, lifeInPercent*2.5f, 1.0f), toColor);
			pParticle->size = pParticle->futureSize;
			pParticle->futureSize = math::max(pParticle->maxSize*lifeInPercent, 0.10f);
			alive = true;
		}
		return alive;
	}
private:
	real mTimeToLetParticlesGo;
	Random mRandom;
};

class GoalParticleSystem : public ParticleComponent3<GoalParticleSystem, MyParticle, PointEmitter<MyParticle> > {
public:
	GoalParticleSystem(Actor* pOwner) : ParticleComponent3(100, PointEmitter<MyParticle>(pOwner, mRandom), pOwner), mTimeToLetParticlesGo(0.0f) {
		mRandom.randomize();
		AbstractMaterial material;
		material.texture = "ptexture.png";
		material.alpha = 0.1f;
		setMaterial(material);
	}
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
		renderParticlesAsBillboards(pWorld, pTime);
	}
	void step(real pTime) {
		updateParticles(pTime);
		mTimeToLetParticlesGo -= pTime;
		int particlesToSpawn = 0;
		while( mTimeToLetParticlesGo < 0.0f ) {
			mTimeToLetParticlesGo += 0.04f;
			++particlesToSpawn;
		}
		addParticles(particlesToSpawn, FCA_IGNORE);
	}
	void setupParticle(MyParticle* pParticle) {
		assert(pParticle);
		pParticle->terminalAge = mRandom.randomFloat()*0.4f + 0.6f;
		pParticle->age = 0;
		pParticle->futureColor = 
			pParticle->color = vec3(mRandom.randomFloat(), 0,0);
		pParticle->maxSize = mRandom.randomFloat() * 0.3f + 0.3f;
		pParticle->velocity = vec3(mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign()) * 0.7f;
		pParticle->futureSize =
			pParticle->size = 0;
		pParticle->futureAlpha = 
			pParticle->alpha = 0.0f;
	}
	bool updateParticle(MyParticle* pParticle, real pTime) const {
		const vec3 fromColor(1, 1, 1);
		const vec3 toColor(1, 0, 0);
		bool alive = false;
		pParticle->age += pTime;
		if( pParticle->age < pParticle->terminalAge ) {
			const real lifeInPercent = pParticle->age / pParticle->terminalAge;
			pParticle->alpha = pParticle->futureAlpha;
			pParticle->futureAlpha = 1 - lifeInPercent;
			pParticle->position = pParticle->futurePosition;
			pParticle->futurePosition += pParticle->velocity*pTime;
			pParticle->color = pParticle->futureColor;
			pParticle->futureColor = op::vec3::getLinearInterpolation(fromColor,
				math::limitRange(0.0f, lifeInPercent/*1.2f-0.1f*/, 1.0f), toColor);
			pParticle->size = pParticle->futureSize;
			pParticle->futureSize = math::max(pParticle->maxSize*lifeInPercent, 0.10f);
			alive = true;
		}
		return alive;
	}
private:
	real mTimeToLetParticlesGo;
	Random mRandom;
};

class TextInfo
	: public Object2
{
public:
	TextInfo(const std::string& pText, const vec2& pLocation, real pLife, const vec2 pMovement)
		: Object2()
		, mFont(FontManager::getInstance().getStandardFont()), mText(pText)
		, mLocation(pLocation)
		, mLife(pLife)
		, mMaxLife(pLife)
		, mMovement(pMovement)
		, mOldLocation(pLocation)
	{
	}

	bool step(real pTime)
	{
		mLife -= pTime;
		mOldLocation = mLocation;
		mLocation += mMovement*pTime;
		if( mLife > 0 ) return true;
		else return false;
	}

	void render(real pTime)
	{
		const vec2 location = mOldLocation + (mLocation-mOldLocation)*pTime;
		FPRINT(mFont, location.getX(), location.getY(),
			FontColor(vec3(1,1,1), mLife/mMaxLife) << mText);
	}
private:
	Font& mFont;
	const std::string mText;
	vec2 mLocation;
	real mLife;
	const real mMaxLife;
	const vec2 mMovement;
	vec2 mOldLocation;
};

class PressAnyKeyObject : public Object2 {
public:
	PressAnyKeyObject(const std::string& pInfo) : Object2(), mFont(FontManager::getInstance().getStandardFont()), mInfo(pInfo) {
	}

	bool step(real pTime) {
		return true;
	}
	void render(real pTime) {
		FPRINT(mFont, 0.3f, 0.95f,
			"Press [ENTER] to " << mInfo);
	}
private:
	Font& mFont;
	const std::string mInfo;
};


const real MOVEMENT_FORCE = 0.5f;
const real MOVEMENT_FORCE_BOOSTED = 1.7f;



struct GameState::GameStatePimpl {
	GameStatePimpl() : loosePoints(true), rotateCamera(false), loadedWorld(false) {
		points = 1000;
		assert(!sInstance);
		sInstance = this;
	}
	~GameStatePimpl() {
		assert(sInstance);
		sInstance = 0;
	}
	unsigned long points;
	bool loosePoints;
	bool rotateCamera;
	std::string currentLevel;
	std::string nextLevel;
	std::string levelName;
	bool loadedWorld;
	void step(real pTime) {
		if( loosePoints ) {
			if( points > 0 ) {
				points -= 1;
			}
		}
	}

	static GameStatePimpl* getInstancePtr() {
		assert(sInstance);
		return sInstance;
	}
	static GameStatePimpl& getInstance() {
		return *getInstancePtr();
	}

	static GameStatePimpl* sInstance;
};

class HiScoreObject : public Object2{
public:
	HiScoreObject() : Object2(), mFont(FontManager::getInstance().getStandardFont()), mYouScore(-1), levelName(GameState::GameStatePimpl::getInstance().levelName) {
		const std::string FILE_NAME = GameState::GameStatePimpl::getInstance().currentLevel + "_hi.score";

		// get the current score and the current high score
		mYouScore = GameState::GameStatePimpl::getInstance().points;
		mScores.push_back(mYouScore);
		try {
			std::istringstream scoreFile;
			loadFileIntoString(FILE_NAME, &scoreFile);
			for(int i=0; i<5; ++i) {
				unsigned long highScore = 0;
				scoreFile >> highScore;
				mScores.push_back(highScore);
			}
		}
		catch(MissingResourceException&) {
			// if file doesn't exist, this must be the first time we run, and we ignore this, since we save it later
		}

		std::sort(mScores.begin(), mScores.end(), std::greater<unsigned long>());

		mScores.resize(5);

		/* save to file */ 
		std::ostringstream fileData;
		for( std::vector<unsigned long>::const_iterator score=mScores.begin(); score != mScores.end(); ++score) {
			fileData << *score << " ";
		}
		saveStringIntoFile(FILE_NAME, fileData.str());
	}

	bool step(real pTime) {
		return true;
	}

	void render(real pTime) {
		real y = 0.2f;
		int index = 0;
		bool hasFoundScore = false;
		FPRINT(mFont, 0.25f, 0.1f, levelName );
		for( std::vector<unsigned long>::const_iterator score=mScores.begin(); score != mScores.end(); ++score) {
			if( !hasFoundScore && *score == mYouScore ) {
				FPRINT(mFont, 0.45f, y, FontColor(vec3(1, 0, 0), 1) << index+1 << ". " << *score << " (you)");
				hasFoundScore = true;
			}
			else {
				FPRINT(mFont, 0.45f, y, index+1 << ". " << *score);
			}
			y+= 0.05f;
			++index;
		}
	}
private:
	Font& mFont;
	std::vector<unsigned long> mScores;
	unsigned long mYouScore;
	const std::string levelName;
};

class GameCompletedObject : public Object2{
public:
	GameCompletedObject() : Object2(), mFont(FontManager::getInstance().getStandardFont()) {
	}

	bool step(real pTime) {
		return true;
	}

	void render(real pTime) {
		FPRINT(mFont, 0.3f, 0.45f,
			FontColor(vec3(1,1,1), 1) << "^^ Game Completed ^^");
	}
private:
	Font& mFont;
};

GameState::GameStatePimpl* GameState::GameStatePimpl::sInstance = 0;

struct ComboTime {
public:
	ComboTime() : bomb(0) {
	}
	void step(real pTime) {
		step(pTime, &bomb);
	}
	void step(real pTime, real* pValue) {
		*pValue -= pTime;
		if( *pValue < 0.0f ) {
			*pValue = 0;
		}
	}

	void increase(real pValue, real* pObject) {
		const real scale = 0.1f;
#define UPDATE(object) if(&object==pObject) {object+=pValue;} else { object -= pValue*scale; if(object < 0.0f)object=0; }
		UPDATE(bomb);
#undef UPDATE
	}

	real bomb;
};

class BombParticleSystem : public ParticleComponent3<BombParticleSystem, MyParticle, PointEmitter<MyParticle> > {
public:
	BombParticleSystem(Actor* pOwner) : ParticleComponent3(200, PointEmitter<MyParticle>(pOwner, mRandom), pOwner), mTimeToLetParticlesGo(0.0f) {
		mRandom.randomize();
		AbstractMaterial material;
		material.texture = "ptexture.png";
		material.alpha = 0.1f;
		setMaterial(material);
		addParticles(200, FCA_IGNORE);
	}
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
		renderParticlesAsBillboards(pWorld, pTime);
	}
	bool step(real pTime) {
		bool shouldKill = !updateParticles(pTime);
		return shouldKill;
	}
	void setupParticle(MyParticle* pParticle) {
		assert(pParticle);
		pParticle->terminalAge = mRandom.randomFloat()*1.5f + 2.5f;
		pParticle->age = 0;
		pParticle->futureColor = 
			pParticle->color = vec3(mRandom.randomFloat(), 0,0);
		pParticle->maxSize = mRandom.randomFloat() * 0.3f + 0.3f;
		pParticle->velocity = vec3(mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign(), mRandom.randomFloatWithSign()) * 2.0f;
		pParticle->futureSize =
			pParticle->size = 0;
		pParticle->futureAlpha = 
			pParticle->alpha = 0.0f;
	}
	bool updateParticle(MyParticle* pParticle, real pTime) const {
		const vec3 fromColor(1, 1, 0);
		const vec3 toColor(1, 0, 0);
		bool alive = false;
		pParticle->age += pTime;
		if( pParticle->age < pParticle->terminalAge ) {
			const real lifeInPercent = pParticle->age / pParticle->terminalAge;
			pParticle->alpha = pParticle->futureAlpha;
			pParticle->futureAlpha = 1 - lifeInPercent;
			pParticle->position = pParticle->futurePosition;
			pParticle->futurePosition += pParticle->velocity*pTime;
			pParticle->velocity *= 1.0f - 1.0f*pTime;
			pParticle->color = pParticle->futureColor;
			pParticle->futureColor = op::vec3::getLinearInterpolation(fromColor,
				math::limitRange(0.0f, lifeInPercent/*1.2f-0.1f*/, 1.0f), toColor);
			pParticle->size = pParticle->futureSize;
			pParticle->futureSize = math::max(pParticle->maxSize*lifeInPercent, 0.10f);
			alive = true;
		}
		return alive;
	}
private:
	real mTimeToLetParticlesGo;
	Random mRandom;
};

class BombOnGroundParticleSystem : public ParticleComponent3<BombOnGroundParticleSystem, MyParticle, PointEmitter<MyParticle> > {
public:
	BombOnGroundParticleSystem(Actor* pOwner) : ParticleComponent3(200, PointEmitter<MyParticle>(pOwner, mRandom), pOwner), mTimeToLetParticlesGo(0.0f) {
		mRandom.randomize();
		AbstractMaterial material;
		material.texture = "ptexture.png";
		material.alpha = 0.1f;
		setMaterial(material);
		addParticles(200, FCA_IGNORE);
	}
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
		renderParticlesAsBillboards(pWorld, pTime);
	}
	bool step(real pTime) {
		bool done = ! updateParticles(pTime);

		return done;
	}
	void setupParticle(MyParticle* pParticle) {
		assert(pParticle);
		pParticle->terminalAge = mRandom.randomFloat()*1.0f + 1.0f;
		pParticle->age = 0;
		pParticle->futureColor = 
			pParticle->color = vec3(mRandom.randomFloat(), 0,0);
		pParticle->maxSize = mRandom.randomFloat() * 0.6f + 0.3f;
		const real xzMulitplier = 0.5f;
		pParticle->velocity = vec3(mRandom.randomFloatWithSign()*xzMulitplier, mRandom.randomFloat()*3.5f + 1.5f, mRandom.randomFloatWithSign()*xzMulitplier);
		pParticle->futureSize =
			pParticle->size = 0;
		pParticle->futureAlpha = 
			pParticle->alpha = 0.0f;
	}
	bool updateParticle(MyParticle* pParticle, real pTime) const {
		const vec3 fromColor(1, 1, 0);
		const vec3 toColor(1, 0, 0);
		bool alive = false;
		pParticle->age += pTime;
		if( pParticle->age < pParticle->terminalAge ) {
			const real lifeInPercent = pParticle->age / pParticle->terminalAge;
			pParticle->alpha = pParticle->futureAlpha;
			pParticle->futureAlpha = 1 - lifeInPercent;
			pParticle->position = pParticle->futurePosition;
			pParticle->futurePosition += pParticle->velocity*pTime;
			pParticle->velocity *= 1.0f - 1.0f*pTime;
			pParticle->color = pParticle->futureColor;
			pParticle->futureColor = op::vec3::getLinearInterpolation(fromColor,
				math::limitRange(0.0f, lifeInPercent/*1.2f-0.1f*/, 1.0f), toColor);
			pParticle->size = pParticle->futureSize;
			pParticle->futureSize = math::max(pParticle->maxSize*lifeInPercent, 0.10f);
			alive = true;
		}
		return alive;
	}
private:
	real mTimeToLetParticlesGo;
	Random mRandom;
};

class BombCombo : public Actor {
public:
	BombCombo(const vec3& pLocation) : Actor("bomb", pLocation, Quaternion(), true) {
		mParticleSystem = new BombParticleSystem(this);
		addComponent( mParticleSystem );
	}
	void doUpdate(float pTime) {
		assert(mParticleSystem);
		bool done = mParticleSystem->step(pTime);
		if( done ) {
			removeMeFromMyWorld();
		}
	}
private:
	BombParticleSystem* mParticleSystem;
};

class BombOnGroundCombo : public Actor {
public:
	BombOnGroundCombo(const vec3& pLocation) : Actor("bomb_on_ground", pLocation, Quaternion(), true) {
		mParticleSystem = new BombOnGroundParticleSystem(this);
		addComponent( mParticleSystem );
	}
	void doUpdate(float pTime) {
		assert(mParticleSystem);
		bool done = mParticleSystem->step(pTime);
		if( done ) {
			removeMeFromMyWorld();
		}
	}
private:
	BombOnGroundParticleSystem* mParticleSystem;
};

class Player : public PhysActor {
public:
	Player(const vec3& pStart, const Quaternion& pRot, World3& pWorld) : PhysActor("player", pStart, pRot, pWorld.getCollisionWorld(), pWorld.getSpace(), false, false),
				 mInOut("inout"),
				 mRightLeft("rightleft"), mHasRotated(false), mSpeed(0.0f),
				 mStartPosition(pStart), mStartRotation(pRot), mParticleSystem(0), mBoost(false) {
		//loadComponentsFromFile("cube.obj");
	    assert(!sInstance);
		sInstance = this;
	    mParticleSystem = new MyParticleSystem(this);
		addComponent(mParticleSystem);
		light_enable();
		data.attenuation.quadratic = 0.2f;
		buildSphere(	0.2f, // radius
						7.0f); // density
		resetRotation(); // setup rotation based on controls
	}

	void setBoost(bool pNewBoost) {
		mBoost = pNewBoost;
#ifdef HEART_SFX
		FmodSystem::getInstance().playHeart(pNewBoost);
#endif
	}

	void jump(real pTime) {
		if( ! GameState::GameStatePimpl::getInstance().loosePoints ) return;
		if( mBoost ) {
			mCombo.increase(0.2f, &mCombo.bomb);
			if( mCombo.bomb > 1.0f ) {
				mCombo.bomb = 0;
				if( collision(op::vec3::yAxisNegative * 0.25f) ) {
					if( ! getWorld().hasObject("bomb_on_ground") ) {
						getWorld().add( new BombOnGroundCombo(getPosition()) );
						GameState::GameStatePimpl::getInstance().points += 150;
					}
				}
				else {
					if( ! getWorld().hasObject("bomb") ) {
						getWorld().add( new BombCombo(getPosition()) );
						GameState::GameStatePimpl::getInstance().points += 200;
					}
				}
			}
		}
		else {
			if( collision(op::vec3::yAxisNegative * 0.30f) ) {
				move(op::vec3::yAxisPositive * 15.0f);
			}
		}
	}

	~Player() {
		assert(sInstance);
		sInstance = 0;
	}

	static Player* getInstancePtr() {
		assert(sInstance);
		return sInstance;
	}
	static Player& getInstance() {
		return *getInstancePtr();
	}

	void doLight(LightSetup* pLightSetup) const {
		pLightSetup->handle(data);
	}

	void resetRotation() {
		rotateTo(mStartRotation);
	}

	void doUpdate(float pTime) {
		mCombo.step(pTime*2);
		doUpdateAxis(pTime);
		assert(mParticleSystem);
		mParticleSystem->step(pTime);
	}

	void doUpdateAxis(float pTime) {
		const real dx = mRightLeft.get();
		const real dy = mInOut.get();
		/*
		if( in.get() ) {
			dy += 1.0f;
		}
		if( out.get() ) {
			dy -= 1.0f;
		}
		if( left.get() ) {
			dx -= 1.0f;
		}
		if( right.get() ) {
			dx += 1.0f;
		}*/
		
		vec3 movement = op::vec3::origo;
		movement += getRotation().getIn()*dy;
		movement += getRotation().getRight()*dx;

		const vec3 direction = movement.getNormalized();
		/*if( movement.getLength() > 0.0f ) {
			lookAt(getPosition()+direction, op::vec3::yAxisPositive);
		}*/
		if( mBoost ) {
			move(direction * MOVEMENT_FORCE_BOOSTED);
		}
		else {
			move(direction * MOVEMENT_FORCE);
		}
	}

	
	/*void doUpdateDoubleAction(float pTime) {
		const float min = 0.13f;
		real speed = MOVEMENT_FORCE;
		if( mBoost ) speed = MOVEMENT_FORCE_BOOSTED;

		float dx = 0.0f;
		float dy = 0.0f;

		if( in.get() ) {
			dy += 1.0f;
		}

		if( out.get() ) {
			mOutDownTime += pTime;
			if( mOutDownTime > min ) {
				dy -= 1.0f;
			}
		}
		else {
			if( mOutDownTime > 0.0f && mOutDownTime < min ) {
				increaseRotation( Quaternion(op::vec3::yAxisPositive, math::PI) );
			}
			mOutDownTime = 0.0f;
		}

		enum {
			LRM_NONE,
			LRM_LEFT,
			LRM_RIGHT
		} leftRightMovement = LRM_NONE;

		if( left.get() ) {
			mLeftDownTime += pTime;
			if( mLeftDownTime > min ) {
				dx -= 1.0f;
				leftRightMovement = LRM_LEFT;
			}
		}
		else {
			if( mLeftDownTime > 0.0f && mLeftDownTime < min ) {
				increaseRotation( Quaternion(op::vec3::yAxisPositive, math::PI/2) );
			}
			mLeftDownTime = 0.0f;
		}

		if( right.get() ) {
			mRightDownTime += pTime;
			if( mRightDownTime > min ) {
				dx += 1.0f;
				leftRightMovement = LRM_RIGHT;
			}
		}
		else {
			if( mRightDownTime > 0.0f && mRightDownTime < min ) {
				increaseRotation( Quaternion(op::vec3::yAxisPositive, -math::PI/2) );
			}
			mRightDownTime = 0.0f;
		}

		const vec3 movement = (getRotation().getIn()*dy + getRotation().getRight()*dx).getNormalized();

		move( movement * speed );

		Quaternion camRotMod;
		if( in.get() ) {
			if( leftRightMovement == LRM_LEFT ) {
				camRotMod = Quaternion(op::vec3::yAxisPositive, math::PI/4);
			}
			else if( leftRightMovement==LRM_RIGHT) {
				camRotMod = Quaternion(op::vec3::yAxisPositive, -math::PI/4);
			}
		}
	}

	void doUpdateRotate(float pTime) {
		real speed = MOVEMENT_FORCE;

		if( mBoost ) {
			speed = MOVEMENT_FORCE_BOOSTED;
		}

		if( in.get() ) {
		}
		else if( out.get() ) {
			speed *= -1;
		}
		else {
			speed = 0;
		}

		const real rot = math::PI * pTime * 0.5f;
		if( left.get() ) {
			increaseRotation( Quaternion(op::vec3::yAxisPositive, rot) );
		}
		else {
			mLeftDownTime = 0.0f;
		}
		if( right.get() ) {
			increaseRotation( Quaternion(op::vec3::yAxisPositive, -rot) );
		}
		else {
			mRightDownTime = 0.0f;
		}

		move(getRotation().getIn() * speed);
	}


	typedef DataPointer<bool> Key;
	void doUpdateKeepRotation(float pTime) {
		Key* currentInKey = 0;
		Key* currentLeftKey = 0;
		Key* currentRightKey = 0;
		switch( mMovementDirection ) {
			case MD_IN:
				subUpdate(pTime,
							in, out, left, right,
							MD_IN, MD_OUT, MD_LEFT, MD_RIGHT);
				currentInKey = &in;
				currentLeftKey = &left;
				currentRightKey = &right;
				break;
			case MD_OUT:
				subUpdate(pTime,
							out, in, left, right,
							MD_OUT, MD_IN, MD_LEFT, MD_RIGHT);
				currentInKey = &out;
				currentLeftKey = &left;
				currentRightKey = &right;
				break;
			case MD_LEFT:
				subUpdate(pTime,
							left, right, out, in,
							MD_LEFT, MD_RIGHT, MD_OUT, MD_IN);
				currentInKey = &left;
				currentLeftKey = &out;
				currentRightKey = &in;
				break;
			case MD_RIGHT:
				subUpdate(pTime,
							right, left, in, out,
							MD_RIGHT, MD_LEFT, MD_IN, MD_OUT);
				currentInKey = &right;
				currentLeftKey = &in;
				currentRightKey = &out;
				break;
		}
		if( mMovementDirection!=MD_IN && !in.get() && !out.get() && !left.get() && !right.get() ) {
			mMovementDirection = MD_IN;
		}
		Quaternion camRotMod;
		if( currentInKey->get() ) {
			if( currentLeftKey->get() ) {
				camRotMod = Quaternion(op::vec3::yAxisPositive, math::PI/4);
			}
			else if( currentRightKey->get() ) {
				camRotMod = Quaternion(op::vec3::yAxisPositive, -math::PI/4);
			}
		}
	}

	enum MovDir {
		MD_IN,
		MD_OUT,
		MD_RIGHT,
		MD_LEFT
	};

	void subUpdate(real pTime, const Key& pInKey, const Key& pOutKey, const Key& pLeftKey, const Key& pRightKey, MovDir pInDir, MovDir pOutDir, MovDir pLeftDir, MovDir pRightDir) {
		const real MIN_SPEED = 1.0f;
		const real MAX_SPEED = 8.0f;
		float dx = 0.0f;
		float dy = 0.0f;

		bool rotate = true;

		MovDir newMoveDir = pInDir;
		if( pInKey.get() ) {
			dy += 1.0f;
			newMoveDir = pInDir;
			rotate = false;
		}
		if( pOutKey.get() ) {
			dy -= 1.0f;
			newMoveDir = pOutDir;
		}
		if( pLeftKey.get() ) {
			dx -= 1.0f;
			newMoveDir = pLeftDir;
		}
		if( pRightKey.get() ) {
			dx += 1.0f;
			newMoveDir = pRightDir;
		}
		const vec3 movement = (getRotation().getIn() * dy + getRotation().getRight()*dx).getNormalized();
		move( movement * MOVEMENT_FORCE );

		if( rotate && !movement.isZero() ) {
			lookAt(getPosition() + movement*10, op::vec3::yAxisPositive);
			mMovementDirection = newMoveDir;
		}
	}
*/
	DataPointer<real> mInOut;
	DataPointer<real> mRightLeft;
	PointLightData data;

	bool mHasRotated;
	real mLeftDownTime;
	real mRightDownTime;
	real mOutDownTime;
	real mSpeed;
	const vec3 mStartPosition;
	const Quaternion mStartRotation;
	MyParticleSystem* mParticleSystem;

	bool mBoost;
	ComboTime mCombo;

	static Player* sInstance;
};

Player* Player::sInstance = 0;


class SpeedIndicatorObject : public Object2 {
public:
	SpeedIndicatorObject(Player* pPlayer) : font(FontManager::getInstance().getStandardFont()), mPlayer(pPlayer) {
		mRand.randomize();
	}

	void render(float pTime) {
		const vec3 speedVector = mPlayer->getSpeed();
		const real speed = math::clampZero(speedVector.getLength());
		FPRINT(font, 0, 0, "Speed: " << std::setprecision(3)
			<< speed );
	}
	bool step(real pTime) {
		const vec3 speedVector = mPlayer->getSpeed();
		const real speed = math::clampZero(speedVector.getLength());
		if (speed>3.5f) {
			std::string speedString = "lawl";
			if( speed > 4.0f ) speedString = "rolf";
			if( speed > 5.0f ) speedString = "lmao";
			if( speed > 6.0f ) speedString = "roflamo";
			if( speed > 7.0f ) speedString = "roflol";
			if( speed > 8.0f ) speedString = "^^ speeeeeeeed ^^";
			if( speed > 10.0f ) speedString = "omg";
			if( speed > 11.0f ) speedString = "w00t";
			if( speed > 12.0f ) speedString = "LOL";
			spawnText(speedString);
		}
		else {
			resetText();
		}
		return true;
	}

	real getRandomValue() {
		return 0.5f - mRand.randomFloatWithSign()*0.2f;
	}

	void spawnText(const std::string& pText) {
		if( pText != mLatestText ) {
			mLatestText = pText;
			const vec2 location(getRandomValue(),getRandomValue());
			mWorld->addObject( new TextInfo(pText, location, 1.0f, vec2(0, 0.1f)) );
		}
	}
	void resetText() {
		mLatestText = "";
	}

private:
	Player* mPlayer;
	Font& font;
	std::string mLatestText;
	Random mRand;
};

class PointIndicatorObject : public Object2 {
public:
	PointIndicatorObject() : font(FontManager::getInstance().getStandardFont()), mPoints(GameState::GameStatePimpl::getInstance().points) {
		mRand.randomize();
	}

	void render(float pTime) {
		unsigned long points = GameState::GameStatePimpl::getInstance().points;
		FPRINT(font, 0, 0.05f, "Score: " << std::setprecision(3)
			<< points );
	}
	bool step(real pTime) {
		unsigned long points = GameState::GameStatePimpl::getInstance().points;
		if( mPoints < points ) {
			std::stringstream str;
			str << points;
			spawnText(str.str());
		}
		mPoints = points;
		return true;
	}

	real getRandomValue() {
		return 0.5f - mRand.randomFloatWithSign()*0.2f;
	}

	void spawnText(const std::string& pText) {
		const vec2 location(getRandomValue(),getRandomValue());
		mWorld->addObject( new TextInfo(pText, location, 3.0f, vec2(0, 0.1f)) );
	}

private:
	Player* mPlayer;
	Font& font;
	Random mRand;
	unsigned long mPoints;
};


class MyCamera : public Camera3 {
public:
	MyCamera(Player* pPlayer, const bool pIsometric, const vec3& pPosition) : Camera3("camera", pPosition, Quaternion()), mPlayer(pPlayer), mIsometric(pIsometric), m360(false), mRotation360(0) {
		assert(!sInstance);
		sInstance = this;
	}
	~MyCamera() {
		assert(sInstance);
		sInstance = 0;
	}
	void doUpdate(float pTime) {
		const vec3 up = op::vec3::yAxisPositive;
		const real defaultHeight = 2;
		const real defaultDistance = 4;
		static real previousIsometricDistance = 1.0f;
		const real variableDistance = previousIsometricDistance + math::limitRange(-0.01f, math::limitRange(0.5f, mPlayer->getSpeed().getLength()*0.7f, 1.0f) - previousIsometricDistance, 0.01f);
		const real isometricDistance = 6 * variableDistance;
		previousIsometricDistance = variableDistance;
		const real height = (mIsometric)?isometricDistance:defaultHeight;
		const real distance = (mIsometric)?isometricDistance:defaultDistance;
		const Quaternion tempRotation = mPlayer->getRotation();
		mTime = math::limitRange(0.0f, mTime+pTime * 3.0f , 1.0f);
		if( tempRotation != mWantedRotation ) {
			mFromRotation = mFromRotation.slerp(mWantedRotation, mTime);
			mWantedRotation = tempRotation;
			mTime = pTime;
		}
		Quaternion rotation = mFromRotation.slerp(mWantedRotation, mTime);
		const bool do360 = m360 || GameState::GameStatePimpl::getInstance().rotateCamera;
		if( do360 ) {
			rotation *= Quaternion(op::vec3::yAxisPositive, mRotation360);
			mRotation360 += 4*pTime;
		}
		const vec3 pos = mPlayer->getPosition();
		if( do360 ) {
			moveTo(pos - rotation.getIn()*5 + up*5);
		}
		else {
			moveTo(pos - rotation.getIn()*distance + up*height);
		}
		lookAt(pos, up);
	}


	void do360() {
		m360 = true;
	}
	

	static MyCamera* getInstancePtr() {
		assert(sInstance);
		return sInstance;
	}
	static MyCamera& getInstance() {
		return *getInstancePtr();
	}

private:
	static MyCamera* sInstance;
	Player* mPlayer;
	Quaternion mFromRotation;
	real mTime;
	Quaternion mWantedRotation;
	bool mIsometric;
	bool m360;
	real mRotation360;
};

MyCamera* MyCamera::sInstance = 0;

class GameoverPlane : public Object3 {
public:
	GameoverPlane(float height, GameState* pNotifier, MyCamera* pCamera, Player* pPlayer) : Object3("GameOverPlane", false, false, true, CR_DEPENDENT_ON_CAMERA), mPlane(op::vec3::yAxisPositive, -height), mNotifier(pNotifier), mPlayer(pPlayer), mCamera(pCamera), mGameOver(false), mTime(0) {
	}

	void update(float pTime) {
		if( mGameOver ) {
			getWorld().setTimeMultiplier(0.15f);
			mTime += pTime;
			if( mTime > 0.6f ) {
				mNotifier->tellToResetLevel();
			}
		}
		else {
			const PlaneRelation pr = mPlane.classify( Player::getInstance().getPosition() );
			if( pr != PR_FRONT ) {
				mNotifier->gameOver();
				GameState::GameStatePimpl::getInstance().loosePoints = false;
				mGameOver = true;
				mCamera->do360();
				mPlayer->update(0.0f);
				mPlayer->freeze();
				getWorld().setTimeMultiplier(0.15f);
			}
		}
	}
private:
	Plane mPlane;
	GameState* mNotifier;
	MyCamera* mCamera;
	Player* mPlayer;
	bool mGameOver;
	real mTime;
};


class Shadow : public Actor {
public:
	Shadow(PhysActor* pActor, const std::string& pName) : Actor(pName, vec3(0,0,0), Quaternion(), true), mActor(pActor) {
		assert(mActor);
		loadComponentsFromFile("shadow.obj");
	}
	void doUpdate(real pTime) {
		assert(mActor);
		real dist = mActor->distanceToWorld(vec3(0, -99, 0));
		moveTo(mActor->getPosition() - vec3(0, dist, 0) );
	}
private:
	PhysActor* mActor;
};

class GoalPoint : public Actor {
public:
	GoalPoint(const vec3& pGoal, GameState* pNotifier, MyCamera* pCamera, Player* pPlayer) : Actor("goal", pGoal, Quaternion(), true), mGoal(pGoal), mNotifier(pNotifier), mParticleSystem(0), mCamera(pCamera), mPlayer(pPlayer), mInGoal(false), mTimeInGoal(0) {
		assert(mCamera);
		assert(mPlayer);
		assert(mNotifier);
		mParticleSystem = new GoalParticleSystem(this);
		addComponent( mParticleSystem );
	}

	void doUpdate(float pTime) {
		if( mInGoal ) {
			getWorld().setTimeMultiplier(0.15f);
			mTimeInGoal += pTime;
			if( mTimeInGoal > 0.7f ) {
				mNotifier->loadNextLevel();
				freeze();
			}
		}
		else {
			const real lengthSquared =
				op::vec3::lengthBetweenSquared(mGoal, Player::getInstance().getPosition());
			const real radius = 1.0f;
			if( lengthSquared < math::square(radius)) {
				mNotifier->completeLevel();
				mCamera->do360();
				mPlayer->update(0.0f);
				mPlayer->freeze();
				getWorld().setTimeMultiplier(0.10f);
				mInGoal = true;
			}
			assert(mParticleSystem);
			mParticleSystem->step(pTime);
		}
	}
private:
	const vec3 mGoal;
	GameState* mNotifier;
	Player* mPlayer;
	MyCamera* mCamera;
	GoalParticleSystem* mParticleSystem;
	bool mInGoal;
	real mTimeInGoal;
};


void GameState::onKey(Key pKey, bool pIsDown) {
	switch(pKey) {
	case K_UP: mIn = pIsDown; break;
	case K_DOWN: mOut = pIsDown; break;
	case K_RIGHT: mRight = pIsDown; break;
	case K_LEFT: mLeft = pIsDown; break;
	/*case K_R:
		{
			resetLevel();
		}
		break;*/
	case K_R_CTRL:
	case K_L_CTRL:
	case KM_RIGHT:
	case KJ_SLOWMO:
		if( Player::getInstance().isDynamic() ) {
			if( pIsDown ) {
				Player::getInstance().setBoost(true);
				world.setTimeMultiplier(0.2f);
			}
			else {
				Player::getInstance().setBoost(false);
				world.setTimeMultiplier(1.0f);
			}
		}
		break;
	case K_L_SHIFT:
	case K_R_SHIFT:
	case KM_LEFT:
	case KJ_JUMP:
		if( Player::getInstance().isDynamic() ) {
			if( pIsDown ) {
				Player::getInstance().jump(mLatestUpdateTime);
			}
		}
		break;
	case K_PLUS:
	case K_KP_PLUS:
	case K_PAGEUP:
		mRotateLeft = pIsDown;
		break;
	case K_MINUS:
	case K_KP_MINUS:
	case K_PAGEDOWN:
		mRotateRight = pIsDown;
		break;
	case K_P:
	case KJ_PAUSE:
		if( pIsDown ) {
			// can't pause while we have completed or died
			if( !mCompletedLevel && !mAllowRestart ) {
				enableState("pause");
			}
		}
		break;/*
	case K_BACKSPACE:
		if( pIsDown ) {
			doLoadNextLevel();
		}
		break;*/
	case K_ESCAPE:
	case KJ_MENU:
		if( pIsDown ) {
			disableThis();
		}
		break;
	case K_RETURN:
	case KJ_RESTART:
		if( mAllowRestart ) {
			resetLevel();
			world.setTimeMultiplier(1.0f);
		}
		else if( mCompletedLevel ) {
			doLoadNextLevel();
		}
		break;
	}
}

void GameState::resetLevel() {
	loadWorld( mPrivateData->currentLevel );
}
void GameState::gameOver() {
	hud.addObject( new TextInfo("Fallout", vec2(0.4f, 0.5f), 4.0f, vec2(0,-0.05f) ) );
	mAllowRestart = true;
#ifdef HEART_SFX
	FmodSystem::getInstance().playHeart(false);
#endif
}
void GameState::completeLevel() {
	hud.addObject( new TextInfo("Level complete!", vec2(0.4f, 0.5f), 4.0f, vec2(0,-0.05f)) );
	mPrivateData->points += 500;
	mPrivateData->loosePoints = false;
	hud.addObject( new HiScoreObject() ); // show high score after each level
	mCompletedLevel = true;
#ifdef HEART_SFX
	FmodSystem::getInstance().playHeart(false);
#endif
}
void GameState::loadNextLevel() {
	if( mPrivateData->nextLevel == "complete" ) {
		doLoadNextLevel();
	}
	else {
		hud.addObject( new PressAnyKeyObject("continue") );
	}
}
void GameState::tellToResetLevel() {
	hud.addObject( new PressAnyKeyObject("restart") );
}
void GameState::doLoadNextLevel() {
	// load next level
	if( mPrivateData->nextLevel == "complete" ) {
		mPrivateData->loadedWorld = false;
		hud.addObject( new GameCompletedObject() );
		world.setTimeMultiplier(0.10f);
	}
	else {
		loadWorld(mPrivateData->nextLevel);
		world.setTimeMultiplier(1.0f);
	}
}

GameState::GameState() : State("game"), world(), mLeft(false), mRight(false), mIn(false), mOut(false),
										mRightLeft("rightleft", 0.0f),
										mInOut("inout", 0.0f), mDx(0), mDy(0),
										mLatestUpdateTime(0), mPrivateData(new GameStatePimpl()), mCompletedLevel(false), mAllowRestart(false), mRotateLeft(false), mRotateRight(false) {
	//doLoadWorld("level01");
	assert( !sInstance );
	sInstance = this;
}
void GameState::loadWorld(const std::string& pFile) {
	mLoadWorldString = pFile;
}

void GameState::unloadLevel() {
	mPrivateData->loadedWorld = false;
	world.removeAllObjects();
	hud.removeAllObjects();
	mCompletedLevel = false;
	mAllowRestart = false;
}

void GameState::doLoadWorld(const std::string& pFile) {
	Quaternion playerRotation;
	bool setRotation = false;
	if( Player::getInstancePtr() ) {
		if( mPrivateData->currentLevel == pFile )  {
			playerRotation = Player::getInstance().getRotation();
			setRotation = true;
		}
	}
	// make sure we are empty
	unloadLevel();

	std::istringstream file;
	loadFileIntoString(pFile + ".lvl", &file);
	TiXmlDocument doc; doc.Parse(file.str().c_str());
	world.load(doc);

	vec3 start(0,0,0);
	vec3 end(5, 5, 5);
	Quaternion rotation;
	real gameOverY = -5;
	std::string levelName = "";
	mPrivateData->nextLevel = "";

	{
		TiXmlHandle docHandle( &doc );
		for(TiXmlElement* entity = docHandle.FirstChild("level").FirstChild("entity").ToElement(); entity; entity = entity->NextSiblingElement("entity") ) {
			const char* namePointer = entity->Attribute("name");
			const std::string name = namePointer?namePointer:"";
			const char* typePointer = entity->Attribute("type");
			const std::string type = typePointer?typePointer:"";

			if( type == "_start" ) {
				start = getPosition(entity);
				rotation = getRotation(entity);
			} else if( type == "_end" ) {
				end = getPosition(entity);
			} else if( type == "_GameOverPlane" ) {
				gameOverY = getPosition(entity).getY();
			} else if( type == "_LevelName" ) {
				levelName = name;
			} else if( type == "_NextLevel" ) {
				mPrivateData->nextLevel = name;
			}

		}
	}

	Player* player = new Player(start, rotation, world);
	hud.addObject( new SpeedIndicatorObject(player) );
	hud.addObject( new PointIndicatorObject() );
	world.add( player );
	world.add( new Shadow(player, "pshadow") );
	MyCamera* cam = new MyCamera(player, true, vec3(5, 5, 5));
	world.add( cam );
	world.add( new GameoverPlane(gameOverY, this, cam, player)  );
	world.add( new GoalPoint(end, this, cam, player) );
	world.selectCamera("camera");
	if( levelName != "" ) {
		hud.addObject( new TextInfo(levelName, vec2(0.3f, 0.5f), 3, vec2(0,-0.05f)) );
	}
	mPrivateData->levelName = levelName;
	mPrivateData->currentLevel = pFile;
	mPrivateData->loosePoints = true;
	world.setTimeMultiplier(1.0f);
	mLoadWorldString = "";
	mPrivateData->loadedWorld = true;

	if( setRotation ) {
		Player::getInstance().rotateTo(playerRotation);
	}
}
void GameState::onTop() {
}
GameState::~GameState() {
	assert( sInstance );
	sInstance = 0;
	world.removeAllObjects();
}

void GameState::onStep(float pTime, unsigned int pDepth) {
	mLatestUpdateTime = pTime;

	if( mLoadWorldString != "" ) {
		doLoadWorld(mLoadWorldString);
	}

	real dx = 0;
	real dy = 0;

	if( mRight ) dx += 1;
	if( mLeft ) dx -= 1;
	if( mIn ) dy += 1;
	if( mOut ) dy -= 1;

	mRightLeft.set( ::math::limitRange(-1, dx + mDx, 1) );
	mInOut.set( ::math::limitRange(-1, dy + mDy, 1) );

	if( mRotateLeft ) {
		Player::getInstance().increaseRotation( Quaternion(op::vec3::yAxisPositive, pTime) );
	}
	if( mRotateRight ) {
		Player::getInstance().increaseRotation( Quaternion(op::vec3::yAxisPositive, -pTime) );
	}

	if( pDepth == 0 ) {
		mPrivateData->rotateCamera = false;
		mPrivateData->step(pTime);
		world.update(true, pTime);
		hud.step(pTime);
	}
	else {
		mPrivateData->rotateCamera = true;
		world.update(false, 0);
		MyCamera::getInstance().update(pTime*0.1f);
		hud.step(pTime);
	}
	mDx = 0;
	mDy = 0;
}
void GameState::onFrame(float pTime, unsigned int pDepth){
}
void GameState::onRender(float pTime, unsigned int pDepth) {
	checkOpengl();
	world.render(pTime);
	checkOpengl();
	hud.render(pTime);
	checkOpengl();
}
void GameState::onAxis(Axis pAxis, real pValue) {
	DataPointer<real> sensitivity("sensitivity");
	switch(pAxis) {
		case AXIS_MOUSE_X:
			mDx += pValue * sensitivity.get();
			break;
		case AXIS_MOUSE_Y:
			mDy += pValue * sensitivity.get();
			break;
		case AXIS_JOY_X:
			mDx += pValue;
			break;
		case AXIS_JOY_Y:
			mDy += pValue;
			break;
	}
}

GameState& GameState::getInstance() {
	return *getInstancePtr();
}
GameState* GameState::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}

bool GameState::hasWorld() {
	return mPrivateData->loadedWorld;
}

int GameState::getPoints() {
	return mPrivateData->points;
}

GameState* GameState::sInstance = 0;

void GameState::resetScore() {
	mPrivateData->points = 1000;
	world.setTimeMultiplier(1.0f);
}