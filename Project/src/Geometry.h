#include <glm/glm.hpp>
struct Region;
struct HalfEdge;
struct Face
{
    Region* region;
    HalfEdge* outerComponent;
};

struct Region
{
    unsigned int index;
    glm::vec2 point;
    Face* face;
};
struct HalfEdge
{
    Vertex* origin;
    Vertex* destination;
    HalfEdge* pair;
    Face* incidentFace;
    HalfEdge* prev;
    HalfEdge* next;
};

struct Vertex
{
    glm::vec2 point;
};

