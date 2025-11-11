#pragma once
#include "precision.h"
#include "core.h"
#include <vector>

class ForceGenerator{
    public:
        virtual void updateForce(RigidBody *p, real duration) = 0;
};

namespace cyclone{
    class RigidBodyForceRegistry{
        protected:
            struct ForceRegistration{
                RigidBody *body;
                ForceGenerator *fg;
            };

            typedef std::vector<ForceRegistration> Registry;
            Registry registrations;

        public:
            void add(RigidBody* body, ForceGenerator *fg);
            void remove(RigidBody* body, ForceGenerator *fg);
            void clear();
            void updateForces(real duration);
    };
}

class Gravity : public ForceGenerator{
    Vector3 gravity;
    public:
        Gravity(const Vector3 &gravity):gravity(gravity){}

        virtual void updateForce(RigidBody *body, real duration);
};

class Drag : public ForceGenerator{
    real k1, k2;
    public:
        Drag(real k1, real k2):k1(k1), k2(k2){}

        virtual void updateForce(RigidBody *body, real duration);
};

class Spring : public ForceGenerator{
    Vector3 connectionPointA;
    Vector3 connectionPointB;
    RigidBody *bodyA;
    RigidBody *bodyB;
    real springConstant;
    real restLength;
    public:
        Spring(Vector3 connectionPointA, Vector3 connectionPointB, 
            RigidBody* bodyA, RigidBody* bodyB, real springConstant, real restLength):
                connectionPointA(connectionPointA), connectionPointB(connectionPointB),
                bodyA(bodyA), bodyB(bodyB), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(RigidBody *bodyA, real duration);
};

class AnchoredSpring : public ForceGenerator{
    Vector3 *anchor;
    Vector3 connectionPoint;
    RigidBody *body;
    real springConstant;
    real restLength;
    public:
        Spring(Vector3* anchor, Vector3 connectionPoint, 
            RigidBody* body, RigidBody* bodyB, real springConstant, real restLength):
                connectionPoint(connectionPoint),
                body(body), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(RigidBody *body, real duration);
};

class Bungee : public ForceGenerator{
    RigidBody *bodyA;
    RigidBody *bodyB;
    real springConstant;
    real restLength;
    public:
        Bungee(RigidBody* bodyA, RigidBody* bodyB, real springConstant, real restLength):
            bodyA(bodyA), bodyB(bodyB), springConstant(springConstant), restLength(restLength){}
        virtual void updateForce(RigidBody *bodyA, real duration);
};

class Buoyancy : public ForceGenerator{
    real waterHeight;
    real liquidDensity;

    public:
        Buoyancy(real waterHeight, real liquidDensity):
            waterHeight(waterHeight), liquidDensity(liquidDensity){}
        virtual void updateForce(RigidBody *body, real duration);
};