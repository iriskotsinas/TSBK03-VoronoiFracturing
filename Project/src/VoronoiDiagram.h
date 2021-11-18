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

class VoronoiDiagram
{
    public:
        VoronoiDiagram(Geometry* mesh);
        ~VoronoiDiagram()
        {
            jcv_diagram_free(&diagram);
        }
        void samplePoints(unsigned int n);
        std::vector<Geometry*> fracture();
        void buildLines();
        void buildTriangles();
        // glm::vec3 enforceBoundaries(glm::vec3 p);
    private:
        std::pair< float, float> x,y;
        std::vector<jcv_point> points;
        Geometry* mesh;
        jcv_diagram diagram;
};
