#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(Geometry* _mesh)
    :mesh{_mesh}
{

    mesh->setBoundaries(x, y);

}
void VoronoiDiagram::samplePoints(unsigned int n)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist1(x.first, x.second);
    std::uniform_real_distribution<> dist2(y.first, y.second);
    for (unsigned int i = 0; i < n; i++)
    {
        jcv_point point;
        point.x = dist1(e2);
        point.y = dist2(e2);
        points.push_back(point);
    }
    
}
void VoronoiDiagram::fracture(){

    //Fortune Sweep

    // jcv_clipping_polygon poligon;
    // poligon.num_points = points.size();
    // poligon.points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)mesh->getVertCount());

    // for (int i = 0; i < poligon.num_points; i++)
    // {
    //     glm::vec3 point = mesh->getVerts()[i];
    //     poligon.points[i].x = point.x;
	// 	poligon.points[i].y = point.y;
    // }

    // jcv_clipper polyClipper;
    // polyClipper.test_fn = jcv_clip_polygon_test_point;
    // polyClipper.clip_fn = jcv_clip_polygon_clip_edge;
    // polyClipper.fill_fn = jcv_clip_polygon_fill_gaps;
    // polyClipper.ctx = &poligon;
    // jcv_clipper* clipper = &polyClipper;
    jcv_point j_points[points.size()];

    for (size_t i = 0; i < points.size(); i++)
    {
        j_points[i].x = points[i].x;
        j_points[i].y = points[i].y;
    }
    

    jcv_diagram diagram;
    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(points.size(), j_points, 0, 0, &diagram);

    



    // jcv_diagram_free(&diagram);
}