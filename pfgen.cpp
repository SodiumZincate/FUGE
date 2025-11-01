#include "pfgen.h"
#include <iostream>

using namespace cyclone;

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg){
    registrations.push_back({particle,fg});
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator *fg){
    Registry::iterator i = registrations.begin();
    for(; i!=registrations.end(); i++){
        if(i->particle == particle && i->fg == fg){
            i = registrations.erase(i);
            std::cout << "REMOVED" << std::endl;
        }
    }
}

void ParticleForceRegistry::clear(){
    registrations.clear();
}

void ParticleForceRegistry::updateForces(real duration){
    Registry::iterator i = registrations.begin();
    for(; i!=registrations.end(); i++){
        i->fg->updateForce(i->particle, duration);
    }
}

void ParticleGravity::updateForce(Particle *particle, real duration){
    if(!particle->hasFiniteMass()) return;

    particle->addForce(gravity*particle->getMass());
}

void ParticleDrag::updateForce(Particle *particle, real duration){
    Vector3 force;
    particle->getVelocity(&force);

    real dragCoeff = force.magnitude();
    dragCoeff = k1*dragCoeff + k2*dragCoeff*dragCoeff;

    force.normalize();

    force*= -dragCoeff;
    particle->addForce(force);
}

void ParticleSpring::updateForce(Particle *particle, real duration){
    Vector3 force = particleA->getPosition() - particleB->getPosition();

    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    particleA->addForce(force);
    particleB->addForce(force*-1);
}

void ParticleAnchoredSpring::updateForce(Particle *particle, real duration){
    Vector3 force;
    particle->getPosition(&force);
    force -= *anchor;

    real magnitude = force.magnitude();
    magnitude = (real_abs(magnitude - restLength));
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    particle->addForce(force);
}

void ParticleBungee::updateForce(Particle *particle, real duration){
    Vector3 force = particleA->getPosition() - particleB->getPosition();

    real magnitude = force.magnitude();
    if(magnitude <= restLength) return;

    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    particleA->addForce(force);
    particleB->addForce(force*-1);
}

void ParticleBuoyancy::updateForce(Particle *particle, real duration){
    real depth = particle->getPosition().y;
    
    // out of water
    if(depth >= waterHeight+particle->maxDepth) return;
    Vector3 force(0,0,0);

    // inside water check if max depth
    if(depth <= waterHeight-particle->maxDepth){
        force.y = liquidDensity*particle->volume;
        particle->addForce(force);
        return;
    }

    // partially submerged: clamp to 0..1
    real submergedFraction = (waterHeight + particle->maxDepth - depth) / (2 * particle->maxDepth);
    if(submergedFraction < 0.0f) submergedFraction = 0.0f;
    if(submergedFraction > 1.0f) submergedFraction = 1.0f;

    force.y = liquidDensity * particle->volume * submergedFraction;
    particle->addForce(force);
}