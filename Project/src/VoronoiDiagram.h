#pragma once
#include <glm/glm.hpp>
#include <list>

class VoronoiDiagram
{
    public:
        VoronoiDiagram(unsigned int _x, unsigned int _y)
            :x{_x}, y{_y}{};
        ~VoronoiDiagram() = default;
        void SamplePoints(unsigned int n);

    private:
        unsigned int x,y;
        std::list<glm::vec2> points;

};
