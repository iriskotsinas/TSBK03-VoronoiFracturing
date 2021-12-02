#pragma once

#include "Geometry.h"
#include "Debug.h"
#include <vector>
class Shape : public Geometry {
    public:
        Shape(glm::vec3 pos, std::string name);
        ~Shape();
        void generatePlaneXY(float width, float height);
        void generatePlaneXZ(float width, float height);
        void generateCube(float width, float height, float depth);
    private:
        glm::vec3 pos;
};