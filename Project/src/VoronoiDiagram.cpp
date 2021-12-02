#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(Geometry* _mesh)
    :mesh{_mesh}
{
    mesh->setBoundaries(x, y);
}

void VoronoiDiagram::sampleUniformPoints(unsigned int n)
{
    // std::cout<<"xp: "<<x.first<<", yp: "<<x.second<<std::endl;
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
        // std::cout<<"P: x: "<<dist1(e2)<<" , y: "<<point.y<<std::endl;
    }
    std::cout<<"VoronoiDiagram uniform points sampled"<<std::endl;
}

void VoronoiDiagram::sampleCrackPoints(unsigned int n)
{
    float middle = (glm::abs(x.second) - glm::abs(x.first)) / 2;
    // std::cout<<"xp: "<<x.first<<", yp: "<<x.second<<std::endl;
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist1(middle - 0.2, middle + 0.2);
    std::uniform_real_distribution<> dist2(y.first, y.second);
    for (unsigned int i = 0; i < n; i++)
    {
        jcv_point point;
        point.x = dist1(e2);
        point.y = dist2(e2);
        points.push_back(point);
        // std::cout<<"P: x: "<<dist1(e2)<<" , y: "<<point.y<<std::endl;
    }
    std::cout<<"VoronoiDiagram crack points sampled"<<std::endl;
}

void VoronoiDiagram::sampleHolePoints(unsigned int n)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1.0f);
    float radius = 0.0f;

    for (unsigned int i = 0; i < n; i++)
    {
        jcv_point point;
        radius *= glm::sqrt(dist(e2));
        float angle = 2.0f * glm::pi<float>() * dist(e2);
        point.x = dist(e2) * glm::cos(angle);
        point.y = dist(e2) * glm::sin(angle);
        points.push_back(point);
    }
    std::cout<<"VoronoiDiagram hole points sampled"<<std::endl;
}

std::vector<Geometry*> VoronoiDiagram::fracture(bool randomColors)
{

    //Fortune Sweep

    jcv_point j_points[points.size()];

    for (size_t i = 0; i < points.size(); i++)
    {
        j_points[i].x = points[i].x;
        j_points[i].y = points[i].y;
    }
    
    std::cout<<"Number of Points: "<<points.size()<<std::endl;

    jcv_rect* bounds = new jcv_rect();
    jcv_point maxPoint, minPoint;

    minPoint.x = x.first;
    minPoint.y = y.first;
    maxPoint.x = x.second;
    maxPoint.y = y.second;

    bounds->max = maxPoint;
    bounds->min = minPoint;


    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(points.size(), j_points, bounds, 0, &diagram);


    std::cout<<"Number of Sites: "<<diagram.numsites <<std::endl;

    //Building data structure for rendering
    std::vector<Geometry*> fractures;
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0.0f, 1.0f);

    const jcv_site* sites = jcv_diagram_get_sites( &diagram );
    for ( int i = 0; i < diagram.numsites; ++i )
    {
        const jcv_site* site = &sites[i];
        glm::vec4 siteColor;
        if (randomColors)
        {
            float sat = dist(e2);
            siteColor = glm::vec4(sat, sat, sat + 0.3f, 1.0f);
        } else {
            siteColor = glm::vec4(0.7f, 0.7f, 0.9f, 1.0f);
        }

        const jcv_graphedge* e = site->edges;

        HalfEdgeMesh* mesh = new HalfEdgeMesh("site");
        mesh->setColor(siteColor);
        mesh->setSiteCenter(glm::vec3(site->p.x, site->p.y, 0.0f));

        while (e) {
            mesh->addHalfEdge(e);
            e = e->next;
        }
        fractures.push_back(mesh);
    }
    std::cout<<"VoronoiDiagram fractured"<<std::endl;
    return fractures;
}