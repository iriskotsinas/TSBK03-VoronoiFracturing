#pragma once

#include <glm/glm.hpp>
#include "Debug.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "iostream"
#include "vector"
#include "Shader.h"
// #include "VertexArray.h"
// #include "VertexBuffer.h"
// #include "IndexBuffer.h"
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
struct Face;
struct Vertex;

class Geometry{
     protected:
        struct HalfEdge {
            HalfEdge();
            Vertex* vert;  // index into mVerts (the origin vertex)
            Face* face;  // index into mFaces
            HalfEdge* next;  // index into mEdges
            HalfEdge* prev;  // index into mEdges
            Face* pair;  // index into mEdges
        };
         struct Vertex {
            Vertex(const glm::vec3& p = glm::vec3(0,0,0),
            const glm::vec3& n = glm::vec3(0,0,0))
            : pos(p),
            normal(n) {}
            glm::vec3 pos;
            glm::vec3 normal;
            HalfEdge* edge;     // index into mEdges
        };
        struct Face {
            Face(const glm::vec3& n = glm::vec3(0,0,0)) 
            : normal(n) {}
            glm::vec3 normal;
            HalfEdge* edge; //!< index into mEdges
        };
        // The edges of the mesh
        std::vector<HalfEdge> mEdges;
        // The vertices in the mesh
        std::vector<glm::vec3> mVerts;
        // The faces in the mesh
        std::vector<Face> mFaces;

        // Vertex list in drawing order
        std::vector< glm::vec3> mOrderedVertexList;
        glm::mat4 mTransMat;
        std::string mObjName;
        // Shader data
        GLuint vertexArrayID;
        GLuint vertexBuffer;
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
        Geometry(){};
        virtual ~Geometry() = default;

        virtual void initialize(glm::vec3) = 0;

        virtual void render(std::vector<glm::mat4x4>) = 0;

        glm::mat4 getTransMat() { return mTransMat; }

        virtual void addVertex(glm::vec3 v) { mVerts.push_back(v); }
        void setName(std::string s){
            mObjName = s;
        }
};