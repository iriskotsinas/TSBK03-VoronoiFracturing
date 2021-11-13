#pragma once

#include "Debug.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Geometry.h"
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

class HalfEdgeMesh : public Geometry{
     private:
        
        // The edges of the mesh
        std::vector<HalfEdge> mEdges;
        // The faces in the mesh
        std::vector<Face> mFaces;
        // Vertex list in drawing order
        std::vector< glm::vec3> mOrderedVertexList;

    public:
        HalfEdgeMesh(std::string s);

        ~HalfEdgeMesh() = default;;
        void generatePlane(float width, float height);
        void initialize(glm::vec3);

        void render(std::vector<glm::mat4x4 >);

        glm::mat4 getTransMat() { return mTransMat; }
        void buildRenderData();

        void addVertex(glm::vec3 v) { mVerts.push_back(v); }
};