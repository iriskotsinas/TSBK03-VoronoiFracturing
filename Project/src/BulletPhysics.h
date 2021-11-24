#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "Geometry.h"
#include <GLFW/glfw3.h>
class BulletPhysics {
public:
    BulletPhysics(float gravity);
    ~BulletPhysics();
    void addRigidBody(const Geometry* geometry, const float mass, const unsigned int type);
    void stepSimulation();
    btRigidBody* getRigidBodyAt(unsigned int i){return m_rigidBodiesList[i];}
private:
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

    std::vector<btRigidBody*> m_rigidBodiesList;
    int m_counter = 0;
    bool m_setGravity = false;
    btVector3 m_boxSize;
    double prevTime;
};

