#include"gl_window.hpp"

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
    
    GLuint createShaderProgram() {
        const char *vshaderSource =
        "#version 410\n"
        "void main(void)\n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
        
        const char *fshaderSource =
        "#version 410\n"
        "out vec4 color;\n"
        "void main() \n"
        "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";
        
        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        glShaderSource(vShader, 1, &vshaderSource, NULL);
        glShaderSource(fShader, 1, &fshaderSource, NULL);
        
        glCompileShader(vShader);
        glCompileShader(fShader);
        
        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);
        glLinkProgram(vfProgram);
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
