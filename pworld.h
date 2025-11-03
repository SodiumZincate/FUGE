#pragma once
#include "core.h"
#include "pfgen.h"
#include "pcontacts.h"
#include "pworld.h"

class ParticleWorld{
    struct ParticleRegistration{
        Particle* particle;
        ParticleRegistration* next;
    };

    ParticleRegistration* firstParticle;

    ParticleForceRegistry *registry;
    ParticleContactResolver *resolver;

    struct ContactGenRegistration{
        ParticleContactGenerator *gen;
        ContactGenRegistration *next;
    };

    ContactGenRegistration *firstContactGen;

    ParticleContact *contacts;
    unsigned maxContacts;
    
    public:
        ParticleWorld(unsigned maxContacts, unsigned iterations=0);
        void startFrame();
        unsigned generateContacts();
        void integrate(real duration);
        void runPhysics(real durtaion);
};
