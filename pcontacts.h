#pragma once
#include "precision.h"
#include "core.h"
#include "pfgen.h"
#include "particle.h"

namespace cyclone {
    class ParticleContact {
    public:
        Particle* particle[2];
        real restitution;
        Vector3 contactNormal;
        real penetration;
        Vector3 particleMovement[2];

        void resolve(real duration);
        real calculateSeparatingVelocity() const;

    private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);
    };

    class ParticleContactResolver {
    protected:
        unsigned iterations;
        unsigned iterationsUsed;
    public:
        ParticleContactResolver(unsigned iterations = 0);
        void setIterations(unsigned iterations);
        void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
    };

    class ParticleContactGenerator {
    public:
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
    };
}
