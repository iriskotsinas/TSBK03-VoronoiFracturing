#include "HalfEdgeMesh.h"

HalfEdgeMesh::HalfEdgeMesh(std::string s)
    :mObjName(s) {

    mTransMat = glm::mat4(1.0f);


}
void HalfEdgeMesh::generatePlane(float width, float height) {

    glm::vec3 v0 = {-width, -height, 0.0f};
    glm::vec3 v1 = {width, height, 0.0f};
    glm::vec3 v2 = {-width, height, 0.0f};
    glm::vec3 v3 = {width, -height, 0.0f};

    std::cout << "\nAdd vertex ...\n\n";

    // Add vertices and normals
    addVertex(v0);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(v1);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(v2);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(v0);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(v3);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(v1);
    // addNormal(glm::vec3(0.0f, 0.0f, 1.0f));

    // calculateCenterOfMass();
}
void HalfEdgeMesh::initialize(glm::vec3 lightPosition) {

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

    GLCall(glGenVertexArrays(1, &vertexArrayID));
    GLCall(glBindVertexArray(vertexArrayID));
    // VertexArray va;
    // VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    // IndexBuffer ib(indices, 6);

    // VertexBufferLayout layout;
    // layout.AddFloat(2);

    // va.AddBuffer(vb, layout);
    // // Create and compile our GLSL program from the shaders
    Shader shader("src/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0, 1.0, 1.0, 1.0);
    shaderProgram = shader.getProgram();

    // MVPLoc          = glGetUniformLocation(shaderProgram, "MVP");
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
    GLCall(glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW));

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
void HalfEdgeMesh::render(std::vector<glm::mat4x4> sceneMatrices) {

    // Use shader
    GLCall(glUseProgram(shaderProgram));
    //glDisable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP](0, 0));
    // glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV](0, 0));
    // glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &sceneMatrices[I_MV_LIGHT](0, 0));
    // glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &sceneMatrices[I_NM](0, 0));
    // glUniform4f(colorLoc, mMaterial.color[0], mMaterial.color[1], mMaterial.color[2], mMaterial.color[3]);
    // glUniform4f(lightAmbLoc, mMaterial.ambient[0], mMaterial.ambient[1], mMaterial.ambient[2], mMaterial.ambient[3]);
    // glUniform4f(lightDifLoc, mMaterial.diffuse[0], mMaterial.diffuse[1], mMaterial.diffuse[2], mMaterial.diffuse[3]);
    // glUniform4f(lightSpeLoc, mMaterial.specular[0], mMaterial.specular[1], mMaterial.specular[2], mMaterial.specular[3]);
    // glUniform1f(specularityLoc, mMaterial.specularity);
    // glUniform1f(shinynessLoc, mMaterial.shinyness);

    // Rebind the buffer data, vertices are now updated
    GLCall(glBindVertexArray(vertexArrayID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW));

    // Rebind the buffer data, normals are now updated
    // glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    // glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0], GL_STATIC_DRAW);

    // Draw the triangle!
    GLCall(glDrawArrays(GL_TRIANGLES, 0, mVerts.size())); // 3 indices starting at 0 -> 1 triangle
    
    // Unbind
//    GLCall(glBindVertexArray(0));
//    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//    GLCall(glDisableVertexAttribArray(0));

    //glDisable( GL_BLEND );

    // mBoundingbox->render(sceneMatrices[I_MVP]);

    // if(mDebugMode) {
    //     for(unsigned int i = 0; i < mDebugPoints.size(); i++)
    //         mDebugPoints[i]->render(sceneMatrices);
    // }
}
void HalfEdgeMesh::buildRenderData() {

    for(int i = 0; i < mFaces.size(); i++ ){
        
        Face face = getFace(i);

        HalfEdge edge = getEdge(face.edge);

        Vertex v1 = getVert(edge.vert);
        edge = getEdge(edge.next);

        Vertex v2 = getVert(edge.vert);
        edge = getEdge(edge.next);

        Vertex v3 = getVert(edge.vert);

        // Add vertices to our drawing list
        mOrderedVertexList.push_back(v1.pos);    
        mOrderedVertexList.push_back(v2.pos);
        mOrderedVertexList.push_back(v3.pos);

        // glm::vec3 faceNormal = getFace(i).normal;

        // Add normals to our drawing list
        // mOrderedNormalList.push_back(faceNormal);
        // mOrderedNormalList.push_back(faceNormal);
        // mOrderedNormalList.push_back(faceNormal);
    }
}
//unsigned int HalfEdgeMesh::getEdge(glm::vec3 vertPos) {
//    for(unsigned int i = 0; i < mEdges.size(); i++) {
//        if(getVert(getEdge(i).vert).pos == vertPos)
//            return i;
//    }
//}
