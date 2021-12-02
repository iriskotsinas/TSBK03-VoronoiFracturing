        #include "Shape.h"

        Shape::Shape(glm::vec3 _pos, std::string name)
            :pos{_pos}
        {
            setName(name);
            mTransMat = glm::translate(glm::mat4(1.0f), pos);
        }

        Shape::~Shape(){ }
        void Shape::generatePlaneXY(float width, float height)
        {
            glm::vec3 v0 = {pos.x-width, pos.y-height, pos.z};
            glm::vec3 v1 = {pos.x+width, pos.y+height, pos.z};
            glm::vec3 v2 = {pos.x-width, pos.y+height, pos.z};
            glm::vec3 v3 = {pos.x+width, pos.y-height, pos.z};

            addTriangle(v0,v1,v2);
            addTriangle(v0,v3,v1);

            
        }
        void Shape::generatePlaneXZ(float width, float height)
        {
            glm::vec3 v0 = {pos.x-width, pos.y, pos.z-height};
            glm::vec3 v1 = {pos.x+width, pos.y, pos.z+height};
            glm::vec3 v2 = {pos.x-width, pos.y, pos.z+height};
            glm::vec3 v3 = {pos.x+width, pos.y, pos.z-height};

            addTriangle(v2,v1,v0);
            addTriangle(v1,v3,v0);
            
        }
        void Shape::generateCube(float width, float height, float depth)
        {
            glm::vec3 v0 = {pos.x-width, pos.y-height, pos.z };
            glm::vec3 v1 = {pos.x+width, pos.y+height, pos.z };
            glm::vec3 v2 = {pos.x-width, pos.y+height, pos.z };
            glm::vec3 v3 = {pos.x+width, pos.y-height, pos.z };

            glm::vec3 v4 = {pos.x-width, pos.y-height, pos.z - depth};
            glm::vec3 v5 = {pos.x+width, pos.y+height, pos.z - depth};
            glm::vec3 v6 = {pos.x-width, pos.y+height, pos.z - depth};
            glm::vec3 v7 = {pos.x+width, pos.y-height, pos.z - depth};

            //Front
            addTriangle(v0,v1,v2);
            addTriangle(v0,v3,v1);

            //Back
            addTriangle(v6,v5,v4);
            addTriangle(v5,v7,v4);
            
            //Left Side
            addTriangle(v0,v2,v4);
            addTriangle(v2,v6,v4);

            //Right Side
            addTriangle(v3,v7,v1);
            addTriangle(v7,v5,v1);

            //Top Side
            addTriangle(v1,v5,v2);
            addTriangle(v2,v5,v6);

            //Bottom Side
            addTriangle(v0,v7,v3);
            addTriangle(v0,v4,v7);

        }