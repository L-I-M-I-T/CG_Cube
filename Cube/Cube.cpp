//1853971 王天 彩色立方体
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int WIDTH = 1024;
unsigned int HEIGHT = 800;
glm::vec3 lightpos = glm::vec3(0.6f, 0.2f, 1.0f);
glm::vec3 lightcolor = glm::vec3(0.8f, 0.8f, 0.8f);
glm::mat4 projection = glm::perspective(45.0f, float(WIDTH) / float(HEIGHT), 1.0f, 10.0f);
glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 model = glm::mat4(1.0f);

class Shader
{
public:
    unsigned int ID;
    Shader(const char* verPath, const char* fragPath)
    {
        string vertexCode, fragmentCode;
        ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try
        {
            vShaderFile.open(verPath);
            fShaderFile.open(fragPath);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            cout << "vertex着色器内容如下：\n" << vertexCode << endl;
            cout << endl;
            cout << "fragment着色器内容如下：\n" << fragmentCode << endl;
            cout << endl;
        }
        catch (ifstream::failure error)
        {
            cout << "错误：Shader读取文件失败" << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "vertex");
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(vertex, "fragment");
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use()
    {
        glUseProgram(ID);
    }
    void setInt(const string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setInt(const string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void SetMat(const string& name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
    void SetVec(const string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "错误：Shader编译错误: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "错误：Shader链接错误: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)//回调函数，适应窗口大小调整
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
    projection = glm::perspective(45.0f, float(WIDTH) / float(HEIGHT), 1.0f, 10.0f);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float vertices[] =//描述立方体的顶点的位置和颜色
{
    -0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,0.0,0.0,1.0,//红0
    -0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,0.0,-1.0,0.0,//红1
    -0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,-1.0,0.0,0.0,//红2
    0.5f,-0.5f,0.5f,1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,//粉3
    0.5f,-0.5f,0.5f,1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f,//粉4
    0.5f,-0.5f,0.5f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,//粉5
    -0.5f,0.5f,0.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,//黄6
    -0.5f,0.5f,0.5f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,//黄7
    -0.5f,0.5f,0.5f,1.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,//黄8
    0.5f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,//白9
    0.5f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,//白10
    0.5f,0.5f,0.5f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,//白11
    0.5f,-0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.0f,-1.0f,//蓝12
    0.5f,-0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,-1.0f,0.0f,//蓝13
    0.5f,-0.5f,-0.5f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,//蓝14
    0.5f,0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,-1.0f,//青15
    0.5f,0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,//青16
    0.5f,0.5f,-0.5f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,//青17
    -0.5f,-0.5f,-0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,//黑18
    -0.5f,-0.5f,-0.5f,0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,//黑19
    -0.5f,-0.5f,-0.5f,0.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,//黑20
    -0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,-1.0f,//绿21
    -0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,//绿22
    -0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,//绿23
};

unsigned int indices[] =//使用检索方式指定绘制三角形顺序 
{
    6,0,3,
    6,3,9,
    11,5,14,
    11,14,17,
    15,12,18,
    15,18,21,
    23,20,2,
    23,2,8,
    22,7,10,
    22,10,16,
    1,19,13,
    1,13,4
};


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ColorfulCube", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //定义顶点位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    //定义顶点颜色属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    //定义顶点法向属性
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    Shader myshader("cube.vert", "cube.frag");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        myshader.SetMat("Projection", projection);
        myshader.SetMat("View", view);
        myshader.SetMat("Model", model);
        myshader.SetVec("LightPos", lightpos);
        myshader.SetVec("LightColor", lightcolor);
        myshader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}
