#include "Scene.h"
#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
//	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		// #include <stdio.h>
		#include <GL/gl.h>
		// #include "MicroGlut.h"
		// #include <GL/glut.h>
	#endif
#endif

Scene::Scene()
{
    physicsWorld = new BulletPhysics(-5.82f);
}
Scene::~Scene(){
    for(auto g : mGeometries){
        g->~Geometry();
    }
    delete physicsWorld;
}
void Scene::initialize()
{
    mCamera.projectionMatrix = glm::perspective(
        mCamera.fov,          // field of view, 45.0
        mCamera.aspectRatio,  // 16/9
        0.1f,                // Near clipping plane
        100.0f);             // far clipping plane

    // Background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());

    std::cout<<"Scene initialized..."<<std::endl;
}

void Scene::setCameraZoom(float x, float y)
{
    if ((mCamera.zoom - y / 5.0f) > 0.1f)
        mCamera.zoom -= y / 5.0f;
}

void Scene::addGeometry(Geometry *G, float mass, unsigned int type)
{
    G->initialize();
    G->setType(type);
    G->calculateGeometryWorldPosition();
    mGeometries.push_back(G);
    physicsWorld->addRigidBody(G, mass, type);
}

void Scene::setCameraRotation(float x, float y)
{
    if (!isPressed)
    {
        return; 
    }
    glm::vec3 p;

    p.y = x - prevX;
    p.x = -(prevY-y);
    p.z = 0.0f;

    mCamera.orientation = glm::rotate(mCamera.viewMatrix, glm::sqrt(p.x*p.x + p.y*p.y) / 100.0f, p);
    prevX = x;
    prevY = y;
}

void Scene::render()
{
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    mCamera.viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f+mCamera.zoom),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::mat4_cast(mCamera.orientation);

    glm::mat4 modelMatrix;
    // render Geometries in scene
    for (std::vector<Geometry*>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
    {
        modelMatrix = (*it)->getTransMat();
        // MVP matrix
        mSceneMatrices[I_MVP] = toMatrix4x4Row(mCamera.projectionMatrix * mCamera.viewMatrix * modelMatrix);
        // Modelview Matrix
        mSceneMatrices[I_MV] = toMatrix4x4Row(mCamera.viewMatrix * modelMatrix);

        (*it)->render(mSceneMatrices);
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}
void Scene::stepSimulation(){
    btTransform physTransMat;
    btQuaternion physRot;
    btVector3 physRotAxis;

    physicsWorld->stepSimulation();
    
    for(unsigned int i = 0; i < mGeometries.size(); i++) {
        if(mGeometries[i]->getType() == 1) {
            physicsWorld->getRigidBodyAt(i)->getMotionState()->getWorldTransform(physTransMat);
            float glTransArr[16];
            physTransMat.getOpenGLMatrix(glTransArr);
            mGeometries[i]->setTransMat(glm::make_mat4(glTransArr));
        }
    }

}
glm::mat4x4 Scene::toMatrix4x4Row(glm::mat4 m)
{
    return glm::mat4x4
    {
        {m[0][0], m[0][1], m[0][2], m[0][3]},
        {m[1][0], m[1][1], m[1][2], m[1][3]},
        {m[2][0], m[2][1], m[2][2], m[2][3]},
        {m[3][0], m[3][1], m[3][2], m[3][3]}
    };
}


glm::mat4x4 Scene::toMatrix4x4Column(glm::mat4 m)
{
    return glm::mat4x4
    {
        {m[0][0], m[1][0], m[2][0], m[3][0]},
        {m[0][1], m[1][1], m[2][1], m[3][1]},
        {m[0][2], m[1][2], m[2][2], m[3][2]},
        {m[0][3], m[2][3], m[2][3], m[3][3]}
    };
}

glm::mat4x4 Scene::toMatrix4x4(glm::mat3 m)
{
    return glm::mat4x4
    {
        {m[0][0], m[0][1], m[0][2], 0.0f},
        {m[1][0], m[1][1], m[1][2], 0.0f},
        {m[2][0], m[2][1], m[2][2], 0.0f},
        {0.0f   , 0.0f   , 0.0f   , 1.0f}
    };
}

void Scene::resetCamera()
{
    mCamera.fov = 45.0f;
    mCamera.aspectRatio = 16.0f / 9.0f;
    mCamera.zoom = 5.f;
    mCamera.viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f+mCamera.zoom),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::mat4_cast(mCamera.orientation);

    mCamera.projectionMatrix = glm::perspective(
            mCamera.fov,          // field of view, 45.0
            mCamera.aspectRatio,  // 16/9
            0.1f,                // Near clipping plane
            100.0f);             // far clipping plane

    glm::quat identityQuat = glm::quat();
    mCamera.orientation = identityQuat;
}
void Scene::deleteGeometryByName(std::string name){
    for(unsigned int i = 0; i < mGeometries.size(); i++){
        if(mGeometries[i]->getName() == name){
            delete mGeometries[i];
            mGeometries.erase(mGeometries.begin() + i);
            physicsWorld->deleteRigidBodyAt(i);
        }
    }

}