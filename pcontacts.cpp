#include "pcontacts.h"

void ParticleContact::resolve(real duration){
    resolveVelocity(duration);
}

void ParticleContact::calculateSeperatingVelocity() const{
    Vector3 relativeVelocity = particle[0]->getVelocity();
    if(particle[1]){
        relativeVelocity -= particle[1]->getVelocity();
    }
    return relativeVelocity*contactNormal;
}

void ParticleContact::resolveVelocity(real duration){
    real seperatingVelocity = calculateSeperatingVelocity();

    if(seperatingVelocity > 0){
        return;
    }

    real newSeperatingVelocity = -seperatingVelocity * restitution;

    Vector3 accCausedVelocity = particle[0]->getAcceleration();
    if(particle[1]){
        accCausedVelocity -= particle[1]->getAcceleration();
    }
    real accCausedSeperatingVelocity = accCausedVelocity * contactNormal * duration;
    if (accCausedSeperatingVelocity < 0){
        newSeperatingVelocity += restitution * accCausedSeperatingVelocity;
        if(newSeperatingVelocity < 0){
            newSeperatingVelocity = 0;
        }
    }
    
    real deltaVelocity = newSeperatingVelocity - seperatingVelocity;

    real totalInverseMass = particle[0]->getInverseMass();
    if(particle[1]){
        totalInverseMass += particle[1].getInverseMass();
    }

    if(totalInverseMass <= 0) return;

    real impulse = deltaVelocity/totalInverseMass;
    Vector3 impulsePerUnitMass = contactNormal * impulse;

    particle[0]->setVelocity(particle[0]->getVelocity()+impulsePerUnitMass*particle[0]->getInverseMass());
    if(particle[1]){
        particle[1]->setVelocity(particle[1]->getVelocity()+impulsePerUnitMass*-particle[0]->getInverseMass())
    }
}

void ParticleCOntact::resolveInterpenetration(real duration){
    if(penetration <= 0) return;

    real totalInverseMass = particle[0]->getInverseMass();
    if(particle[1]){
        totalInverseMass += particle[1].getInverseMass();
    }

    if(totalInverseMass <= 0) return;

    Vector3 movePerUnitMass = contactNormal * -(penetration/totalInverseMass);

    particle[0]->setPosition(particle[0]->getPosition()+movePerUnitMass/particle[0]->getInverseMass);
    if(particle[1]){
        particle[1]->setPosition(particle[1]->getPosition()+movePerUnitMass/particle[1]->getInverseMass);
    }
}

void ParticleContactResolver::resolveContacts(ParticleContact *contactArray, unsigned numContacts, real duration){
    iterationsUsed = 0;
    while(iterationsUsed < iterations){
        real max = 0;
        unsigned maxIndex = numContacts;
        for (unsigned i = 0; i < numContacts; i++){
            real sepVel = contactArray[i].calculateSeparatingVelocity();
            if (sepVel < max){
                max = sepVel;
                maxIndex = i;
            }
        }
        contactArray[maxIndex].resolve(duration);
        iterationsUsed++;
    }
}