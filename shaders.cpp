#include "shaders.hpp"
#include <vector>
#include <fstream>

using namespace std;

namespace shaders {
    Program::Program(string &frag_shader) {
        this->filename = frag_shader;
        this->program_handle = 0;
        this->vertex_handle = 0;
        this->fragment_handle = 0;

        this->reload();
    }

    void Program::compile(GLuint handle, string &shader_code) {
        const char *shader_src = shader_code.c_str();

        GLint result = GL_FALSE;
        int logLength;

        glShaderSource(handle, 1, &shader_src, NULL);
        glCompileShader(handle);

        glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> shader_error(logLength);
        
        glGetShaderInfoLog(handle, logLength, NULL, &shader_error[0]);
        if (result != GL_TRUE) {
            cerr << &shader_error[0] << endl;
            //throw std::runtime_error(&shader_error[0]);
            this->deleteExistingPrograms();
        }
    }

    void Program::deleteExistingPrograms() {
        if (this->program_handle) {
            glDeleteShader(this->program_handle);
            this->program_handle = 0;
        }
        
        if (this->vertex_handle) {
            glDeleteShader(this->vertex_handle);
            this->vertex_handle = 0;
        }

        if (this->fragment_handle) {
            glDeleteShader(this->fragment_handle);
            this->fragment_handle = 0;
        }
    }

    string readFile(string &path) {
        string content;
        ifstream fileStream(path.c_str(), std::ios::in);
        
        if (!fileStream.is_open()) {
            string msg = "File doesn't exist: ";
            msg += path;
            throw std::runtime_error(msg);
        }

        std::string line = "";
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }

    void Program::reload() {
        this->deleteExistingPrograms();
        this->vertex_handle = glCreateShader(GL_VERTEX_SHADER);
        
        string vertex_code = "#version 330 core \n\n \
layout(location = 0) in vec3 vertexPosition_modelspace; \n \
void main() { \n \
  gl_Position.xyz = vertexPosition_modelspace; \n \
  gl_Position.w = 1.0; \n \
}";
        this->compile(this->vertex_handle, vertex_code);

        this->fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
        string fragment_code = readFile(this->filename);
        this->compile(this->fragment_handle, fragment_code);

        this->program_handle = glCreateProgram();
        glAttachShader(this->program_handle, this->vertex_handle);
        glAttachShader(this->program_handle, this->fragment_handle);
        glLinkProgram(this->program_handle);

        GLint result = GL_FALSE;
        int logLength;

        glGetProgramiv(this->program_handle, GL_LINK_STATUS, &result);
        glGetProgramiv(this->program_handle, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> program_error((logLength > 1) ? logLength : 1);
        glGetProgramInfoLog(this->program_handle, logLength, NULL, &program_error[0]);
        if (result != GL_TRUE) {
            cerr << &program_error[0] << endl;
            //throw std::runtime_error(&program_error[0]);
            this->deleteExistingPrograms();
        }

        if (this->program_handle) {
            glUseProgram(this->program_handle);
            this->res_handle = glGetUniformLocation(this->program_handle, "resolution");
            glUseProgram(0);
        }
    }

    Program::~Program() {
        this->deleteExistingPrograms();
    }

    void Program::use(unsigned int screen_width, unsigned int  screen_height) {
        glUseProgram(this->program_handle);
        if (this->res_handle) {
            glUniform2f(this->res_handle, screen_width, screen_height);
        }
    }

    void Program::release() {
        glUseProgram(0);
    }
}
