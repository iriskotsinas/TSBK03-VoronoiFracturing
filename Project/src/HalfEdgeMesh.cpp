#include "HalfEdgeMesh.h"

HalfEdgeMesh::HalfEdgeMesh(std::string s)
{
    setName(s);
    mTransMat = glm::mat4(1.0f);
}

void HalfEdgeMesh::initialize(glm::vec3 lightPosition)
{
    std::cout << "\nInitializing Half-Edge mesh ...\n\n";

    // mBoundingbox = new Boundingbox(buildVertexData());

    // mBoundingbox->initialize();

    // mBoundingbox->setWireFrame(true);

    // buildRenderData();

    // mTransformedVertexList = mOrderedVertexList;

    // Update face normals
    // for(unsigned int i = 0; i < mFaces.size(); i++) {
    //     getFace(i).normal = calculateFaceNormal(i);
    // }

    // // Update vertex normals
    // for(unsigned int i = 0; i < mVerts.size(); i++)
    //     getVert(i).normal = calculateVertNormal(i);

    // Update the lists that we draw
    // updateRenderData();

    // std::cout << "\t --- \t Volume: " << volume() << "\t --- \t" << std::endl;
    buildRenderData();

    GLCall(glGenVertexArrays(1, &vertexArrayID));
    GLCall(glBindVertexArray(vertexArrayID));

    Shader shader("src/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0, 1.0, 1.0, 1.0);
    shaderProgram = shader.getProgram();

    MVPLoc          = glGetUniformLocation(shaderProgram, "MVP");
    // MVLoc           = glGetUniformLocation(shaderProgram, "MV");
    // MVLightLoc      = glGetUniformLocation(shaderProgram, "MV_light");
    // NMLoc           = glGetUniformLocation(shaderProgram, "NM");
    // lightPosLoc     = glGetUniformLocation(shaderProgram, "lightPos");
    // colorLoc        = glGetUniformLocation(shaderProgram, "color");
    // lightAmbLoc     = glGetUniformLocation(shaderProgram, "ambientColor");
    // lightDifLoc     = glGetUniformLocation(shaderProgram, "diffuseColor");
    // lightSpeLoc     = glGetUniformLocation(shaderProgram, "specularColor");
    // specularityLoc  = glGetUniformLocation(shaderProgram, "specularity");
    // shinynessLoc    = glGetUniformLocation(shaderProgram, "shinyness");

    // glUniform4f(lightPosLoc, lightPosition[0], lightPosition[1], lightPosition[2], 1.0f);

    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));

    // // 1st attribute buffer : vertices
    GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Position")));
    GLCall(glVertexAttribPointer(
        glGetAttribLocation(shaderProgram, "in_Position"),                          // attribute 0. I.e. layout 0 in shader
        3,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        reinterpret_cast<void*>(0)  // array buffer offset
    ));

    if (debug)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0], GL_STATIC_DRAW));
    } else {
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));
    }

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
    // glGenBuffers(1, &normalBuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    // glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0], GL_STATIC_DRAW);
    // // 2nd attribute buffer : normals
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(
    //     1,                          // attribute 1. I.e. layout 1 in shader
    //     3,                          // size
    //     GL_FLOAT,                   // type
    //     GL_FALSE,                   // normalized?
    //     0,                          // stride
    //     reinterpret_cast<void*>(0)  // array buffer offset
    // );

    std::cout << "\nHalf-Edge mesh initialized!\n" << std::endl;
}

void HalfEdgeMesh::render(std::vector<glm::mat4x4> sceneMatrices)
{
    GLCall(glUseProgram(shaderProgram));

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);

    GLCall(glBindVertexArray(vertexArrayID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    if (debug)
    {
        //Lines
        GLCall(glBufferData(GL_ARRAY_BUFFER, orderedEdgePoints.size() * sizeof(glm::vec3), &orderedEdgePoints[0],
                            GL_STATIC_DRAW));
        // std::cout<<"size: " <<orderedEdgePoints.size()<<std::endl;

        glDrawArrays(GL_LINES, 0, orderedEdgePoints.size());
    } else {
        //Triangles
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0],
                            GL_STATIC_DRAW));
        // std::cout<<"size: " <<orderedEdgePoints.size()<<std::endl;

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, colorBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedColorList.size() * sizeof(glm::vec4), &mOrderedColorList[0],
                            GL_STATIC_DRAW));

        glDrawArrays(GL_TRIANGLES, 0, mOrderedVertexList.size());
    }
}

void HalfEdgeMesh::buildRenderData()
{
    jcv_graphedge *edge = mEdges[0];
    while (edge)
    {
        addVertex(siteCenter);
        addVertex(glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f));
        addVertex(glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f));

        addColor(mColor);
        addColor(mColor);
        addColor(mColor);

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
        offsetEdges.push_back(edge);
        addVertex(glm::vec3(edge->pos[1].x, edge->pos[1].y, -offset));
        addVertex(glm::vec3(edge->pos[0].x, edge->pos[0].y, -offset));
        addVertex(siteCenterOffset);

        addColor(mColor);
        addColor(mColor);
        addColor(mColor);

        edge = edge->next;
    }

    edge = mEdges[0];
    jcv_graphedge* edgeOffset = offsetEdges[0];

    // Connect planes
    while (edge && edgeOffset)
    {
        // First triangle
        addVertex(glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f));
        addVertex(glm::vec3(edgeOffset->pos[0].x, edgeOffset->pos[0].y, -offset));
        addVertex(glm::vec3(edgeOffset->pos[1].x, edgeOffset->pos[1].y, -offset));

        // Second triangle
        addVertex(glm::vec3(edge->pos[0].x, edge->pos[0].y, 0.0f));
        addVertex(glm::vec3(edgeOffset->pos[1].x, edgeOffset->pos[1].y, -offset));
        addVertex(glm::vec3(edge->pos[1].x, edge->pos[1].y, 0.0f));

        addColor(mColor);
        addColor(mColor);
        addColor(mColor);
        addColor(mColor);
        addColor(mColor);
        addColor(mColor);

        edge = edge->next;
        edgeOffset = edgeOffset->next;
    }
}
