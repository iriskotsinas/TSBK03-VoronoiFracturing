#pragma once

#include "Debug.h"
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
    private:
        GLuint m_RendererID;
        std::string m_FilePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;

    public:
        Shader() = default;
        Shader(const std::string& filepath, GLuint id);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
        void SetUniform1f(const std::string& name, float value);

        GLuint getProgram(){
            return m_RendererID;
        } 
    private:
        int GetUniformLocation(const std::string& name);
        struct ShaderProgramSource ParseShader(const std::string& filepath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};
