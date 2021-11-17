#pragma once

#include <glm/glm.hpp>
#include "Debug.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "iostream"
#include "vector"
#include "Shader.h"
#include <limits>
#include <glm/gtc/matrix_transform.hpp>
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
            Vertex* vert;
            Face* face; 
            HalfEdge* next;  
            HalfEdge* prev; 
            Face* pair;  
        };
        struct Vertex {
            Vertex(const glm::vec3& p = glm::vec3(0,0,0),
            const glm::vec3& n = glm::vec3(0,0,0))
            : pos(p),
            normal(n) {}
            glm::vec3 pos;
            glm::vec3 normal;
            HalfEdge* edge;    
        };
        struct Face {
            Face(const glm::vec3& n = glm::vec3(0,0,0)) 
            : normal(n) {}
            glm::vec3 normal;
            HalfEdge* edge; 
        };
        bool debug = false;
        std::vector<glm::vec3> orderedEdgePoints;
    // The vertices in the mesh
        std::vector<glm::vec3> mVerts;
        // Vertex list in drawing order
        std::vector< glm::vec3> mOrderedVertexList;
        std::vector< glm::vec4> mOrderedColorList;
        glm::mat4 mTransMat;
        std::string mObjName;
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
        Geometry(){};
        virtual ~Geometry() = default;

        virtual void initialize(glm::vec3) = 0;

        virtual void render(std::vector<glm::mat4x4>) = 0;

        glm::mat4 getTransMat() { return mTransMat; }
        void setTransMat(glm::mat4 model) { 
            mTransMat = model; 
        }

        void addVertex(glm::vec3 v) { mVerts.push_back(v);}
        void setName(std::string s){
            mObjName = s;
        }
        int getVertCount(){
            return mVerts.size();
        }
        std::vector<glm::vec3> getVerts(){
            return mVerts;
        }
        void setBoundaries(std::pair< float, float> &x, std::pair< float, float> &y){
            float min_x = std::numeric_limits<float>::infinity();
            float max_x = std::numeric_limits<float>::min();
            float min_y = std::numeric_limits<float>::infinity();
            float max_y = std::numeric_limits<float>::min();
            for (auto v : mVerts)
            {
                if(v[0] < min_x){
                    min_x = v[0];
                }
                if(v[0] > max_x){
                    max_x = v[0];
                }
                if(v[1] < min_y){
                    min_y = v[1];
                }
                if(v[1] > max_y){
                    max_y = v[1];
                }
            }
            x.first = min_x;
            x.second = max_x;
            y.first = min_y;
            y.second = max_y;
            
        }
};