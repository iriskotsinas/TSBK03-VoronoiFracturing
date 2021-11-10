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
void Scene::initialize() {

    // Init the lightsource parameters
    mPointLight.position = glm::vec3(0.0f, 0.0f, 3.0f);
    mPointLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());
    mSceneMatrices.push_back(glm::mat4x4());

    for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
        (*it)->initialize(mPointLight.position);
    std::cout<<"Scene initialized..."<<std::endl;
}
void Scene::addGeometry(Geometry *G) {

    //updateVoronoiPatterns("icosphere", Matrix4x4<float>());
    mGeometries.push_back(G);
    // G->calculateCenterOfMass();
    // physicsWorld->addGeometry(G->getUniqueVertexList(), G->getWorldCenterOfMass() , type, G->volume());

}
void Scene::render() {

    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mCamera.projectionMatrix = glm::perspective(
        mCamera.fov,          // field of view, 45.0
        mCamera.aspectRatio,  // 4/3 atm
        0.1f,                // Near clipping plane
        100.0f);             // far clipping plane

    mCamera.viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f+mCamera.zoom),   
        glm::vec3(0.0f, 0.0f, 0.0f),            
        glm::vec3(0.0f, 1.0f, 0.0f)) *                                 
        glm::mat4_cast(mCamera.orientation); 

    glm::mat4 modelMatrix;
    // render Geometries in scene
    for(std::vector<Geometry*>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it) {

        // The scene modelmatrix is nothing atm, the geometries will have their own model transforms
        modelMatrix = (*it)->getTransMat();

        // Construct MVP matrix
        mSceneMatrices[I_MVP] = toMatrix4x4Row(mCamera.projectionMatrix * mCamera.viewMatrix * modelMatrix);

        // Modelview Matrix, apply camera transforms here as well
        mSceneMatrices[I_MV] = toMatrix4x4Row(mCamera.viewMatrix * modelMatrix);

        // Modelview Matrix for our light
        mSceneMatrices[I_MV_LIGHT] = toMatrix4x4Row(mCamera.viewMatrix * modelMatrix);

        // Normal Matrix, used for normals in our shading
        mSceneMatrices[I_NM] = toMatrix4x4Row(glm::inverseTranspose(glm::mat4(mCamera.viewMatrix * modelMatrix)));

        (*it)->render(mSceneMatrices);
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}
glm::mat4x4 Scene::toMatrix4x4Row(glm::mat4 m) {
    return glm::mat4x4 {
        {m[0][0], m[0][1], m[0][2], m[0][3]},
        {m[1][0], m[1][1], m[1][2], m[1][3]},
        {m[2][0], m[2][1], m[2][2], m[2][3]},
        {m[3][0], m[3][1], m[3][2], m[3][3]}
    };
}


glm::mat4x4 Scene::toMatrix4x4Column(glm::mat4 m) {
    return glm::mat4x4 {
        {m[0][0], m[1][0], m[2][0], m[3][0]},
        {m[0][1], m[1][1], m[2][1], m[3][1]},
        {m[0][2], m[1][2], m[2][2], m[3][2]},
        {m[0][3], m[2][3], m[2][3], m[3][3]}
    };
}

glm::mat4x4 Scene::toMatrix4x4(glm::mat3 m) {
    return glm::mat4x4 {
        {m[0][0], m[0][1], m[0][2], 0.0f},
        {m[1][0], m[1][1], m[1][2], 0.0f},
        {m[2][0], m[2][1], m[2][2], 0.0f},
        {0.0f   , 0.0f   , 0.0f   , 1.0f}
    };
}