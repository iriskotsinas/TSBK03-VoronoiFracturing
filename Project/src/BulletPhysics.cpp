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
    //Remove the rigidbodies from the dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject(obj);

        delete obj;
    }

    //Delete collision shapes
    for (int i = 0; i < m_collisionShapes.size(); i++)
    {
        if (m_collisionShapes[i] == 0)
            continue;

        btCollisionShape* shape = m_collisionShapes[i];
        if (shape == nullptr)
            continue;

        m_collisionShapes[i] = 0;
        delete shape;
    }

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;

    m_collisionShapes.clear();
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
        glm::vec4 pos = geometry->getGeometryWorldPosition();
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
        glm::vec4 pos = geometry->getGeometryWorldPosition();
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