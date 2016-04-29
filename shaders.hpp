#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <stdexcept>

#pragma once

using namespace std;

namespace shaders {
    class Program {
    public:
        Program(string &frag_shader);
        ~Program();
        void reload();

        void use();
        void release();
    protected:
        GLuint program_handle;
        GLuint vertex_handle;
        GLuint fragment_handle;
        string filename;
    private:
        void deleteExistingPrograms();
        void compile(GLuint handle, string &shader_code);
    };
}
