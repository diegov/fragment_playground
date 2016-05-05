#include "shaders.hpp"

namespace render {

    class Quad {
    public:
        Quad(shaders::Program *program, unsigned int width, unsigned int height);
        ~Quad();
        void render(float time);
        void resize(unsigned int width, unsigned int height);
    private:
        shaders::Program *program;
        GLuint buffer_handle;
        unsigned int width;
        unsigned int height;
    };
}
