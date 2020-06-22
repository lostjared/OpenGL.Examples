
#include"gl_window.hpp"

int glWindow::create(std::string name, int w, int h) {
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    window = glfwCreateWindow(w, h, name.c_str(),0,0);
    if(!window) return 0;
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK)
        exit(EXIT_FAILURE);
    glfwSwapInterval(1);
    init();
    return 1;
}

void glWindow::loop() {
    while(!glfwWindowShouldClose(window)) {
        update(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int ShaderProgram::printShaderLog(GLuint shader) {
    int len = 0;
    int ch = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = new char [len+1];
        glGetShaderInfoLog(shader, len, &ch, log);
        std::cout << "Shader: " << log << "\n";
        delete [] log;
    }
    return 0;
}
void ShaderProgram::printProgramLog(int p) {
    int len = 0;
    int ch = 0;
    char *log;
    glGetShaderiv(p, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = new char [len+1];
        glGetProgramInfoLog(p, len, &ch, log);
        std::cout << "Program: " << log << "\n";
        delete [] log;
    }
}
bool ShaderProgram::checkError() {
    bool e = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR) {
        std::cout << "GL Error: " << glErr << "\n";
        e = true;
        glErr = glGetError();
    }
    return e;
}

GLuint ShaderProgram::createProgram(const char *vshaderSource, const char *fshaderSource) {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    
    glCompileShader(vShader);
    checkError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    
    if(vertCompiled != 1) {
        std::cout << "Error on Vertex compile\n";
        printShaderLog(vShader);
    }
    
    glCompileShader(fShader);
    checkError();
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    
    if(fragCompiled != 1) {
        std::cout << "Error on Fragment compile\n";
        printShaderLog(vShader);
    }
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1) {
        std::cout << "Linking failed...\n";
        printProgramLog(vfProgram);
    }
    return vfProgram;
}
