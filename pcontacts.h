#pragma once
#include "precision.h"
#include "core.h"

class ParticleContact{
    public:
        Particle* particle[2];
        real restitution;
        Vector3 contactNormal;
        real penetration;

    protected:
        void resolve(real duration);
        real calculateSeperatingVelocity() const;

    private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);
};

class ParticleContactResolver
{
    protected:
    unsigned iterations;
    unsigned iterationsUsed;
    public:
        ParticleContactResolver(unsigned iterations);
        void setIterations(unsigned iterations);
        void resolveContacts(ParticleContact *contactArray, unsigned numContacts, real duration);
};