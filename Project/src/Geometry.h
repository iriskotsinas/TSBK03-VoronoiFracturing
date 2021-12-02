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
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		#include <GL/gl.h>
	#endif
#endif
#define I_MVP 0
#define I_MV 1

class Geometry{
     protected:
        unsigned int type = 0;
        std::vector<glm::vec3> mUniqueVertexList;
        std::vector<glm::vec3> mOrderedVertexList;
        std::vector<glm::vec4> mOrderedColorList;
        std::vector<glm::vec3> mOrderedNormalList;

        glm::mat4 mTransMat;
        std::string mObjName;

        glm::vec4 mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 mWorldPosition;

        GLuint vertexArrayID;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint normalBuffer;
        GLuint shaderProgram;

        Shader shader;
        GLint MVPLoc;        
        GLint MVLoc;   

    public:
        Geometry() {};
        virtual ~Geometry() = default;
        virtual void initialize();
        void render(std::vector<glm::mat4x4>);
        
        void addVertex(glm::vec3 v) { mOrderedVertexList.push_back(v); }
        void addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
        void addColor(glm::vec4 c) { mOrderedColorList.push_back(c); }
        void addNormal(glm::vec3 n) { mOrderedNormalList.push_back(n); }

        void setColor(glm::vec4 c) { mColor = c; }
        void setType(const unsigned int t){type = t;}
        void setTransMat(glm::mat4 model){ mTransMat = model; }
        void setName(std::string s){mObjName = s;}

        unsigned int getType(){return type;}
        glm::mat4 getTransMat() { return mTransMat; }
        std::string getName(){return mObjName;}
        int getVertCount() const{return mOrderedVertexList.size();}
        int getUniqueVertCount() const{return mUniqueVertexList.size();}
        std::vector<glm::vec3> getVerts() const{return mOrderedVertexList;}
        std::vector<glm::vec3> getUniqueVerts() const{return mUniqueVertexList;}
        glm::vec3 getWorldPosition() const{return mWorldPosition;}

        glm::vec3 calculateNormal(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2);
        void calculateGeometryWorldPosition();
        void setBoundaries(std::pair< float, float> &x, std::pair< float, float> &y);

        //Matrix operations
        glm::mat4x4 toMatrix4x4Row(glm::mat4 m);
        glm::mat4x4 toMatrix4x4Column(glm::mat4 m);
        glm::mat4x4 toMatrix4x4(glm::mat3 m) const;
};