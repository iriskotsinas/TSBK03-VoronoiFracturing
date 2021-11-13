#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(Geometry* _mesh)
    :mesh{_mesh}
{
    mTransMat = glm::mat4(1.0f);
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
    }
    std::cout<<"VoronoiDiagram points sampled"<<std::endl;
}
void VoronoiDiagram::initialize(glm::vec3 lightPosition){
    
    GLCall(glGenVertexArrays(1, &vertexArrayID));
    GLCall(glBindVertexArray(vertexArrayID));

    Shader shader("src/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);
    shaderProgram = shader.getProgram();

    MVPLoc = glGetUniformLocation(shaderProgram, "MVP");

    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0], GL_STATIC_DRAW));

    // // 1st attribute buffer : vertices
    GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Position")));
    GLCall(glVertexAttribPointer(
        glGetAttribLocation(shaderProgram, "in_Position"),
        3,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        reinterpret_cast<void*>(0)  // array buffer offset
    ));
    std::cout<<"VoronoiDiagram initialized"<<std::endl;
}
void VoronoiDiagram::fracture(){

    //Fortune Sweep

    jcv_point j_points[points.size()];

    for (size_t i = 0; i < points.size(); i++)
    {
        j_points[i].x = points[i].x;
        j_points[i].y = points[i].y;
    }
    
    std::cout<<"Number of Points: "<<points.size()<<std::endl;

    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(points.size(), j_points, 0, 0, &diagram);

    
    const jcv_site* sites = jcv_diagram_get_sites(&diagram);

    std::cout<<"Number of Sites: "<<diagram.numsites <<std::endl;

    //If all you need are the edges
    const jcv_edge* edge = jcv_diagram_get_edges( dynamic_cast<const jcv_diagram*>(&diagram) );
    while( edge )
    {
        std::cout<<"E: p0: "<<edge->pos[0].x<<", p1: "<< edge->pos[1].x <<std::endl;
        // draw_line(edge->pos[0], edge->pos[1], sceneMatrices);
        orderedEdgePoints.push_back(glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f));
        orderedEdgePoints.push_back(glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f));
        edge = jcv_diagram_get_next_edge(edge);
    }
    // orderedEdgePoints.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    // orderedEdgePoints.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    // orderedEdgePoints.push_back(glm::vec3(-0.1f, 0.6f, 0.0f));
    // orderedEdgePoints.push_back(glm::vec3(-0.1f, 0.0f, 0.0f));
    std::cout<<"VoronoiDiagram fractured"<<std::endl;
}
void VoronoiDiagram::render(std::vector<glm::mat4x4> sceneMatrices){

    GLCall(glUseProgram(shaderProgram));

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);

    GLCall(glBindVertexArray(vertexArrayID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0], GL_STATIC_DRAW));
    // std::cout<<"size: " <<orderedEdgePoints.size()<<std::endl;
    
    glDrawArrays(GL_LINES, 0, orderedEdgePoints.size());
}