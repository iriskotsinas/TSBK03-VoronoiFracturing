#pragma once


#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"
// #define JC_VORONOI_CLIP_IMPLEMENTATION
// #include <jc_voronoi_clip.h>

#include <glm/glm.hpp>
#include <list>
#include <random>
#include <iostream>
#include <string.h>
#include "Debug.h"
#include "Geometry.h"
#include "HalfEdgeMesh.h"
#include "Scene.h"

class VoronoiDiagram : public Geometry
{
    public:
        VoronoiDiagram(Geometry* mesh, Scene* s);
        ~VoronoiDiagram()
        {
            jcv_diagram_free(&diagram);
        }
        void samplePoints(unsigned int n);
        void fracture();
        void initialize(glm::vec3);
        void render(std::vector<glm::mat4x4> sceneMatrices);
        void buildLines();
        void buildTriangles();
        // glm::vec3 enforceBoundaries(glm::vec3 p);
    private:
        std::pair< float, float> x,y;
        std::vector<jcv_point> points;
        Geometry* mesh;
        jcv_diagram diagram;
        Scene* scene;
};
