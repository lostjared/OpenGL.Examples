#include<iostream>
#include<GL/glew.h>
#include "SDL.h"

SDL_Window *window;
SDL_GLContext context;
static constexpr int w = 1280;
static constexpr int h = 720;

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(int argc, char **argv) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Error initilizing SDL: " << SDL_GetError() << "\n";
        exit(-1);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    int full_val = 0;
    window = SDL_CreateWindow("acidcamGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, full_val | SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    if(glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW:\n";
        exit(EXIT_FAILURE);
    }
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    SDL_Event e;
    bool active = true;
    std::string num;
    while(active) {
        render();
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    active = false;
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        active = false;
                    break;
            }
            
            SDL_GL_SwapWindow(window);
        }
    }
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
