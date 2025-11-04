#include "particle.h"
#include "precision.h"
#include <iostream>
#include <cassert>

void Particle::integrate(real duration){
    assert(duration > 0.0);

    position.addScaledVector(velocity, duration);

    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    velocity.addScaledVector(resultingAcc, duration);
    velocity*=real_pow(damping, duration);

    real kineticEnergy = 0.5f*mass*velocity.magnitude()*velocity.magnitude();
    real potentialEnergy = mass*9.81f*position.y;
    real totalEnergy = kineticEnergy + potentialEnergy; 

    // if(position.y>0){
    // std::cout << "Kinetic Energy = " << kineticEnergy << std::endl;
    // std::cout << "Potential Energy = " << potentialEnergy << std::endl;
    // std::cout << "Total Energy = " << totalEnergy << std::endl;
    // std::cout << "position = " << position.y << " " << position.z << std::endl; 
    // std::cout << "velocity = " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl; 
    // std::cout << "acceleration = " <<resultingAcc.x << " " <<resultingAcc.y << " " << resultingAcc.z << std::endl;
    // }

    clearAccumulator();
}

void Particle::setMass(const real m){
    mass = m;
    inverseMass = 1/m;
}

real Particle::getMass(){
    return mass;
}

real Particle::getInverseMass(){
    return inverseMass;
}

bool Particle::hasFiniteMass(){
    if(inverseMass==0){
        return false;
    }
    else{
        return true;
    }
}

void Particle::setVolume(const real v){
    volume = v;
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

Vector3 Particle::getPosition(){
    return position;
}

void Particle::getPosition(Vector3* v){
    *v = position;
}

void Particle::setVelocity(const Vector3 &vel){
    velocity = vel;
}

Vector3 Particle::getVelocity(){
    return velocity;
}

void Particle::getVelocity(Vector3* v){
    *v = velocity;
}

void Particle::setAcceleration(const Vector3 &acc){
    acceleration = acc;
}

Vector3 Particle::getAcceleration(){
    return acceleration;
}

void Particle::getAcceleration(Vector3* v){
    *v = acceleration;
}

void Particle::setDamping(const real d){
    damping = d;
}