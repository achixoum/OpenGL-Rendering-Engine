#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "stb_image.h"
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Sphere.h"
#include "Line.h"


using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

constexpr unsigned short width  = 1280;
constexpr unsigned short height = 720;

vec3 cameraPos   = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp    = vec3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos);

float lastX     = width/2.0;
float lastY     = height/2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {
    auto xPos = static_cast<float>(xPosIn);
    auto yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        firstMouse = false;
        lastX = xPos;
        lastY = yPos;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout<<"Failed to initialize GLAD"<<endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader mainShader("../Shaders/vertex.vert", "../Shaders/fragment.frag");

    mainShader.deleteVertexShader();
    mainShader.deleteFragmentShader();

    Shader lightingShader("../Shaders/lighting_source.vert", "../Shaders/lighting_source.frag");

    float vertices[] = {
    // POSITIONS          // COLORS           // TEXTURE COORDS

    // 1. Back Face (Red)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

    // 2. Front Face (Green)
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

    // 3. Left Face (Blue)
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

    // 4. Right Face (Yellow)
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,

    // 5. Bottom Face (Cyan)
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    // 6. Top Face (Magenta)
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(5.0f,  0.0f,  5.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };


    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
//    unsigned char* image = stbi_load("../Assets/Images/P6090015.JPG", &w, &h, &channels, 0);

    Sphere lightingSphere(3, vec3(0.0, 3.0f, 0.0f));
    lightingSphere.createVertices(100, 100);
    lightingSphere.createBuffers();
    lightingSphere.processAttribForBuffer();
    lightingSphere.setAttributes();


    Sphere sphere(2, vec3(0.0f,0.0f,0.0f));
    Line xAxis(vec3(0.0f,0.0f,0.0f),vec3(100.0f,0.0f,0.0f),"../Shaders/vertex.vert", "../Shaders/line_fragment.frag");
    Line yAxis(vec3(0.0f,0.0f,0.0f),vec3(0.0f,100.0f,0.0f),"../Shaders/vertex.vert", "../Shaders/line_fragment.frag");
    Line zAxis(vec3(0.0f,0.0f,0.0f),vec3(0.0f,0.0f,100.0f),"../Shaders/vertex.vert", "../Shaders/line_fragment.frag");

    xAxis.setAttributes();
    yAxis.setAttributes();
    zAxis.setAttributes();
    xAxis.setColor(vec3(1.0f,0.0f,0.0f));
    yAxis.setColor(vec3(0.0f,1.0f,0.0f));
    zAxis.setColor(vec3(0.0f,0.0f,1.0f));

    sphere.createVertices(100,100);
    sphere.createBuffers();
    sphere.processAttribForBuffer();
    sphere.setAttributes();
    sphere.printProperties();
    //generate the buffers
    unsigned int VBO, lightVBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &lightVBO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    unsigned int VAO, lightVAO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);

    //glEnable(GL_FRAMEBUFFER_SRGB);
    //initialize data for the shaders
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char* image = stbi_load("../Assets/Images/earth.jpg", &w, &h, &channels, 3);
    //set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    image = stbi_load("../Assets/Images/cringe.jpg", &w, &h, &channels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mainShader.use();
    mainShader.setInt("texture1", 0);
    mainShader.setInt("texture2", 1);

    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        //update delta time
        deltaTime = time - lastFrame;
        lastFrame = time;

        //process input
        processInput(window);

        //rendering
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float greenValue = sin(time) / 2.0f + 0.5f;
        mat4 trans = mat4(1.0f);
        mat4 trans2 = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 model = mat4(1.0f);

        //trans = scale(trans, vec3(2,2,1));
        //trans = translate(trans, vec3(0.5f, -0.5f, 0.0f));
        model = rotate(trans,radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
        projection = perspective(radians(camera.getFov()), (float) width/height, 0.1f, 100.0f);
        view = camera.getViewMatrix();
        float radius = 2.0f;
        float camX = cos(time*0.5f) * radius;
        float camZ = sin(time*0.5f) * radius;

        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor2");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        mat4 light_model(1.0f);

        lightingShader.use();
        lightingShader.setMat4("model", light_model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        mainShader.use();
        mainShader.setMat4("model", model);
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);

        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, value_ptr(trans));
        glBindVertexArray(VAO);
        //glUniform4f(vertexColorLocation, 0.0f, 0.0f, greenValue, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glUseProgram(shaderProgram);
        //int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "ourColor");
        //glUniform4f(vertexColorLocation2, 0.0f, greenValue, 0.0f, 1.0f);
        float angle = 45.0f;
        for (int i=0; i<1; i++) {
            mat4 model2 = mat4(1.0f);
            //model2 = translate(model2, cubePositions[i]);
            float angle2 = 15.0f * i;
            model2 = rotate(model2, radians(180.0f), vec3(1.0f, 0.0f, 1.0f));
            if (i%3 == 0) {
                //model2 = translate(model2, vec3(0,0,0));
                model2 = translate(model2, vec3(cubePositions[i].x*camX, cubePositions[i].y, cubePositions[i].z*camZ));
                model2 = rotate(model2,time * radians(angle), vec3(0.0f, 1.0f, 0.3f));
                //double scale_f = abs(sin(time*0.3)+1)*0.5;
                //model2 =  scale(model2, vec3(scale_f,scale_f,1.0f));
            }
            mainShader.setMat4("model", model2);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        /*
        mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        mainShader.setMat4("model", model);

        mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        mainShader.setMat4("model", model);
        */
        xAxis.draw(model, view, projection);
        yAxis.draw(model, view, projection);
        zAxis.draw(model, view, projection);
        mainShader.use();
        mainShader.setVec3("lightingSource", vec3(1.0f,1.0f,1.0f));
        mainShader.setVec3("lightPos",lightingSphere.getPos());
        mainShader.setVec3("viewPos", camera.getPos());
        sphere.draw();
        lightingShader.use();
        lightingShader.setVec3("color", vec3(1.0f,1.0f,1.0f));
        lightingSphere.draw();
        /*
        //drawing another triangle that has a popping effect
        trans2 = translate(trans2, vec3(-0.5f, 0.5f, 0.0f));
        trans2 = scale(trans2, vec3(fabs(sin(time)), fabs(sin(time)), 0.0f));
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, value_ptr(trans2));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/
        //glDrawElements(GL_TRIANGLES ,3 ,GL_UNSIGNED_INT, 0);

        //check call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    mainShader.deleteProgram();

    glfwTerminate();
    return 0;
}
