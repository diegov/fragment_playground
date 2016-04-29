#include <iostream>

#include <GL/glew.h>
#include <SDL.h>
#include <GL/gl.h>

#include "init.hpp"

using namespace std;

namespace init {

    void sdl_report_and_quit(const char *msg) {
        cout << msg << endl << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Window *init(int window_width, int window_height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) { 
            sdl_report_and_quit("Unable to initialize SDL");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        SDL_Window *window = SDL_CreateWindow("Shader Playground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        if (!gl_context) {
            sdl_report_and_quit("Unable to create GL context");
        }

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            sdl_report_and_quit("Failed to initialise glew");
        }
    
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        SDL_GL_SetSwapInterval(1);
                
        return window;
    }
}
