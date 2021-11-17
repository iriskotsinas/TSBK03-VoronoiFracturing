        #include "Plane.h"

        Plane::Plane(float width, float height, glm::vec3 pos)
        {
            setName("Plane");
            generatePlane(width, height, pos);
            mTransMat = glm::translate(glm::mat4(1.0f),pos);
        }
        Plane::~Plane(){

        }
        void Plane::initialize(glm::vec3){
            GLCall(glGenVertexArrays(1, &vertexArrayID));
            GLCall(glBindVertexArray(vertexArrayID));

            shader = Shader("src/res/shaders/SimpleColor.shader");
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
            std::cout<<"Plane Initialized"<<std::endl;
        }
        void Plane::render(std::vector<glm::mat4x4> sceneMatrices){
            GLCall(glUseProgram(shaderProgram));
            //glDisable( GL_BLEND );
            //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            
            glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
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

        }
        void Plane::generatePlane(float width, float height, glm::vec3 pos){
            glm::vec3 v0 = {-width, -height, 0.0f};
            glm::vec3 v1 = {width, height, 0.0f};
            glm::vec3 v2 = {-width, height, 0.0f};
            glm::vec3 v3 = {width, -height, 0.0f};
            //First Triangle
            addVertex(v0);
            addVertex(v1);
            addVertex(v2);
            //Second Triangle
            addVertex(v0);
            addVertex(v3);
            addVertex(v1);
        }