#include "Renderer/basicShader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

int windowWidth, windowHeight;
//GLfloat Coords[] = {
//    0.0f, 4.3f, 0.0f,
//    3.7f, 2.1f, 0.0f,
//    2.7f, -3.8f, 0.0f
//};

//GLfloat Coords[] = {
//    0.0f, 0.15f, 0.0f,
//    0.98f, 0.55f, 0.0f,
//    0.34f, 1.0f, 0.0f
//};

GLfloat Coords[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f
};
GLfloat Colors[] = {
    0.65f, 0.18f, 0.0f,
    1.0f, 0.85f, 0.3f,
    0.94f, 0.46f, 0.02f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";

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
        glfwSetWindowShouldClose(pWindow, 1);
    }
}

class Colorizer{
public:
    float c1, c2, c3;
    int stp;
    void GradientInitilise(float color1, float color2, float color3, int step) {
        c1 = color1;
        c2 = color2;
        c3 = color3;
        stp = step;
    }
    void Gradient() {
        switch (stp) {
        case 0:
            c2 += 0.005f;
            if (c2 >= 1) stp = 1;
            break;
        case 1:
            c1 -= 0.005f;
            if (c1 <= 0) stp = 2;
            break;
        case 2:
            c3 += 0.005f;
            if (c3 >= 1) stp = 3;
            break;
        case 3:
            c2 -= 0.005f;
            if (c2 <= 0) stp = 4;
            break;
        case 4:
            c1 += 0.005f;
            if (c1 >= 1) stp = 5;
            break;
        case 5:
            c3 -= 0.005f;
            if (c3 <= 0) stp = 0;
            break;
        }
    }
};

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

   std::string vertexShader(vertex_shader);
   std::string fragmentShader(fragment_shader);
   Renderer::basicShader basicShader(vertexShader, fragmentShader);
   if (basicShader.isCompiled()) std::cout << "Shaders compiled succesfuly" << std::endl;
   else std::cerr << "Error: cannot compile shaders" << std::endl;

   GLuint points_vbo = 0;
   glGenBuffers(1, &points_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Coords), Coords, GL_STATIC_DRAW);

   GLuint colors_vbo = 0;
   glGenBuffers(1, &colors_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

   GLuint vao = 0;
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   float col1 = 1, col2 = 0, col3 = 0;
   int step = 0;
   Colorizer Grdnt;
   Grdnt.GradientInitilise(col1, col2, col3, 0);
   std::chrono::milliseconds timespan(5);

    while (!glfwWindowShouldClose(pWindow)){//Render loop, отобржается, пока окно не будет закрыто
        glClearColor(col1, col2, col3, 1);
        glClear(GL_COLOR_BUFFER_BIT);//Render here

        basicShader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(pWindow);//Swap front and back buffers
        glfwPollEvents();//Обрабатывает события (позиция курсора, закрытие окна, нажатие клавиш)
        
        Grdnt.Gradient();
        col1 = Grdnt.c1;
        col2 = Grdnt.c2;
        col3 = Grdnt.c3;
        step = Grdnt.stp;

        std::this_thread::sleep_for(timespan);
    }

    glfwTerminate();
    return 0;
}