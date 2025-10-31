#include "particle.h"
#include "precision.h"
#include <cassert>

void Particle::integrate(real duration){
    const float groundY = 0.0f;
    assert(duration > 0.0);

    position.addScaledVector(velocity, duration);

    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    velocity.addScaledVector(resultingAcc, duration);

    velocity*=real_pow(damping, duration);

    clearAccumulator();
}

void Particle::setMass(const real m){
    mass = m;
    inverseMass = 1/m;
}

void Particle::clearAccumulator(){
    forceAccum.clear();
}

void Particle::addForce(const Vector3 &force){
    forceAccum += force;
}

void Particle::setPosition(const Vector3 &pos){
    position = pos;
}

void Particle::setVelocity(const Vector3 &vel){
    velocity = vel;
}

void Particle::setAcceleration(const Vector3 &acc){
    acceleration = acc;
}

void Particle::setDamping(const real d){
    damping = d;
}