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
#include "jc_voronoi.h"

#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

class HalfEdgeMesh : public Geometry{
     private:
        // The edges of the mesh
        std::vector<jcv_graphedge*> mEdges;
        glm::vec3 siteCenter;

    public:
        HalfEdgeMesh(std::string s);

        ~HalfEdgeMesh();
        void extrude();
        void generatePlane(float width, float height);
        void initialize() override;
        glm::mat4 getTransMat() { return mTransMat; }
        void buildRenderData();
        void addHalfEdge(const jcv_graphedge* e) {
            mEdges.push_back((jcv_graphedge*)e);
        }
        void setSiteCenter(glm::vec3 s) { siteCenter = s; }
        
};