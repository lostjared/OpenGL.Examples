#ifndef __GL_WINDOW__H_
#define __GL_WINDOW__H_

#include<iostream>
#include<string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

class glWindow {
public:
    int create(std::string name, int w, int h);
    virtual void init() = 0;
    virtual void update(double timeval) = 0;
    void loop();
private:
    GLFWwindow *window;
};

class ShaderProgram {
public:
    int printShaderLog(GLuint err);
    void printProgramLog(int p);
    bool checkError();
    GLuint createProgram(const char *vert, const char *frag);
};


#endif

