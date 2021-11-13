#pragma once


#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"

#include <glm/glm.hpp>
#include <list>
#include <random>
#include <iostream>
#include <string.h>

#include "Geometry.h"

class VoronoiDiagram
{
    public:
        VoronoiDiagram(Geometry* mesh);
        ~VoronoiDiagram() = default;
        void samplePoints(unsigned int n);
        void fracture();
    private:
        std::pair< float, float> x,y;
        std::vector<jcv_point> points;
        Geometry* mesh;

};
