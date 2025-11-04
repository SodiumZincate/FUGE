#pragma once
#include "precision.h"
#include "core.h"
#include "pfgen.h"
#include "pcontacts.h"
#include "particle.h"
#include <vector>

namespace cyclone {
    typedef std::vector<Particle*> Particles;
    class ParticleWorld {
        struct ParticleRegistration {
            Particle* particle;
            ParticleRegistration* next;
        };
        
        ParticleRegistration* firstParticle = nullptr;
        ParticleForceRegistry registry;
        ParticleContactResolver resolver;
        
        struct ContactGenRegistration {
            ParticleContactGenerator* gen;
            ContactGenRegistration* next;
        };
        
        ContactGenRegistration* firstContactGen = nullptr;
        
        ParticleContact* contacts = nullptr;
        unsigned maxContacts = 0;
        bool calculateIterations = false;
        
    public:
        ParticleWorld(unsigned maxContacts, unsigned iterations = 0);

        void startFrame();
        unsigned generateContacts();
        void integrate(real duration);
        void runPhysics(real duration);
        void addParticle(Particle* particle);
        void addContactGenerator(ParticleContactGenerator* contact);

        ParticleForceRegistry& getForceRegistry();
        ParticleRegistration* getParticleReg();
        ContactGenRegistration* getContactGen();
    };

    // Ground contact with full definition
    class GroundContact : public ParticleContactGenerator {
        public:
            void init(Particles* p, real res);
            virtual unsigned addContact(ParticleContact* contact, unsigned limit) const override;
        private:
            Particles* particles;
            real restitution;
    };

    class ParticleCollisionContact : public ParticleContactGenerator {
        public:
            void init(Particles* p, real res, real radius);
            virtual unsigned addContact(ParticleContact* contact, unsigned limit) const override;
        private:
            Particles* particles;
            real restitution;
            real radius;
    };
};
