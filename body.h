namespace cyclone{
    class RigidBody{
        public:
            real inverseMass;
            Vector3 position;
            Quaternion orientation;
            Vector3 velocity;
            Vector3 rotation;
            Matrix4 transformMatrix;
            Vector3 forceAccum;
            
            void calculateDerivedData();
            Matrix3 inverseInertiaTensor;

            void addForce(const Vector3 &force);
            void integrate(real duration);
            void clearAccumulators();
            void addForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
    }
}