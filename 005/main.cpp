#include"gl_window.hpp"
#include"gl_shader.hpp"

static constexpr int numVAOs = 1;

class MainWindow : public glWindow {

    virtual void init() override {
        renderingProgram = createShaderProgram();
        glGenVertexArrays(numVAOs, vao);
        glBindVertexArray(vao[0]);
    }
    
    virtual void update(double timeval) override {
        static float x = 0.0f;
        static float inc = 0.01;
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(renderingProgram);
        x += inc;
        if(x > 1.0f) inc = -0.01f;
        if(x < -1.0f) inc = 0.01f;
        
        GLuint offset_loc = glGetUniformLocation(renderingProgram, "offset");
        glProgramUniform1f(renderingProgram, offset_loc, x);
        glDrawArrays(GL_TRIANGLES,0,3);
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
