#pragma once

#include "Geometry.h"
#include "Debug.h"
#include <vector>
class Plane : public Geometry {
    public:
        Plane(glm::vec3 pos);
        ~Plane();
        void initialize(glm::vec3);
        void render(std::vector<glm::mat4x4>);
        void generatePlane(float width, float height);
        void generateCube(float width, float height, float depth);
    private:
        glm::vec3 pos;
};