#include "shaders.hpp"

namespace render {

    class Quad {
    public:
        Quad(shaders::Program *program);
        ~Quad();
        void render();
    private:
        shaders::Program *program;
        GLuint buffer_handle;
    };
}
