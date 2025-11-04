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
    // --- LOG BEFORE CONTACT ---
    for (unsigned i = 0; i < usedContacts; i++) {
        ParticleContact& c = contacts[i];
        if (!c.particle[0] || !c.particle[1]) continue;  // skip single-particle contacts
        std::cout << "[BEFORE CONTACT] Particle0 vel: " 
                << c.particle[0]->getVelocity().x << " "
                << c.particle[0]->getVelocity().y << " "
                << c.particle[0]->getVelocity().z << " "
                << ", Particle1 vel: " 
                << c.particle[1]->getVelocity().x << " "
                << c.particle[1]->getVelocity().y << " "
                << c.particle[1]->getVelocity().z << " "
                << std::endl;
    }

    if(calculateIterations){
        resolver.setIterations(usedContacts*2);
    }
    resolver.resolveContacts(contacts, usedContacts, duration);
    for (unsigned i = 0; i < usedContacts; i++) {
        ParticleContact& c = contacts[i];
        if (!c.particle[0] || !c.particle[1]) continue;
        std::cout << "[AFTER CONTACT] Particle0 vel: "
                << c.particle[0]->getVelocity().x << " "
                << c.particle[0]->getVelocity().y << " "
                << c.particle[0]->getVelocity().z << " "
                << ", Particle1 vel: " 
                << c.particle[1]->getVelocity().x << " "
                << c.particle[1]->getVelocity().y << " "
                << c.particle[1]->getVelocity().z << " "
                << std::endl;
    }
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

void GroundContact::init(Particles* p, real res){
    particles = p;
    restitution = res;
}

unsigned GroundContact::addContact(ParticleContact *contact, unsigned limit) const {
    unsigned count = 0;
    if (limit == 0) return 0;

    for(Particles::iterator p = particles->begin(); p!= particles->end(); p++){
        real y = (*p)->getPosition().y;
        if(y < 0.0f){
            contact->particle[0] = *p;
            contact->particle[1] = nullptr;
            contact->contactNormal = Vector3(0, 1, 0); // pointing up
            contact->penetration = -contact->particle[0]->getPosition().y;
            contact->restitution = restitution;
            contact++;
            count++;
        }
        if (count >= limit) return count; 
    }
    return count;
}

void ParticleCollisionContact::init(Particles* p, real res, real rad){
    particles = p;
    restitution = res;
    radius = rad;
}

unsigned ParticleCollisionContact::addContact(ParticleContact *contact, unsigned limit) const {
    unsigned count = 0;
    if (limit == 0) return 0;

    for(Particles::iterator p1 = particles->begin(); p1!= particles->end(); p1++){
        for(Particles::iterator p2 = p1+1; p2 != particles->end(); p2++){
            contact->particle[0] = *p1;
            contact->particle[1] = *p2;

            Vector3 delta = contact->particle[0]->getPosition() - contact->particle[1]->getPosition();
            real distance = delta.magnitude();
            real penetration = (radius*2.0f) - distance;

            if(penetration <= 0) continue;
            
            contact->contactNormal = delta/distance;
            contact->penetration = penetration;
            contact->restitution = restitution; 
            contact++;
            count++;
        }
        if (count >= limit) return count; 
    }
    return count;
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