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

    // Create rigid body from vertex list
    if (type == 0) // Static object
    {
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

        shape = new btConvexHullShape((const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
        glm::vec3 pos = geometry->getWorldPosition();
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0], pos[1], pos[2])));

    } else if (type == 1) { // Dynamic object
        btConvexHullShape *bConvex = new btConvexHullShape();

        for (auto v : geometry->getVerts())
        {
            bConvex->addPoint(btVector3(v.x, v.y, v.z), false);
        }

        bConvex->recalcLocalAabb();
        bConvex->setMargin(0.04);
        btShapeHull* hull = new btShapeHull(bConvex);
        btScalar margin = bConvex->getMargin();
        hull->buildHull(margin);

        shape = bConvex;
        glm::vec3 pos = geometry->getWorldPosition();
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0]/20.0f, pos[1]/20.0f, pos[2]/20.0f)));
        shape->calculateLocalInertia(mass, inertia);
    }  else {
        std::cout << "Something went wrong when adding rigid body" << std::endl;
    }

    // Adjust parameters for the object
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, motionState, shape, inertia);
    btRigidBody *body = new btRigidBody(shapeRigidBodyCI);

    body->setRestitution(0.4);

    // Add rigid body to dynamic world
    m_dynamicsWorld->addRigidBody(body);
    m_rigidBodiesList.push_back(body);
}

void BulletPhysics::stepSimulation(glm::mat4x4 MVP) {
    
    double deltaT = glfwGetTime() - prevTime;
    // std::cout<<"time: "<<deltaT<<std::endl;
    m_dynamicsWorld->stepSimulation(deltaT, 10);
    prevTime = glfwGetTime();

    // int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
    // std::cout<<numManifolds<<std::endl;
    // for (int i=0;i<numManifolds;i++) {

    //     btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    //     const btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
    //     const btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

    //     int numContacts = contactManifold->getNumContacts();
        
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