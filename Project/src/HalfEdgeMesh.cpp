#include "HalfEdgeMesh.h"

HalfEdgeMesh::HalfEdgeMesh(std::string s)
{
    setName(s);
    mTransMat = glm::mat4(1.0f);
}
HalfEdgeMesh::~HalfEdgeMesh(){
    for(auto e : mEdges){
        delete e;
    }
}
void HalfEdgeMesh::initialize()
{
    buildRenderData();
    Geometry::initialize();
}

void HalfEdgeMesh::buildRenderData()
{
    jcv_graphedge *edge = mEdges[0];
    while (edge)
    {
        glm::vec3 p0 = glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f);
        glm::vec3 p1 = glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f);
        addTriangle(siteCenter, p0, p1);
        
        edge = edge->next;
    }
    extrude();
}

void HalfEdgeMesh::extrude()
{
    std::vector<jcv_graphedge*> offsetEdges;
    float offset = 0.5f;
    glm::vec3 siteCenterOffset = glm::vec3(siteCenter.x, siteCenter.y, siteCenter.z-offset);
    jcv_graphedge *edge = mEdges[0];

    // Duplicate planes with triangles
    while (edge)
    {
        glm::vec3 p0 = glm::vec3(edge->pos[1].x, edge->pos[1].y, -offset);
        glm::vec3 p1 = glm::vec3(edge->pos[0].x, edge->pos[0].y, -offset);

        offsetEdges.push_back(edge);
        addTriangle(p0, p1, siteCenterOffset);

        edge = edge->next;
    }

    edge = mEdges[0];
    jcv_graphedge* edgeOffset = offsetEdges[0];

    // Connect planes
    while (edge && edgeOffset)
    {
        glm::vec3 p0, p1, p2, p3;
        p0 = glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f);
        p1 = glm::vec3(edgeOffset->pos[0].x, edgeOffset->pos[0].y, -offset);
        p2 = glm::vec3(edgeOffset->pos[1].x, edgeOffset->pos[1].y, -offset);
        p3 = glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f);

        addTriangle(p0, p1, p2);
        addTriangle(p0, p2, p3);


        edge = edge->next;
        edgeOffset = edgeOffset->next;
    }
}
