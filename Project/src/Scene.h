#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Geometry.h"
#include "BulletPhysics.h"
#include <vector>
#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

class Scene
{
public:
    Scene();

    ~Scene();

    void initialize();
    
    void render();

    void addGeometry(Geometry *G, float mass, unsigned int type);

    void setCameraRotation(float x, float y);
    void setCameraZoom(float x, float y);

    void setWindowPressed(float x, float y)
    {
        isPressed = true;
        prevX = x;
        prevY = y;
    }

    void setWindowReleased()
    {
        isPressed = false;
    }
    void stepSimulation();
    void resetCamera();
    void deleteGeometryByName(std::string name);
    void applyForce(glm::vec3 pos, float power){
        physicsWorld->applyForce(pos, power);
    }

private:
    bool isPressed = false;
    float prevX = 0.0f, prevY = 0.0f;
    glm::mat4x4 toMatrix4x4Row(glm::mat4);

    glm::mat4x4 toMatrix4x4Column(glm::mat4);
    
    glm::mat4x4 toMatrix4x4(glm::mat3);

    std::vector<Geometry*>mGeometries;
    
    std::vector<glm::mat4x4>mSceneMatrices;

    BulletPhysics* physicsWorld;

    struct LightSource
    {
        glm::vec3 color;
        glm::vec3 position;
    } mPointLight;

    struct cameraHandler
    {
        float fov = 45.0f;
        float aspectRatio = 16.0f / 9.0f;
        float zoom = 5.f;
        glm::quat orientation = glm::quat();
        glm::mat4 projectionMatrix = glm::mat4();
        glm::mat4 viewMatrix = glm::mat4();
    } mCamera;
};
