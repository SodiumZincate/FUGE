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
		void integrate(real duration);
        
        Vector3 forceAccum;
        void clearAccumulator();

        void addForce(const Vector3 &force);
        void setMass(const real mass);
        void setPosition(const Vector3 &v);
        void setVelocity(const Vector3 &v);
        void setAcceleration(const Vector3 &v);
        void setDamping(const real damping);
};