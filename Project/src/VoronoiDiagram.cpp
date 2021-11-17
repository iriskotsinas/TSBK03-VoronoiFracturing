#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(Geometry* _mesh, Scene* s)
    :mesh{_mesh}, scene{s}
{
    mTransMat = mesh->getTransMat();
    mesh->setBoundaries(x, y);

}

void VoronoiDiagram::samplePoints(unsigned int n)
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
    std::cout<<"VoronoiDiagram points sampled"<<std::endl;
}

void VoronoiDiagram::initialize(glm::vec3 lightPosition)
{
    std::cout<<"Colors: "<<mOrderedColorList.size()<<std::endl;
    std::cout<<"Vertices: "<<mOrderedVertexList.size()<<std::endl;
    //Positions
    GLCall(glGenVertexArrays(1, &vertexArrayID));
    GLCall(glBindVertexArray(vertexArrayID));

    Shader shader("src/res/shaders/Basic.shader");
    shader.Bind();
    // shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);
    shaderProgram = shader.getProgram();

    MVPLoc = glGetUniformLocation(shaderProgram, "MVP");
    //Positions
    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));


    if (debug)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0], GL_STATIC_DRAW));
    } else {
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));
    }
    
    //Position attribute buffer
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(
        0,
        3,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        0  // array buffer offset
    ));

    //Vertex Colors attribute buffer
    GLCall(glGenBuffers(1, &colorBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, colorBuffer));

    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedColorList.size() * sizeof(glm::vec4), &mOrderedColorList[0], GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(
        1,
        4,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        0  // array buffer offset
    ));

    std::cout<<"VoronoiDiagram initialized"<<std::endl;
}

void VoronoiDiagram::fracture()
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
    if (debug)
    {
        buildLines();
    } else {
        buildTriangles();
    }
    std::cout<<"VoronoiDiagram fractured"<<std::endl;
}

// glm::vec3 VoronoiDiagram::enforceBoundaries(glm::vec3 p){
//     if(p.x < x.first)
//         p.x = x.first;
//     else if(p.x > x.second)
//         p.x = x.second;

//     if(p.y < y.first)
//         p.y = y.first;
//     else if(p.y > y.second)
//         p.y = y.second;

//     std::cout<<"p: x: "<<p.x<<", y: "<< p.x <<std::endl;
//     return p;
// }

void VoronoiDiagram::buildLines()
{
    const jcv_edge* edge = jcv_diagram_get_edges( dynamic_cast<const jcv_diagram*>(&diagram) );
    while( edge )
    {
        // draw_line(edge->pos[0], edge->pos[1], sceneMatrices);
        glm::vec3 pos0 = glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f);
        glm::vec3 pos1 = glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f);
        orderedEdgePoints.push_back(pos0);
        orderedEdgePoints.push_back(pos1);
        mOrderedColorList.push_back(glm::vec4(1.0, 0, 0, 1.0f));
        mOrderedColorList.push_back(glm::vec4(1.0, 0, 0, 1.0f));
        edge = jcv_diagram_get_next_edge(edge);
    }
}

void VoronoiDiagram::buildTriangles()
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0.0f, 1.0f);

    const jcv_site* sites = jcv_diagram_get_sites( &diagram );
    for ( int i = 0; i < diagram.numsites; ++i )
    {
        const jcv_site* site = &sites[i];
        glm::vec4 siteColor = glm::vec4(dist(e2), dist(e2), dist(e2), 1.0f);
        const jcv_graphedge* e = site->edges;

        HalfEdgeMesh* mesh = new HalfEdgeMesh("site");
        mesh->setColor(siteColor);
        mesh->setSiteCenter(glm::vec3(site->p.x, site->p.y, 0.0f));

        while (e) {
            mesh->addHalfEdge(e);
            e = e->next;
        }
        scene->addGeometry(mesh);
    }
}

void VoronoiDiagram::render(std::vector<glm::mat4x4> sceneMatrices)
{
    GLCall(glUseProgram(shaderProgram));

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);

    GLCall(glBindVertexArray(vertexArrayID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    if (debug)
    {
        //Lines
        GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0], GL_STATIC_DRAW));
        // std::cout<<"size: " <<orderedEdgePoints.size()<<std::endl;
        
        glDrawArrays(GL_LINES, 0, orderedEdgePoints.size());
    } else {
        //Triangles
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));
        // std::cout<<"size: " <<orderedEdgePoints.size()<<std::endl;
        
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, colorBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedColorList.size() * sizeof(glm::vec4), &mOrderedColorList[0], GL_STATIC_DRAW));
        
        glDrawArrays(GL_TRIANGLES, 0, mOrderedVertexList.size());
    }
}
