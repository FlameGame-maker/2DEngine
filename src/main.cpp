#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

int windowWidth, windowHeight;

void getResolution() {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    windowWidth = mode->width;
    windowHeight = mode->height;
}

void sizeCallback(GLFWwindow* pWindow, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0,windowWidth, windowHeight);
}

void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, 0);
    }
}

int main(void) {
    if (!glfwInit()){//Инициализация glfw
        std::cout << "glfw initialisation failed" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    getResolution();
    GLFWwindow* pWindow = glfwCreateWindow(windowWidth, windowHeight, "2DEngine", nullptr, nullptr);//Create a windowed mode window and its OpenGL context

    if (!pWindow){//Инициализация окна
        glfwTerminate();
        std::cout << "glfwCreateWIndow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, sizeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);

    glfwMakeContextCurrent(pWindow);//Make the window's context current

   if(!gladLoadGL()){//Инициализация GLAD
      std::cout << "GLAD initialisation failed" << std::endl;
      return -1;
   }
  
   std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
   std::cout << "OpenGl min version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
   std::cout << "Current OpenGl version: " << glGetString(GL_VERSION) << std::endl;

   glClearColor(1, 1, 0, 1);
   float color1 = 1, color2 = 0, color3 = 0;
   int step = 0;
   std::chrono::milliseconds timespan(5);

    while (!glfwWindowShouldClose(pWindow)){//Render loop, отобржается, пока окно не будет закрыто
        glClearColor(color1, color2, color3, 1);
        glClear(GL_COLOR_BUFFER_BIT);//Render here
        glfwSwapBuffers(pWindow);//Swap front and back buffers
        glfwPollEvents();//Обрабатывает события (позиция курсора, закрытие окна, нажатие клавиш
        
        switch (step) {
            case 0:
                color2 += 0.005;
                if (color2 >= 1) step = 1;
            break;
            case 1:
                color1 -= 0.005;
                if (color1 <= 0) step = 2;
             break;
            case 2:
                color3 += 0.005;
                if (color3 >= 1) step = 3;
            break;
            case 3:
                color2 -= 0.005;
                if (color2 <= 0) step = 4;
            break;
            case 4:
                color1 += 0.005;
                if (color1 >= 1) step = 5;
            break;
            case 5:
                color3 -= 0.005;
                if (color3 <= 0) step = 0;
            break;
        }

        std::this_thread::sleep_for(timespan);
        /*std::cout << color1 << " " << color2;
        std::this_thread::sleep_for(timespan);
        system("cls");
        if (color == 1.0) {
            while (color >= 0.0) {
                color -= 0.1;
                std::cout << color;
                std::this_thread::sleep_for(timespan);
                std::cout << "\b\b\b";
            }
        }*/
    }

    glfwTerminate();
    return 0;
}