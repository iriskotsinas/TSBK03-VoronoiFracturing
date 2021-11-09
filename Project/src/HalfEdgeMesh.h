#pragma once
#include "Debug.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "iostream"
#include "vector"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
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

class HalfEdgeMesh{
     private:
        struct HalfEdge {
            HalfEdge();
            unsigned int vert;  // index into mVerts (the origin vertex)
            unsigned int face;  // index into mFaces
            unsigned int next;  // index into mEdges
            unsigned int prev;  // index into mEdges
            unsigned int pair;  // index into mEdges
        };
         struct Vertex {
            Vertex(const glm::vec3& p = glm::vec3(0,0,0),
            const glm::vec3& n = glm::vec3(0,0,0))
            : pos(p),
            normal(n) {}
            glm::vec3 pos;
            glm::vec3 normal;
            unsigned int edge;     // index into mEdges
        };
        // A face has a normal and an index to an edge
        struct Face {
            Face(const glm::vec3& n = glm::vec3(0,0,0)) 
            : normal(n) {}
            glm::vec3 normal;
            unsigned int edge; //!< index into mEdges
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
        explicit HalfEdgeMesh(std::string s);

        ~HalfEdgeMesh() = default;;
        void generatePlane(float width, float height);
        void initialize(glm::vec3);

        void render(std::vector<glm::mat4x4 >);

        glm::mat4 getTransMat() { return mTransMat; }
        void buildRenderData();

        unsigned int getEdge(glm::vec3 vertPos);
        HalfEdge& getEdge(unsigned int i) { return mEdges.at(i); }
        const HalfEdge& getEdge(unsigned int i) const { return mEdges.at(i); }

        Face& getFace(unsigned int i) { return mFaces.at(i); }
        const Face& getFace(unsigned int i) const { return mFaces.at(i); }

        glm::vec3& getVert(unsigned int i) { return mVerts.at(i); }
        const Vertex getVert(unsigned int i) const { return mVerts.at(i); }

        void addVertex(glm::vec3 v) { mVerts.push_back(v); }
};