#include "Geometry.h"
void Geometry::initialize()
{
    GLCall(glGenVertexArrays(1, &vertexArrayID));
    GLCall(glBindVertexArray(vertexArrayID));

    Shader shader("src/res/shaders/Phong.shader", 0);
    shader.Bind();
    shaderProgram = shader.getProgram();

    MVPLoc          = glGetUniformLocation(shaderProgram, "MVP");
    MVLoc           = glGetUniformLocation(shaderProgram, "MV");

    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));

    //Positions attribute buffer
    GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Position")));
    GLCall(glVertexAttribPointer(
        glGetAttribLocation(shaderProgram, "in_Position"),                          // attribute 0. I.e. layout 0 in shader
        3,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        reinterpret_cast<void*>(0)  // array buffer offset
    ));

    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0], GL_STATIC_DRAW));


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

    //Normal attribute buffer
    GLCall(glGenBuffers(1, &normalBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0], GL_STATIC_DRAW));
 
    GLCall(glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "in_Normal")));
    GLCall(glVertexAttribPointer(
         glGetAttribLocation(shaderProgram, "in_Normal"),                          // attribute 1. I.e. layout 1 in shader
         3,                          // size
         GL_FLOAT,                   // type
         GL_FALSE,                   // normalized?
         0,                          // stride
         reinterpret_cast<void*>(0)  // array buffer offset
     ));
}

void Geometry::render(std::vector<glm::mat4x4> sceneMatrices)
{
    GLCall(glUseProgram(shaderProgram));

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);

    GLCall(glBindVertexArray(vertexArrayID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    //Triangles
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedVertexList.size() * sizeof(glm::vec3), &mOrderedVertexList[0],
                        GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, colorBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedColorList.size() * sizeof(glm::vec4), &mOrderedColorList[0],
                        GL_STATIC_DRAW));

    glDrawArrays(GL_TRIANGLES, 0, mOrderedVertexList.size());

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, mOrderedNormalList.size() * sizeof(glm::vec3), &mOrderedNormalList[0],
                        GL_STATIC_DRAW));
    
}
 void Geometry::setBoundaries(std::pair< float, float> &x, std::pair< float, float> &y)
{
    float min_x = std::numeric_limits<float>::infinity();
    float max_x = std::numeric_limits<float>::min();
    float min_y = std::numeric_limits<float>::infinity();
    float max_y = std::numeric_limits<float>::min();
    for (auto v : mOrderedVertexList)
    {
        if(v[0] < min_x)
        {
            min_x = v[0];
        }
        if(v[0] > max_x)
        {
            max_x = v[0];
        }
        if(v[1] < min_y)
        {
            min_y = v[1];
        }
        if(v[1] > max_y)
        {
            max_y = v[1];
        }
    }
    x.first = min_x;
    x.second = max_x;
    y.first = min_y;
    y.second = max_y;
}
void Geometry::addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    if (std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v0) == mUniqueVertexList.end())
    {
        mUniqueVertexList.push_back(v0);
    }
    if (std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v1) == mUniqueVertexList.end())
    {
        mUniqueVertexList.push_back(v1);
    }
    if(std::find(mUniqueVertexList.begin(), mUniqueVertexList.end(), v2) == mUniqueVertexList.end())
    {
        mUniqueVertexList.push_back(v2);
    }

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
}
 void Geometry::calculateGeometryWorldPosition()
{
    glm::vec4 tmpPos = glm::vec4(0.0f);
    for (auto v : mOrderedVertexList)
    {
        tmpPos += glm::vec4(v, 1.0f);
    }
    tmpPos = toMatrix4x4(glm::inverse(mTransMat)) * tmpPos;
    tmpPos /= mOrderedVertexList.size();
    

    mWorldPosition = glm::vec3(tmpPos[0], tmpPos[1], tmpPos[2]);
}
glm::vec3 Geometry::calculateNormal(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) {
    glm::vec3 edge0 = v1 - v0;
    glm::vec3 edge1 = v2 - v0;

    return glm::normalize(glm::cross(edge0, edge1));
}



glm::mat4x4 Geometry::toMatrix4x4Row(glm::mat4 m)
{
    return glm::mat4x4
            {
                    {m[0][0], m[0][1], m[0][2], m[0][3]},
                    {m[1][0], m[1][1], m[1][2], m[1][3]},
                    {m[2][0], m[2][1], m[2][2], m[2][3]},
                    {m[3][0], m[3][1], m[3][2], m[3][3]}
            };
}
glm::mat4x4 Geometry::toMatrix4x4Column(glm::mat4 m)
{
    return glm::mat4x4
            {
                    {m[0][0], m[1][0], m[2][0], m[3][0]},
                    {m[0][1], m[1][1], m[2][1], m[3][1]},
                    {m[0][2], m[1][2], m[2][2], m[3][2]},
                    {m[0][3], m[2][3], m[2][3], m[3][3]}
            };
}
glm::mat4x4 Geometry::toMatrix4x4(glm::mat3 m) const
{
    return glm::mat4x4
            {
                    {m[0][0], m[0][1], m[0][2], 0.0f},
                    {m[1][0], m[1][1], m[1][2], 0.0f},
                    {m[2][0], m[2][1], m[2][2], 0.0f},
                    {0.0f   , 0.0f   , 0.0f   , 1.0f}
            };
}