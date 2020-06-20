#include"gl_window.hpp"
#include"gl_shader.hpp"

#define numVAOs 1

class MainWindow : public glWindow {

    virtual void init() override {
        renderingProgram = createShaderProgram();
        glGenVertexArrays(numVAOs, vao);
        glBindVertexArray(vao[0]);
    }
    virtual void update(double timeval) override {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(renderingProgram);
        glPointSize(30.0f);
        glDrawArrays(GL_POINTS,0,1);
    }
    GLuint renderingProgram;
    GLuint vao[numVAOs];
    ShaderProgram program;
    
    GLuint createShaderProgram() {
        GLuint vfProgram = program.createProgramFromFile("shader.vert", "shader.frag");
        return vfProgram;
    }
};

int main(int argc, char **argv) {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    MainWindow window;
    window.create("Test", 1280, 720);
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    window.loop();
    return 0;
}
