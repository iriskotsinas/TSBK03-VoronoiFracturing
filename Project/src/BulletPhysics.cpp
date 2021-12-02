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
    
    for (auto v : geometry->getUniqueVerts())
    {
        bConvex->addPoint(btVector3(v.x, v.y, v.z), false);
    }
    float margin = 0;
    bConvex->recalcLocalAabb();
    bConvex->setMargin(margin);
    btShapeHull* hull = new btShapeHull(bConvex);
    hull->buildHull(margin);

    shape = bConvex;
    
    if (type == 0) // Static object
    {
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0], pos[1], pos[2])));
    } else if (type == 1)
    { // Dynamic object
        shape->calculateLocalInertia(mass, inertia);
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos[0]/50.0f, pos[1]/50.0f, pos[2]/50.0f)));
    }
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, motionState, shape, inertia);
    btRigidBody *body = new btRigidBody(shapeRigidBodyCI);
    body->setRestitution(0);

    // Add rigid body to dynamic world
    m_dynamicsWorld->addRigidBody(body);
    m_rigidBodiesList.push_back(body);

    delete hull;
}
void BulletPhysics::deleteRigidBodyAt(unsigned int i){
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
    btRigidBody* body = btRigidBody::upcast(obj);

    if (body && body->getMotionState()) {
        delete body->getMotionState();
    }

    m_dynamicsWorld->removeCollisionObject( obj );
    delete obj;

    m_rigidBodiesList.erase(m_rigidBodiesList.begin()+i);
    m_rigidBodiesList.shrink_to_fit();
}
void BulletPhysics::stepSimulation()
{
    //workaround for allowing pausing and resetting the scene
    m_dynamicsWorld->stepSimulation(0.01, 10);

    //more accurate
    // double deltaT = glfwGetTime() - prevTime;
    // prevTime = glfwGetTime();
    //m_dynamicsWorld->stepSimulation(deltaT, 10);
}
void BulletPhysics::applyForce(glm::vec3 pos, float power){
    for(auto b : m_rigidBodiesList){
        glm::vec3 centralMassPos = glm::vec3(b->getCenterOfMassPosition()[0], b->getCenterOfMassPosition()[1], b->getCenterOfMassPosition()[2]);
        glm::vec3 forceDir = glm::normalize(centralMassPos - pos);
        float distance = glm::length(centralMassPos - pos);
        glm::vec3 force = (forceDir / distance) * power;
        b->applyCentralImpulse(btVector3(force.x, force.y, force.z));
    }
}