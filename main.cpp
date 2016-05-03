#include <iostream>

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/inotify.h>
#include <string>
#include <atomic>

#include <GL/glew.h>
#include <SDL.h>
#include <GL/gl.h>

#include "init.hpp"
#include "shaders.hpp"
#include "render.hpp"

using namespace std;

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

sig_atomic_t quit;
atomic<bool> file_changed;

pthread_t watch_thread_id;

void sigint(int signo) {
    quit = 1;
}

void* watch_file(void *arg) {
    string *filepath = (string*)arg;

    char *filepath_cstr = new char[filepath->length() + 1]();

    strcpy(filepath_cstr, filepath->c_str());
    string filename = basename(filepath_cstr);

    strcpy(filepath_cstr, filepath->c_str());
    char *directory = dirname(filepath_cstr);

    cout << "Watching file " << filename << " in directory " << directory << endl;

    int fd, wd;
    char buffer[BUF_LEN];

    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
    }

    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    wd = inotify_add_watch(fd, directory, IN_MODIFY | IN_CREATE);

    while (quit == 0) {
        int length, i = 0;
        length = read(fd, buffer, BUF_LEN);
        if (length < 0 && errno == EAGAIN) {
            usleep(50000);
            continue;
        }

        if (length < 0) {
            perror("read");
        }

        while (i < length) {
            struct inotify_event *event =
                (struct inotify_event *) &buffer[i];
            if (event->len) {
                string affected = event->name;
                if (affected == filename) {
                    file_changed = true;
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    delete filepath;
    delete filepath_cstr;

    return NULL;
}

int start_watcher_thread(string &filename) {
    sigset_t sigset, oldset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    pthread_sigmask(SIG_BLOCK, &sigset, &oldset);

    string *thread_data = new string(filename);

    int retval = pthread_create(&watch_thread_id, NULL, watch_file, thread_data);

    struct sigaction sigact;
    sigact.sa_handler = sigint;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, NULL);

    pthread_sigmask(SIG_SETMASK, &oldset, NULL);

    return retval;
}

int main(int argc, char **argv) {
    quit = 0;
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " file" << endl;
        exit(1);
    }

    string filename = argv[1];

    int width = 400;
    int height = 400;

    SDL_Window *window = init::init(width, height);

    start_watcher_thread(filename);

    GLuint vert_array_id;
    glGenVertexArrays(1, &vert_array_id);
    glBindVertexArray(vert_array_id);

    shaders::Program p(filename);

    render::Quad quad(&p, width, height);

    SDL_Event e;

    while (quit == 0) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = 1;
            }

            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                glViewport(0, 0, e.window.data1, e.window.data2);
                quad.resize(e.window.data1, e.window.data2);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        quad.render();
        
        SDL_GL_SwapWindow(window);

        bool expected = true;
        if (file_changed.compare_exchange_strong(expected, false)) {
            cout << filename << " modified" << endl;
            p.reload();
        }
    }

    pthread_join(watch_thread_id, NULL);
}
