#include"gl_window.hpp"
#include"gl_shader.hpp"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

static constexpr int numVAOs = 1;
static constexpr int numVBOs = 2;

class MainWindow : public glWindow {
    GLuint renderingProgram;
    GLuint vao[numVAOs];
    GLuint vbo[numVBOs];
    ShaderProgram program;

    float cameraX, cameraY, cameraZ;
    float cube_x, cube_y, cube_z;
    GLuint mv_loc, proj_loc;
    int width, height;
    float aspect;
    glm::mat4 p_mat, v_mat, m_mat, mv_mat,r_mat;
    
public:
    
    virtual void init() override {
        renderingProgram = createShaderProgram();
        cameraX = 0;
        cameraY = 0;
        cameraZ = 8.0f;
        cube_x = 0.0f;
        cube_y = -2.0f;
        cube_z = 0.0f;

        GLfloat vertices[] = { -1.0f, -1.0f, 1.0f, // front face
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            
            -1.0f, -1.0f, -1.0f, // left side
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            
            -1.0f, 1.0f, -1.0f, // top
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            
            -1.0f, -1.0f, -1.0f, // bottom
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            
            1.0f, -1.0f, -1.0f, // right
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            
            -1.0f, -1.0f, -1.0f, // back face
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
        };
        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);
        glGenBuffers(numVBOs, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
    }
    
    virtual void update(double timeval) override {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(renderingProgram);
        float tf = 0.01;
        mv_loc = glGetUniformLocation(renderingProgram, "mv_matrix");
        proj_loc = glGetUniformLocation(renderingProgram,"proj_matrix");
        
        glfwGetFramebufferSize(win(), &width, &height);
        aspect = (float)width/(float)height;
        
        p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
        v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, cameraY, -cameraZ));
        
        float pos_x = -4.0, pos_y = 4.0f;        
        for(int z = 0; z < 25; ++z) {
            pos_x = -10.0f;
            for(int i = 0; i < 25; ++i) {
                pos_x += 1.0f;
                m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, -2.0f));
                
                r_mat = glm::rotate(glm::mat4(1.0f), 1.75f*(float)timeval,glm::vec3(0.0f, 1.0f, 0.0f));
                r_mat = glm::rotate(r_mat, 1.75f*(float)timeval,glm::vec3(1.0f, 0.0f, 0.0f));
                r_mat = glm::rotate(r_mat, 1.75f*(float)timeval, glm::vec3(0.0f, 0.0f, 1.0f));
                
                m_mat = m_mat * r_mat;
                mv_mat = v_mat * m_mat;
                
                glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
                glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
                
                glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
                glEnableVertexAttribArray(0);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_LEQUAL);
                glDrawArrays(GL_TRIANGLES,0,36);
            }
            pos_y -= 2.0f;
        }
    }

    GLuint createShaderProgram() {
        GLuint vfProgram = program.createProgramFromFile("vertex.glsl", "frag.glsl");
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
