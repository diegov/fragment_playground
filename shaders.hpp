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

        void use(unsigned int screen_width, unsigned int screen_height);
        void release();
    private:
        GLuint program_handle;
        GLuint vertex_handle;
        GLuint fragment_handle;
        string filename;

        GLuint res_handle;

        void deleteExistingPrograms();
        void compile(GLuint handle, string &shader_code);
    };
}
