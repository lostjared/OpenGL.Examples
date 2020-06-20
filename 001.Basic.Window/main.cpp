#include"gl_window.hpp"

class MainWindow : public glWindow {
    virtual void init() override {
        
    }
    virtual void update(double timeval) override {
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
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
