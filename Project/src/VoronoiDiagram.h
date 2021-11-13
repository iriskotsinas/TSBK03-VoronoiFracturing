#pragma once


#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"

#include <glm/glm.hpp>
#include <list>
#include <random>
#include <iostream>
#include <string.h>
#include "Debug.h"
#include "Geometry.h"

class VoronoiDiagram : public Geometry
{
    public:
        VoronoiDiagram(Geometry* mesh);
        ~VoronoiDiagram(){
            jcv_diagram_free(&diagram);
        }
        void samplePoints(unsigned int n);
        void fracture();
        void initialize(glm::vec3);
        void render(std::vector<glm::mat4x4> sceneMatrices);
        
    private:
        std::pair< float, float> x,y;
        std::vector<jcv_point> points;
        std::vector<glm::vec3> orderedEdgePoints;
        Geometry* mesh;
        jcv_diagram diagram;
};
