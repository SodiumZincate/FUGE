#pragma once
#include "core.h"
using namespace cyclone;

class Particle{
	public:
		Vector3 position;
		Vector3 velocity;
		Vector3 acceleration;
		real damping;
		real mass;
		real inverseMass;
        real volume = 0.064f;
        real maxDepth = 0.2f;
		void integrate(real duration);
        
        Vector3 forceAccum;
        void clearAccumulator();

        void addForce(const Vector3 &force);
        void setMass(const real mass);
        real getMass();
        real getInverseMass();
        bool hasFiniteMass();
        void setVolume(const real volume);
        void setPosition(const Vector3 &v);
        Vector3 getPosition();
        void getPosition(Vector3* v);
        void setVelocity(const Vector3 &v);
        Vector3 getVelocity();
        void getVelocity(Vector3* v);
        void setAcceleration(const Vector3 &v);
        Vector3 getAcceleration();
        void getAcceleration(Vector3 *v);
        void setDamping(const real damping);
};