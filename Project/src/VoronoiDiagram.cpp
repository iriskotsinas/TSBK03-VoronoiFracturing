#include "VoronoiDiagram.h"
#include <random>
#include "iostream"
void VoronoiDiagram::SamplePoints(unsigned int n){

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist1(0, x);
    std::uniform_real_distribution<> dist2(0, y);
    for (unsigned int i = 0; i < n; i++)
    {
        glm::vec2 point = glm::vec2(dist1(e2), dist2(e2));
        points.push_back(point);
    }
    
}