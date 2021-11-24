#include "BulletPhysics.h"

BulletPhysics::BulletPhysics(float gravity)
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, gravity, 0));
}

BulletPhysics::~BulletPhysics()
{

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

void BulletPhysics::addRigidBody(const Geometry *geometry, const float mass, const unsigned int type)
{
    btCollisionShape* shape;
    btMotionState* motionState;
    btVector3 inertia(0, 0, 0);
    glm::vec3 pos = geometry->getWorldPosition();
    btConvexHullShape *bConvex = new btConvexHullShape();
    for (auto v : geometry->getVerts())
    {
        bConvex->addPoint(btVector3(v.x, v.y, v.z), false);
    }

    bConvex->recalcLocalAabb();
    bConvex->setMargin(0.04); // padding
    btShapeHull* hull = new btShapeHull(bConvex);
    btScalar margin = bConvex->getMargin();
    hull->buildHull(margin);

    shape = bConvex;
    
    if (type == 0) // Static object
    {
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0], pos[1], pos[2])));

    } else if (type == 1) { // Dynamic object

        shape->calculateLocalInertia(mass, inertia);
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0]/50.0f, pos[1]/50.0f, pos[2]/50.0f)));
       
    }
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, motionState, shape, inertia);
    btRigidBody *body = new btRigidBody(shapeRigidBodyCI);
    body->setRestitution(0.5);
    // body->setFriction(btScalar(5.0f));
    // Add rigid body to dynamic world
    m_dynamicsWorld->addRigidBody(body);
    m_rigidBodiesList.push_back(body);
    
}

void BulletPhysics::stepSimulation() {
    
    double deltaT = glfwGetTime() - prevTime;
    // std::cout<<"time: "<<deltaT<<std::endl;
    m_dynamicsWorld->stepSimulation(deltaT, 10);
    prevTime = glfwGetTime();

    // int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
    // for (int i=0;i<numManifolds;i++) {

    //     btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    //     const btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
    //     const btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

    //     int numContacts = contactManifold->getNumContacts();
        
    //     std::cout<<"collisions: "<<numContacts<<std::endl;
    //     for (int j=0;j<numContacts;j++) {
            
    //         btManifoldPoint& pt = contactManifold->getContactPoint(j);
            
    //         if (pt.getDistance()<0.f) {
            
    //             const btVector3& ptA = pt.getPositionWorldOnA();
    //             const btVector3& ptB = pt.getPositionWorldOnB();
    //             const btVector3& normalOnB = pt.m_normalWorldOnB;
    //         }
    //     }
    // }
}