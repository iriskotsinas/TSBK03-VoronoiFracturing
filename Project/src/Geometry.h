#pragma once
#include <glm/glm.hpp>
#include "Debug.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <set>
#include "Shader.h"
#include <limits>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
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
#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

class Geometry{
     protected:
        bool debug = false;
        unsigned int type = 0;
        std::vector<glm::vec3> orderedEdgePoints;

        std::vector<glm::vec3> mUniqueVertexList;
        std::vector<glm::vec3> mOrderedVertexList;
        std::vector<glm::vec3> mTransformedVertexList;
        std::vector<glm::vec4> mOrderedColorList;
        std::vector<glm::vec3> mOrderedNormalList;

        glm::mat4 mTransMat;
        std::string mObjName;

        glm::vec4 mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 mWorldPosition;
    // Shader data
        GLuint vertexArrayID;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint normalBuffer;
        GLuint shaderProgram;

        Shader shader;
        // Shader indices for Matrices
        GLint MVPLoc;           // MVP matrix
        GLint MVLoc;            // MV matrix
        GLint MVLightLoc;       // MVLight matrix
        GLint NMLoc;            // NM matrix
        GLint lightPosLoc;      // Light position
        GLint colorLoc;         // Color
        GLint lightAmbLoc;      // Ambient light
        GLint lightDifLoc;      // Diffuse light
        GLint lightSpeLoc;      // Specular light
        GLint specularityLoc;   // Specular constant
        GLint shinynessLoc;     // How much specularity (magnitude)

    public:
        Geometry() {};
        virtual ~Geometry() = default;

        virtual void initialize(glm::vec3) = 0;

        virtual void render(std::vector<glm::mat4x4>) = 0;
        void addVertex(glm::vec3 v) { mOrderedVertexList.push_back(v); }
        void addColor(glm::vec4 c) { mOrderedColorList.push_back(c); }
        void addNormal(glm::vec3 n) { mOrderedNormalList.push_back(n); }
        void setType(const unsigned int t){type = t;}
        void updateMesh(glm::mat4 transMat){
            // mTransformedVertexList.clear();
            // mTransformedVertexList.shrink_to_fit();
            mTransMat = transMat;
            // std::cout<<"transMat"<<transMat[0][1]<<std::endl;
            // glm::mat4x4 transformMatrix = toMatrix4x4(mTransMat);
            // // std::cout<<"size: "<<mOrderedVertexList.size()<<std::endl;
            // for(unsigned int i = 0; i < mOrderedVertexList.size(); i++) {
            //     // Apply the rotation to the vertices
            //     glm::vec4 v = transformMatrix * glm::vec4(mOrderedVertexList[i], 1.0f);
            //     mTransformedVertexList[i] = (glm::vec3(v[0], v[1], v[2]));
            // }    
            // mOrderedVertexList = mTransformedVertexList;
            
        }
        unsigned int getType(){return type;}
        glm::mat4x4 toMatrix4x4Row(glm::mat4 m)
        {
            return glm::mat4x4
                    {
                            {m[0][0], m[0][1], m[0][2], m[0][3]},
                            {m[1][0], m[1][1], m[1][2], m[1][3]},
                            {m[2][0], m[2][1], m[2][2], m[2][3]},
                            {m[3][0], m[3][1], m[3][2], m[3][3]}
                    };
        }

        glm::mat4x4 toMatrix4x4Column(glm::mat4 m)
        {
            return glm::mat4x4
                    {
                            {m[0][0], m[1][0], m[2][0], m[3][0]},
                            {m[0][1], m[1][1], m[2][1], m[3][1]},
                            {m[0][2], m[1][2], m[2][2], m[3][2]},
                            {m[0][3], m[2][3], m[2][3], m[3][3]}
                    };
        }

        glm::mat4x4 toMatrix4x4(glm::mat3 m) const
        {
            return glm::mat4x4
                    {
                            {m[0][0], m[0][1], m[0][2], 0.0f},
                            {m[1][0], m[1][1], m[1][2], 0.0f},
                            {m[2][0], m[2][1], m[2][2], 0.0f},
                            {0.0f   , 0.0f   , 0.0f   , 1.0f}
                    };
        }

        glm::vec3 calculateNormal(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) {
            glm::vec3 edge0 = v1 - v0;
            glm::vec3 edge1 = v2 - v0;

            return glm::normalize(glm::cross(edge0, edge1));
        }

        glm::mat4 getTransMat() { return mTransMat; }
        void setTransMat(glm::mat4 model)
        {
            mTransMat = model; 
        }

        void setName(std::string s)
        {
            mObjName = s;
        }

        int getVertCount() const
        {
            return mOrderedVertexList.size();
        }
        int getUniqueVertCount() const
        {
            return mUniqueVertexList.size();
        }

        std::vector<glm::vec3> getVerts() const
        {
            return mOrderedVertexList;
        }
        std::vector<glm::vec3> getUniqueVerts() const
        {
            return mUniqueVertexList;
        }

        void setBoundaries(std::pair< float, float> &x, std::pair< float, float> &y)
        {
            float min_x = std::numeric_limits<float>::infinity();
            float max_x = std::numeric_limits<float>::min();
            float min_y = std::numeric_limits<float>::infinity();
            float max_y = std::numeric_limits<float>::min();
            for (auto v : mOrderedVertexList)
            {
                if(v[0] < min_x)
                {
                    min_x = v[0];
                }
                if(v[0] > max_x)
                {
                    max_x = v[0];
                }
                if(v[1] < min_y)
                {
                    min_y = v[1];
                }
                if(v[1] > max_y)
                {
                    max_y = v[1];
                }
            }
            x.first = min_x;
            x.second = max_x;
            y.first = min_y;
            y.second = max_y;
        }

        void rotateX(float radians)
        {
            mTransMat = glm::rotate(mTransMat, radians, glm::vec3(1.0f, 0.0f, 0.0f));
            
        }

        void calculateGeometryWorldPosition()
        {
            glm::vec4 tmpPos = glm::vec4(0.0f);
            for (auto v : mOrderedVertexList)
            {
              tmpPos += glm::vec4(v, 1.0f);
            }
            tmpPos = toMatrix4x4(glm::inverse(mTransMat)) * tmpPos;
            tmpPos /= mOrderedVertexList.size();
            

            mWorldPosition = glm::vec3(tmpPos[0], tmpPos[1], tmpPos[2]);
        }
        glm::vec3 getWorldPosition() const {
            return mWorldPosition;
        }
        void addTriangle(glm::vec3 v0, glm::vec3 v1,glm::vec3 v2){
            
            if(std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v0) == mUniqueVertexList.end()){
                mUniqueVertexList.push_back(v0);
            }
            if(std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v1) == mUniqueVertexList.end()){
                mUniqueVertexList.push_back(v1);
            }
            if(std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v2) == mUniqueVertexList.end()){
                mUniqueVertexList.push_back(v2);
            }

            addVertex(v0);
            addVertex(v1);
            addVertex(v2);

            glm::vec3 normal0 = calculateNormal(v0, v1, v2);
            addNormal(normal0);
            addNormal(normal0);
            addNormal(normal0);
            // std::cout<<"x: "<<normal0.x<<", y: "<<normal0.y<<", z: "<<normal0.z<<" size: "<<mOrderedVertexList.size()<<std::endl;
            addColor(mColor);
            addColor(mColor);
            addColor(mColor);
        }
};