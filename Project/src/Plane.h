#pragma once

#include "Geometry.h"
#include "Debug.h"
#include <vector>
class Plane : public Geometry {
    public:
        Plane(float width, float height, glm::vec3 pos);
        ~Plane();
        void initialize(glm::vec3);
        void render(std::vector<glm::mat4x4>);
        void generatePlane(float width, float height, glm::vec3 pos);
};