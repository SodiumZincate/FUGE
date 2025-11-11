#include "fgen.h"
#include <iostream>

using namespace cyclone;

void ForceRegistry::add(RigidBody* body, ForceGenerator *fg){
    registrations.push_back({body,fg});
}

void ForceRegistry::remove(RigidBody* body, ForceGenerator *fg){
    Registry::iterator i = registrations.begin();
    for(; i!=registrations.end(); i++){
        if(i->body == body && i->fg == fg){
            i = registrations.erase(i);
            std::cout << "REMOVED" << std::endl;
        }
    }
}

void ForceRegistry::clear(){
    registrations.clear();
}

void ForceRegistry::updateForces(real duration){
    Registry::iterator i = registrations.begin();
    for(; i!=registrations.end(); i++){
        i->fg->updateForce(i->body, duration);
    }
}

void Gravity::updateForce(RigidBody *body, real duration){
    if(!body->hasFiniteMass()) return;

    body->addForce(gravity*body->getMass());
}

void Drag::updateForce(RigidBody *body, real duration){
    Vector3 force;
    body->getVelocity(&force);

    real dragCoeff = force.magnitude();
    dragCoeff = k1*dragCoeff + k2*dragCoeff*dragCoeff;

    force.normalize();

    force*= -dragCoeff;
    body->addForce(force);
}

void Spring::updateForce(RigidBody *body, real duration){
    Vector3 bodyAws =  bodyA->getPointInWorldSpace(connectionPointA);
    Vector3 bodyBws = - bodyB->getPointInWorldSpace(connectionPointB);

    Vector3 force = bodyAws - bodyBws;
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    bodyA->addForceAtPoint(force, bodyAws);
    bodyB->addForceAtPoint(force*-1, bodyBws);
}

void AnchoredSpring::updateForce(RigidBody *body, real duration){
    Vector3 bodyws =  body->getPointInWorldSpace(connectionPoint);

    Vector3 force = bodyws;
    force -= *anchor;

    real magnitude = force.magnitude();
    magnitude = (real_abs(magnitude - restLength));
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    body->addForce(force, bodyws);
}

void Bungee::updateForce(RigidBody *body, real duration){
    Vector3 force = bodyA->getPosition() - bodyB->getPosition();

    real magnitude = force.magnitude();
    if(magnitude <= restLength) return;

    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;

    bodyA->addForce(force);
    bodyB->addForce(force*-1);
}

void Buoyancy::updateForce(RigidBody *body, real duration){
    real depth = body->getPosition().y;
    
    // out of water
    if(depth >= waterHeight+body->maxDepth) return;
    Vector3 force(0,0,0);

    // inside water check if max depth
    if(depth <= waterHeight-body->maxDepth){
        force.y = liquidDensity*body->volume;
        body->addForce(force);
        return;
    }

    // partially submerged: clamp to 0..1
    real submergedFraction = (waterHeight + body->maxDepth - depth) / (2 * body->maxDepth);
    if(submergedFraction < 0.0f) submergedFraction = 0.0f;
    if(submergedFraction > 1.0f) submergedFraction = 1.0f;

    force.y = liquidDensity * body->volume * submergedFraction;
    body->addForce(force);
}