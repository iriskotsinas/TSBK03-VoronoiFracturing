        #include "Plane.h"

        Plane::Plane(glm::vec3 _pos, std::string name)
            :pos{_pos}
        {
            setName(name);
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


                GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
                GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0],
                                    GL_STATIC_DRAW));
                glDrawArrays(GL_TRIANGLES, 0, mOrderedVertexList.size());
            }
        }

        void Plane::generatePlaneXY(float width, float height)
        {
            glm::vec3 v0 = {pos.x-width, pos.y-height, pos.z};
            glm::vec3 v1 = {pos.x+width, pos.y+height, pos.z};
            glm::vec3 v2 = {pos.x-width, pos.y+height, pos.z};
            glm::vec3 v3 = {pos.x+width, pos.y-height, pos.z};

            addTriangle(v0,v1,v2);
            addTriangle(v0,v3,v1);
            // addTriangle(v2,v1,v0);
            // addTriangle(v1,v3,v0);
            
        }
        void Plane::generatePlaneXZ(float width, float height)
        {
            glm::vec3 v0 = {pos.x-width, pos.y, pos.z-height};
            glm::vec3 v1 = {pos.x+width, pos.y, pos.z+height};
            glm::vec3 v2 = {pos.x-width, pos.y, pos.z+height};
            glm::vec3 v3 = {pos.x+width, pos.y, pos.z-height};

            addTriangle(v2,v1,v0);
            addTriangle(v1,v3,v0);
            // addTriangle(v2,v1,v0);
            // addTriangle(v1,v3,v0);
            
        }
        void Plane::generateCube(float width, float height, float depth)
        {
            glm::vec3 v0 = {pos.x-width, pos.y-height, pos.z };
            glm::vec3 v1 = {pos.x+width, pos.y+height, pos.z };
            glm::vec3 v2 = {pos.x-width, pos.y+height, pos.z };
            glm::vec3 v3 = {pos.x+width, pos.y-height, pos.z };

            glm::vec3 v4 = {pos.x-width, pos.y-height, pos.z - depth};
            glm::vec3 v5 = {pos.x+width, pos.y+height, pos.z - depth};
            glm::vec3 v6 = {pos.x-width, pos.y+height, pos.z - depth};
            glm::vec3 v7 = {pos.x+width, pos.y-height, pos.z - depth};

            //Front
            addTriangle(v0,v1,v2);
            addTriangle(v0,v3,v1);

            //Back
            addTriangle(v6,v5,v4);
            addTriangle(v5,v7,v4);
            
            //Left Side
            addTriangle(v0,v2,v4);
            addTriangle(v2,v6,v4);

            //Right Side
            addTriangle(v3,v7,v1);
            addTriangle(v7,v5,v1);

            //Top Side
            addTriangle(v1,v5,v2);
            addTriangle(v2,v5,v6);

            //Bottom Side
            addTriangle(v0,v7,v3);
            addTriangle(v0,v4,v7);
            


        }