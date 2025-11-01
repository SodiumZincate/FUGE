#pragma once
#include "precision.h"
#include "core.h"
#include <vector>

class ParticleForceGenerator{
    public:
        virtual void updateForce(Particle *p, real duration) = 0;
};

namespace cyclone{
    class ParticleForceRegistry{
        protected:
            struct ParticleForceRegistration{
                Particle *particle;
                ParticleForceGenerator *fg;
            };

            typedef std::vector<ParticleForceRegistration> Registry;
            Registry registrations;

        public:
            void add(Particle* particle, ParticleForceGenerator *fg);
            void remove(Particle* particle, ParticleForceGenerator *fg);
            void clear();
            void updateForces(real duration);
    };
}

class ParticleGravity : public ParticleForceGenerator{
    Vector3 gravity;
    public:
        ParticleGravity(const Vector3 &gravity):gravity(gravity){}

        virtual void updateForce(Particle *particle, real duration);
};

class ParticleDrag : public ParticleForceGenerator{
    real k1, k2;
    public:
        ParticleDrag(real k1, real k2):k1(k1), k2(k2){}

        virtual void updateForce(Particle *particle, real duration);
};

class ParticleSpring : public ParticleForceGenerator{
    Particle *particleA;
    Particle *particleB;
    real springConstant;
    real restLength;
    public:
        ParticleSpring(Particle* particleA, Particle* particleB, real springConstant, real restLength):
            particleA(particleA), particleB(particleB), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(Particle *particleA, real duration);
};

class ParticleAnchoredSpring : public ParticleForceGenerator{
    Vector3 *anchor;
    real springConstant;
    real restLength;
    public:
        ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength):anchor(anchor), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(Particle *particle, real duration);
};

class ParticleBungee : public ParticleForceGenerator{
    Particle *particleA;
    Particle *particleB;
    real springConstant;
    real restLength;
    public:
        ParticleBungee(Particle* particleA, Particle* particleB, real springConstant, real restLength):
            particleA(particleA), particleB(particleB), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(Particle *particleA, real duration);
};

class ParticleBuoyancy : public ParticleForceGenerator{
    real waterHeight;
    real liquidDensity;

    public:
        ParticleBuoyancy(real waterHeight, real liquidDensity):
            waterHeight(waterHeight), liquidDensity(liquidDensity){}
        virtual void updateForce(Particle *particle, real duration);
};