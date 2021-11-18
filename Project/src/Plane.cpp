        #include "Plane.h"

        Plane::Plane(float width, float height, glm::vec3 pos)
        {
            setName("Plane");
            generatePlane(width, height, pos);
            mTransMat = glm::translate(glm::mat4(1.0f), pos);
        }

        Plane::~Plane(){ }

        void Plane::initialize(glm::vec3)
        {
            GLCall(glGenVertexArrays(1, &vertexArrayID));
            GLCall(glBindVertexArray(vertexArrayID));

            Shader shader("src/res/shaders/Phong.shader", 0);
            shader.Bind();
            //shader.SetUniform4f("u_Color", 0.0, 1.0, 1.0, 1.0);
            shaderProgram = shader.getProgram();

            MVPLoc          = glGetUniformLocation(shaderProgram, "MVP");
            MVLoc           = glGetUniformLocation(shaderProgram, "MV");
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

            GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Color")));
            GLCall(glVertexAttribPointer(
                    glGetAttribLocation(shaderProgram, "in_Color"),
                    4,                          // size
                    GL_FLOAT,                   // type
                    GL_FALSE,                   // normalized?
                    0,                          // stride
                    0  // array buffer offset
            ));

            GLCall(glGenBuffers(1, &normalBuffer));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
            GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0], GL_STATIC_DRAW));
            // 2nd attribute buffer : normals
            GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Normal")));
            GLCall(glVertexAttribPointer(
                glGetAttribLocation(shaderProgram, "in_Normal"),                          // attribute 1. I.e. layout 1 in shader
                3,                          // size
                GL_FLOAT,                   // type
                GL_FALSE,                   // normalized?
                0,                          // stride
                reinterpret_cast<void*>(0)  // array buffer offset
            ));
            std::cout<<"Plane Initialized"<<std::endl;
        }

        void Plane::render(std::vector<glm::mat4x4> sceneMatrices)
        {
                GLCall(glUseProgram(shaderProgram));

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);

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

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0],
                            GL_STATIC_DRAW));
    }
        }

        void Plane::generatePlane(float width, float height, glm::vec3 pos)
        {
            glm::vec3 v0 = {pos.x-width, pos.y-height, pos.z};
            glm::vec3 v1 = {pos.x+width, pos.y+height, pos.z};
            glm::vec3 v2 = {pos.x-width, pos.y+height, pos.z};
            glm::vec3 v3 = {pos.x+width, pos.y-height, pos.z};

            //First Triangle
            addVertex(v0);
            addVertex(v1);
            addVertex(v2);

            glm::vec3 normal0 = calculateNormal(v0, v1, v2);
            addNormal(normal0);
            addNormal(normal0);
            addNormal(normal0);

            addColor(mColor);
            addColor(mColor);
            addColor(mColor);

            //Second Triangle
            addVertex(v0);
            addVertex(v3);
            addVertex(v1);

            glm::vec3 normal1 = calculateNormal(v0, v3, v1);
            addNormal(normal1);
            addNormal(normal1);
            addNormal(normal1);

            addColor(mColor);
            addColor(mColor);
            addColor(mColor);
        }