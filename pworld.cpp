#include "particle.h"
#include "pworld.h"
#include <iostream>

using namespace cyclone;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
:
resolver(iterations),
maxContacts(maxContacts)
{
    contacts = new ParticleContact[maxContacts];
    calculateIterations = (iterations == 0);

}

void ParticleWorld::startFrame(){
    ParticleRegistration *reg = firstParticle;

    while(reg){
        reg->particle->clearAccumulator();
        reg = reg->next;
    }
}

unsigned ParticleWorld::generateContacts(){
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;

    ContactGenRegistration *reg = firstContactGen;
    while(reg){
        unsigned used = reg->gen->addContact(nextContact, limit);
        if (used > limit) {
        std::cerr << "ERROR: contact generator returned used=" << used
                << " but limit=" << limit << " (clamping to limit)\n";
        used = limit;
    }
        limit -= used;
        nextContact += used;

        if(limit<=0) break;

        reg = reg->next;   
    }
    return maxContacts-limit;
}

void ParticleWorld::integrate(real duration){
    ParticleRegistration *reg = firstParticle;
    while(reg){
        reg->particle->integrate(duration);

        reg = reg->next;
    }
}

void ParticleWorld::runPhysics(real duration){
    registry.updateForces(duration);

    integrate(duration);

    unsigned usedContacts = generateContacts();

    if(calculateIterations){
        resolver.setIterations(usedContacts*2);
    }
    resolver.resolveContacts(contacts, usedContacts, duration);
}

void ParticleWorld::addParticle(Particle* particle){
    ParticleRegistration* reg = new ParticleRegistration();
    reg->particle = particle;
    reg->next = firstParticle;
    firstParticle = reg;
}

void ParticleWorld::addContactGenerator(ParticleContactGenerator* particleConGen){
    
    ContactGenRegistration *reg = new ContactGenRegistration();
    reg->gen = particleConGen;
    reg->next = firstContactGen;
    firstContactGen = reg; 
}


unsigned GroundContact::addContact(ParticleContact *contact, unsigned limit) const {
    if (limit == 0 || particle->getPosition().y >= 0) return 0;

    contact->particle[0] = particle;
    contact->particle[1] = nullptr;
    contact->contactNormal = Vector3(0, 1, 0); // pointing up
    contact->penetration = -particle->getPosition().y;
    contact->restitution = restitution;

    return 1;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry(){
    return registry;
}

// Sample game loop
// void loop(){
//     while(1){
//         world.startFrame;

//         runGraphicsUpdate();
//         updateCharacters();

//         world.runPhysics();

//         if(gameOver) break;
//     }
// }