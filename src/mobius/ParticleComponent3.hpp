#ifndef PARTICLE_COMPONENT3_HPP
#define PARTICLE_COMPONENT3_HPP

#include "vec3.hpp"
#include "Component3.hpp"
#include "Actor.hpp"
#include "Random.hpp"
#include <boost/scoped_array.hpp>
#include "Camera3.hpp"
#include "Quaternion.hpp"
#include <cassert>

template <class T> // t should be a subclass of ParticleBase
struct ParticleBase {
	T* next;

	vec3 position;
	vec3 futurePosition;

	vec3 color;
	vec3 futureColor;

	float size;
	float futureSize;

	float alpha;
	float futureAlpha;

	ParticleBase() : next(0), position(0,0,0), futurePosition(0,0,0), color(0,0,0), futureColor(0,0,0) {}
};

struct Particle : public ParticleBase<Particle> {
};

enum FailedCreationAction {
	FCA_IGNORE,
	FCA_TAKE_OLDEST
};

template <class TParticle>
class PointEmitter {
public:
	PointEmitter(Actor* pActor, Random& pRandom) : mActor(pActor), mRand(pRandom) {
		mRand.randomize();
	}
	void emit(TParticle* pParticle, float pRange) {
		assert(pParticle);
		pParticle->position =
			pParticle->futurePosition =
			mActor->getPositionObject().estimatePosition( math::limitRange(0.0f, pRange + mRand.randomFloatWithSign() * 0.01f, 1.0f) );
	}
private:
	Actor* mActor;
	Random& mRand;
};

template <class TParticleSystem, class TParticle, class TEmitter>
class ParticleComponent3 : public Component3 {
public:
	ParticleComponent3(unsigned int pParticleCount, const TEmitter& pEmitter, Object3* pOwner) : Component3(pOwner), mEmitter(pEmitter), mParticles( new TParticle[pParticleCount] ), mAlive(0), mDead(0), mActiveParticles(0) {
		TParticle* lastParticle = 0;
		for(unsigned int particleIndex=0; particleIndex<pParticleCount; ++particleIndex) {
			TParticle* particle = &mParticles[particleIndex];
			if( lastParticle ) {
				lastParticle->next = particle;
			}
			else {
				mDead = particle;
			}
			lastParticle = particle;
		}
	}
	// returns true if should continue updating
	bool updateParticles(real pTime) {
		TParticle* previousParticle = 0;
		TParticle* currentParticle = mAlive;
		mActiveParticles = 0;
		while( currentParticle ) {
			TParticle* next = currentParticle->next;
			if( static_cast<TParticleSystem*>(this)->updateParticle(currentParticle, pTime) ) {
				++mActiveParticles;
				previousParticle = currentParticle;
			}
			else {
				if( previousParticle ) {
					previousParticle->next = currentParticle->next;
				}
				else {
					mAlive = currentParticle->next;
				}
				currentParticle->next = mDead;
				mDead = currentParticle;
			}
			currentParticle = next;
		}
		return mActiveParticles!=0;
	}
	void addParticles(unsigned int pParticleCount, FailedCreationAction pAction) {
		for(unsigned int particleAdded=0; particleAdded<pParticleCount; ++particleAdded) {
			// add a particle
			TParticle* particle = mDead;
			if( particle ) {
				mDead = particle->next;
			}
			else {
				// no dead particles to reanimate
				if( pAction == FCA_IGNORE ) {
					// we can't possible create anything more, so let's don't try
					return;
				}
				else if( FCA_TAKE_OLDEST) {
					TParticle* oldest = 0;
					TParticle* nextOldest = 0;
					for(TParticle* currentParticle=mAlive; currentParticle; currentParticle=currentParticle->next) {
						nextOldest = oldest;
						oldest = currentParticle;
					}
					assert(oldest);
					assert(nextOldest);
					nextOldest->next = 0;
					particle = oldest;

				}
				else {
					assert(0 && "bad value of pAction");
				}
			}

			// particle should be valid
			assert(particle); // let's make sure it is

			mEmitter.emit(particle, particleAdded/real(pParticleCount));
			static_cast<TParticleSystem*>(this)->setupParticle(particle);
			// add it to the list
			particle->next = mAlive;
			mAlive = particle;
		}
	}

	void renderParticles(const World3& pWorld, real pTime) const {
		setupRenderingForParticles();
		for(TParticle* currentParticle=mAlive; currentParticle; currentParticle=currentParticle->next) {
			static_cast<const TParticleSystem*>(this)->renderParticle(*currentParticle, pWorld, pTime);
		}
		restoreRenderingForParticles();
	}
	void renderParticlesAsBillboards(const World3& pWorld, real pTime) const {
		setupRenderingForParticles();
		const vec3 up = pWorld.getCamera().getRotationObject().getCurrentRotation().getUp();
		const vec3 right = pWorld.getCamera().getRotationObject().getCurrentRotation().getRight();
		for(TParticle* currentParticle=mAlive; currentParticle; currentParticle=currentParticle->next) {
			const TParticle& p = *currentParticle;
			::renderBillboard(up, right, pWorld,
				op::vec3::getLinearInterpolation(p.position, pTime, p.futurePosition),
				op::vec3::getLinearInterpolation(p.color, pTime, p.futureColor),
				math::interpolate(p.alpha, pTime, p.futureAlpha),
				math::interpolate(p.size, pTime, p.futureSize) );
		}
		restoreRenderingForParticles();
	}
private:
	Actor* mActor;
	boost::scoped_array<TParticle> mParticles;
	TParticle* mAlive;
	TParticle* mDead;
	TEmitter mEmitter;
	unsigned long mActiveParticles;
};

void setupRenderingForParticles();
void restoreRenderingForParticles();
void renderBillboard(const vec3& pUp, const vec3& pRight, const World3& pWorld, const vec3& pPosition, const vec3& pColor, real pAlpha, real pSize); 

#endif