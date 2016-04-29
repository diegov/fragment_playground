#include <GL/glew.h>
#include <GL/gl.h>

#include "render.hpp"

namespace render {
    static GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };

    Quad::Quad(shaders::Program *program) {
        this->program = program;
        glGenBuffers(1, &this->buffer_handle);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffer_handle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    Quad::~Quad() {
        glDeleteBuffers(1, &this->buffer_handle);
    }
    
    void Quad::render() {
        this->program->use();
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffer_handle);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0);
    
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        
        this->program->release();
    }
}
