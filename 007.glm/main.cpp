#include"gl_window.hpp"
#include"gl_shader.hpp"

// from glm header files
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

static constexpr int numVAOs = 1;

class MainWindow : public glWindow {
public:
    
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

    void keypress(int key, int scancode, int action, int mode) {
        if(key == GLFW_KEY_ESCAPE)
            exit(EXIT_SUCCESS);
    }
};

MainWindow main_window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    main_window.keypress(key, scancode, action, mode);
    std::cout << "keypress: " << key << "\n";
}

int main(int argc, char **argv) {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    main_window.create("Test", 1280, 720);
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    main_window.loop();
    return 0;
}
