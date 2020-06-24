#include"gl_window.hpp"
#include"gl_shader.hpp"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<opencv2/opencv.hpp>


static constexpr int numVAOs = 1;
static constexpr int numVBOs = 2;
std::string filename;

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
    glm::mat4 p_mat, v_mat, m_mat, mv_mat, r_mat;
    GLuint texture;
    float pos_z;
    cv::VideoCapture cap;
    float color_alpha_r, color_alpha_g, color_alpha_b;
public:
    
    virtual void init() override {
        renderingProgram = createShaderProgram();
        cameraX = 0;
        cameraY = 0;
        cameraZ = 8.0f;
        cube_x = 0.0f;
        cube_y = -2.0f;
        cube_z = 0.0f;
        pos_z = -2.0f;
        color_alpha_r = 0.1;
        color_alpha_g = 0.2;
        color_alpha_b = 0.3;
        
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
        
        GLfloat texCoords[] = {
            0, 0, // front
            1, 1,
            0, 1,
            
            0, 0,
            1, 0,
            1, 1,
            
            0, 0, // left
            1, 0,
            0, 1,
            
            0, 1,
            1, 0,
            1, 1,
            
            0,0, // top
            0,1,
            1,1,
            
            1, 1,
            1, 0,
            0, 0,
            
            0, 0,// bottom
            0, 1,
            1, 1,
            
            1,1,
            1,0,
            0,0,
            
            0,0,// right
            1,0,
            0,1,
            
            0,1,
            1,0,
            1,1,
            
            0,0, // back
            1,1,
            0,1,
            
            0,0,
            1,0,
            1,1
        };
        
        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);
        glGenBuffers(numVBOs, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        
        glfwGetFramebufferSize(win(), &width, &height);
        aspect = (float)width/(float)height;
        
        p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        cv::Mat frame;
        
        cap.open(filename);
        if(!cap.isOpened()) {
            std::cerr << "Error opening file..\n";
            exit(EXIT_FAILURE);
        }
        cap.read(frame);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
    }
    
    virtual void update(double timeval) override {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(renderingProgram);
        
        mv_loc = glGetUniformLocation(renderingProgram, "mv_matrix");
        proj_loc = glGetUniformLocation(renderingProgram,"proj_matrix");
        
        GLuint samp = glGetUniformLocation(renderingProgram,"samp");
        
        GLuint calpha_r = glGetUniformLocation(renderingProgram,"value_alpha_r");
        
        GLuint calpha_g = glGetUniformLocation(renderingProgram,"value_alpha_g");
        
        GLuint calpha_b = glGetUniformLocation(renderingProgram,"value_alpha_b");
        
        cameraZ = 8.0f;
        
        v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, cameraY, -cameraZ));
        
        cv::Mat frame;
        if(!cap.read(frame)) {
            cap.open(filename);
            cap.read(frame);
        }
        cv::flip(frame, frame, 0);
        
        
        float pos_x = -4.0, pos_y = -2.0f;
        
        static int dir = 1;
        if(dir == 1) {
            pos_z += 0.01f;
            if(pos_z > 4.0f)
                dir = 0;
        } else {
            pos_z -= 0.01f;
            if(pos_z <= -2.0f)
                dir = 1;
        }
        
        for(int z = 0; z < 4; ++z) {
            pos_x = -12.0f;
            for(int i = 0; i < 4; ++i) {
                pos_x += 5.0f;
                m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, pos_z));
                
                r_mat = glm::rotate(glm::mat4(1.0f), 1.75f*(float)timeval,glm::vec3(0.0f, 1.0f, 0.0f));
                r_mat = glm::rotate(r_mat, 1.75f*(float)timeval,glm::vec3(1.0f, 0.0f, 0.0f));
                r_mat = glm::rotate(r_mat, 1.75f*(float)timeval, glm::vec3(0.0f, 0.0f, 1.0f));
                
                m_mat = m_mat * r_mat;
                mv_mat = v_mat * m_mat;
                
                glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
                glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
                
                glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
                glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
                
                glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
                glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
                
                glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
                glEnableVertexAttribArray(0);
                
                glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
                glEnableVertexAttribArray(1);
                
                
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
                
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);
                
                color_alpha_r += (rand()%100) * 0.01f;
                color_alpha_g += (rand()%100) * 0.01f;
                color_alpha_b += (rand()%100) * 0.01f;
                
                if(color_alpha_r > 1)
                    color_alpha_r = 0.1f;
                if(color_alpha_g > 1)
                    color_alpha_g = 0.1f;
                if(color_alpha_b > 1)
                    color_alpha_b = 0.1f;
                
                glUniform1i(samp, 0);
                glUniform1f(calpha_r, color_alpha_r);
                glUniform1f(calpha_g, color_alpha_g);
                glUniform1f(calpha_b, color_alpha_b);
                
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_LEQUAL);
                
                glDrawArrays(GL_TRIANGLES,0,36);
                
            }
            pos_y += 5.0f;
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
    
    if(argc != 2) {
        std::cerr << "Error requires video file as argument.\n";
        exit(EXIT_FAILURE);
    }
    
    filename = argv[1];
    
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    main_window.create("Cube Demo", 2560, 1440);
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    main_window.loop();
    return 0;
}
