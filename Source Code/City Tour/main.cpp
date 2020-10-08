//Christopher Yefta
//James Adhitthana
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include "Shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <MMSystem.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// banyaknya pemetaan gedung
float range = 99.0f;

// view mode
int mode = 0;

int random(int i)
{
    return rand()%i+1;
}

vector<int> generatepos()
{
    vector<int> pos;
    for(int i=range; i>=-(range); i-=3)
        {
            for(int j= -(range); j<=range; j+=3)
            {
                pos.push_back(random(4));
            }
        }
    return pos;
}

vector<int> generatesprite()
{
    vector<int> pos;
    for(int i=range; i>=-(range); i-=3)
        {
            for(int j= -(range); j<=range; j+=3)
            {
                pos.push_back(random(10));
            }
        }
    return pos;
}

int main()
{
    // choose mode
    cout << "--------------------- City Tour ---------------------" << endl << endl;
    cout << "Choose city tour mode: " << endl;
    cout << "1. Car mode" << endl;
    cout << "2. Helicopter mode" << endl;
    cout << "3. Free mode" << endl << endl;

    while(true)
    {
        cout << ">> ";
        cin >> mode;

        if(mode == 1 || mode == 2 || mode == 3)
        {
            break;
        }
        else
        {
            cout << "Error!" << endl;
        }
    }

    cout << "Thank you!!" << endl;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "City Tour", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

     // build and compile shaders
    // -------------------------
    Shader shader("texture.vs", "texture.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices_1[] = {
        // positions          // texture Coords

        // yang paling jauh dari kamera, z-nya minus
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  2.5f, -0.5f,  1.0f, 0.0f,

         0.5f,  2.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  2.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // yang bagian depannya
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  2.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  2.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  2.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // bagian kiri
        -0.5f,  2.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  2.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  2.5f,  0.5f,  0.0f, 0.0f,

        // bagian kanan
         0.5f,  2.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  2.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  2.5f,  0.5f,  0.0f, 0.0f,

         // bagian bawah
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // bagian atas
        -0.5f,  2.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  2.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  2.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  2.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  2.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  2.5f, -0.5f,  0.0f, 1.0f
    };
    float cubeVertices_1_1[] = {
        // positions          // texture Coords

        // yang paling jauh dari kamera, z-nya minus
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.0f, -0.5f,  1.0f, 0.0f,

         0.5f,  1.0f, -0.5f,  1.0f, 0.0f,
        -0.5f,  1.0f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // yang bagian depannya
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // bagian kiri
        -0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.0f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  1.0f,  0.5f,  0.0f, 0.0f,

        // bagian kanan
         0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
         0.5f,  1.0f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  1.0f,  0.5f,  0.0f, 0.0f,

         // bagian bawah
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // bagian atas
        -0.5f,  1.0f, -0.5f,  0.0f, 1.0f,
         0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.0f, -0.5f,  0.0f, 1.0f
    };
    float cubeVertices_2[] = {
        // positions          // texture Coords

        // yang paling jauh dari kamera, z-nya minus
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,

         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // yang bagian depannya
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // bagian kiri
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,

        // bagian kanan
         0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  0.0f, 0.0f,

         // bagian bawah
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // bagian atas
        -0.5f,  1.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  0.0f, 1.0f
    };
    float cubeVertices_3[] = {
        // positions          // texture Coords

        // yang paling jauh dari kamera, z-nya minus
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  3.5f, -0.5f,  1.0f, 0.0f,

         0.5f,  3.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  3.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // yang bagian depannya
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  3.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  3.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  3.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // bagian kiri
        -0.5f,  3.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  3.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  3.5f,  0.5f,  0.0f, 0.0f,

        // bagian kanan
         0.5f,  3.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  3.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  3.5f,  0.5f,  0.0f, 0.0f,

         // bagian bawah
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // bagian atas
        -0.5f,  3.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  3.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  3.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  3.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  3.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  3.5f, -0.5f,  0.0f, 1.0f
    };
    float cubeVertices_3_1[] = {
        // positions          // texture Coords

        // yang paling jauh dari kamera, z-nya minus
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,

         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // yang bagian depannya
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // bagian kiri
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,

        // bagian kanan
         0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  0.0f, 0.0f,

         // bagian bawah
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // bagian atas
        -0.5f,  1.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  1.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  1.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  1.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  1.5f, -0.5f,  0.0f, 1.0f
    };


    float planeVertices[] = {
        // positions          // texture Coords

        // x, y, z, coordinate texture
         1.0f, -0.5f,  1.0f,  0.0f, 0.0f,
        -1.0f, -0.5f,  1.0f,  1.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 1.0f,

         1.0f, -0.5f,  1.0f,  1.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  1.0f, 1.0f,
         1.0f, -0.5f, -1.0f,  0.0f, 1.0f
    };
    float planeVertices2[] = {
        // positions          // texture Coords

        // x, y, z, coordinate texture
         1.0f, -0.5f,  1.0f,  0.0f, 1.0f,
        -1.0f, -0.5f,  1.0f,  1.0f, 1.0f,
        -1.0f, -0.5f, -1.0f,  1.0f, 0.0f,

         1.0f, -0.5f,  1.0f,  0.0f, 1.0f,
        -1.0f, -0.5f, -1.0f,  1.0f, 0.0f,
         1.0f, -0.5f, -1.0f,  0.0f, 0.0f
    };

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.0f,  0.0f,  1.0f,  0.0f
    };
    // cube VAO
    // yang pertama
    unsigned int cubeVAO_1, cubeVAO_1_1, cubeVAO_2, cubeVAO_3, cubeVAO_3_1,cubeVBO;
    glGenVertexArrays(1, &cubeVAO_1);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO_1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices_1), &cubeVertices_1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // sub building
    glGenVertexArrays(1, &cubeVAO_1_1);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO_1_1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices_1_1), &cubeVertices_1_1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // yang kedua
    glGenVertexArrays(1, &cubeVAO_2);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO_2);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices_2), &cubeVertices_2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // yang ketiga
    glGenVertexArrays(1, &cubeVAO_3);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO_3);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices_3), &cubeVertices_3, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // sub building
    glGenVertexArrays(1, &cubeVAO_3_1);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO_3_1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices_3_1), &cubeVertices_3_1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // plane VAO
    unsigned int planeVAO, planeVAO2, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //
    glGenVertexArrays(1, &planeVAO2);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO2);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices2), &planeVertices2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // transparent VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
     // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture_1 = loadTexture("textures/building1.jpg");
    unsigned int cubeTexture_2 = loadTexture("textures/building2.jpg");
    unsigned int cubeTexture_3 = loadTexture("textures/building3.jpg");
    unsigned int cubeTexture_4 = loadTexture("textures/building4.jpg");
    unsigned int topTexture_1 = loadTexture("textures/top1.jpg");
    unsigned int topTexture_2 = loadTexture("textures/top2.jpg");
    unsigned int topTexture_3 = loadTexture("textures/top3.jpg");
    unsigned int topTexture_4 = loadTexture("textures/top4.jpg");
    unsigned int floorTexture_1 = loadTexture("textures/roads/plaza.jpg");
    unsigned int floorTexture_2 = loadTexture("textures/roads/road2.jpg");
    unsigned int floorTexture_3 = loadTexture("textures/roads/road3.jpg");
    unsigned int floorTexture_4 = loadTexture("textures/roads/road4.jpg");
    unsigned int spriteTexture_1 = loadTexture("textures/car.png");
    unsigned int spriteTexture_2 = loadTexture("textures/car2.png");
    unsigned int spriteTexture_3 = loadTexture("textures/people.png");
    unsigned int sunTexture = loadTexture("textures/sun.png");
    unsigned int moonTexture = loadTexture("textures/moon.png");

    vector<std::string> faces
    {
        ("skybox/miramar_ft.jpg"),
        ("skybox/miramar_bk.jpg"),
        ("skybox/miramar_up.jpeg"),
        ("skybox/miramar_dn.jpg"),
        ("skybox/miramar_rt.jpg"),
        ("skybox/miramar_lf.jpg")
    };
    vector<std::string> faces2
    {
        ("skybox/miramar_ft(1).jpg"),
        ("skybox/miramar_bk(1).jpg"),
        ("skybox/miramar_up(1).jpeg"),
        ("skybox/miramar_dn(1).jpg"),
        ("skybox/miramar_rt(1).jpg"),
        ("skybox/miramar_lf(1).jpg")
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    // untuk generate jenis gedung
    vector<int> v = generatepos();
    // untuk generate jenis sprite
    vector<int> v2 = generatesprite();

    camera.Position.x = 1.5;
    camera.Position.z = 1.5;

    // untuk mengganti hari
    int day = 1;
    int day2 = 1;
    int timed = 0;
    int direction = 1;

    // Lagu
    if(mode == 1)
        PlaySound("audio/carMode.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    else if(mode == 2)
        PlaySound("audio/heliMode.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    else
        PlaySound("audio/carMode.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 700.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // cubes
        //glBindVertexArray(cubeVAO_1);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, cubeTexture_1);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //shader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        int ix =0;
        int value=0;
        for(int i=range; i>=-(range); i-=3)
        {
            for(int j= -(range); j<=range; j+=3)
            {
                value = v[ix];
                ix++;
                if(value == 1)
                {
                    glBindVertexArray(cubeVAO_2);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_2);
                }
                else if(value == 2)
                {
                    glBindVertexArray(cubeVAO_3_1);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_3);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f+0.2f, 0.0f, j*1.0f+0.2f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(cubeVAO_2);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_3);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f-0.2f, 0.0f, j*1.0f-0.2f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);

                    glBindVertexArray(cubeVAO_3);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_3);
                }
                else if(value == 3)
                {
                    glBindVertexArray(cubeVAO_1);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_4);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 0.0f, j*1.0f));
                    model = glm::scale(model, glm::vec3(1.3f, 1.0f, 1.3f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                else
                {
                    glBindVertexArray(cubeVAO_1_1);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_1);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 0.0f, j*1.0f));
                    model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.2f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);

                    glBindVertexArray(cubeVAO_1);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture_1);
                }
                model = glm::mat4();
                model = glm::translate(model, glm::vec3(i*1.0f, 0.0f, j*1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // untuk bagian atas gedung
                glBindVertexArray(planeVAO);
                if(value == 1)
                {
                    glBindTexture(GL_TEXTURE_2D, topTexture_1);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 2.0005f, j*1.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                else if(value == 2)
                {
                    glBindTexture(GL_TEXTURE_2D, topTexture_2);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 4.0005f, j*1.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                else if(value == 3)
                {
                    glBindTexture(GL_TEXTURE_2D, topTexture_3);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 3.0005f, j*1.0f));
                    model = glm::scale(model, glm::vec3(0.65f, 1.0f, 0.65f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, topTexture_4);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 3.0005f, j*1.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
            }
        }

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture_1);
        model = glm::mat4();
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        for(int i=range; i>=-(range); i-=3)
        {
            for(int j= -(range); j<=range; j+=3)
            {
                model = glm::mat4();
                model = glm::translate(model, glm::vec3(i*1.0f, 0.005f, j*1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        for(float i=range; i>=-(range); i-=3)
        {
            for(float j= -(range-1.5); j<=(range-1.5); j+=3)
            {
                glBindTexture(GL_TEXTURE_2D, floorTexture_2);
                model = glm::mat4();
                model = glm::translate(model, glm::vec3(i*1.0f, 0.0f, j*1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        for(float i=range; i>=-(range); i-=3)
        {
            for(float j= -(range-1.5); j<=(range-1.5); j+=3)
            {
                glBindTexture(GL_TEXTURE_2D, floorTexture_3);
                model = glm::mat4();
                model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        for(float i=(range-1.5); i>=-(range-1.5); i-=3)
        {
            for(float j= -(range-1.5); j<=(range-1.5); j+=3)
            {
                glBindTexture(GL_TEXTURE_2D, floorTexture_4);
                model = glm::mat4();
                model = glm::translate(model, glm::vec3(j*1.0f, 0.0007f, i*1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        // sprite
        ix =0;
        value=0;

        //glBindVertexArray(transparentVAO);
        //glBindTexture(GL_TEXTURE_2D, spriteTexture_1);
        for(float i=range; i>=-(range); i-=3)
        {
            for(float j= -(range-1.5); j<=(range-1.5); j+=3)
            {
                value = v2[ix];
                ix++;
                if(value == 1)
                {
                    glBindVertexArray(transparentVAO);
                    glBindTexture(GL_TEXTURE_2D, spriteTexture_1);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, glfwGetTime()*0.1f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                else if(value == 2)
                {
                    glBindVertexArray(transparentVAO);
                    glBindTexture(GL_TEXTURE_2D, spriteTexture_2);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f));
                    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, glfwGetTime()*-0.1f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                else if(value == 3)
                {
                    glBindVertexArray(transparentVAO);
                    glBindTexture(GL_TEXTURE_2D, spriteTexture_3);
                    model = glm::mat4();
                    model = glm::translate(model, glm::vec3(i*1.0f, 0.0f, j*1.0f));
                    model = glm::rotate(model, -4.75f, glm::vec3(0.0f, 1.0f, 0.0f));
                    model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.25f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, glfwGetTime()*0.01f));
                    shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
            }
        }


        // mengganti skybox
        timed++;
        if(timed%1000 == 0)
        {
            day = (-1)*day;
        }
        if(day != day2 && day == 1)
        {
            cubemapTexture = loadCubemap(faces);
            day2 = day;
        }
        if(day != day2 && day == -1)
        {
            cubemapTexture = loadCubemap(faces2);
            day2 = day;
        }

        // matahari
        float sunrotate = (float)timed*0.00314f ;

        glBindVertexArray(planeVAO2);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 200.0f, 200.0f));
        model = glm::translate(model, glm::vec3(0.0f, -200.0f, -200.0f));
        model = glm::rotate(model, 1.55f, glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, sunrotate, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 200.0f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // bulan
        float moonrotate = (float)timed*0.00314f ;

        glBindVertexArray(planeVAO2);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 200.0f, 200.0f));
        model = glm::translate(model, glm::vec3(0.0f, -200.0f, -200.0f));
        model = glm::rotate(model, 1.55f, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, sunrotate, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 200.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // autocam

        // car mode
        if(mode == 1)
        {
            if(timed % 150 == 0)
            {
                direction++;
            }
            if(direction > 13)
            {
                direction = 2;
            }
            if(direction == 1 || direction == 3 || direction == 7 || direction == 10)
            {
                camera.Position.z -= 0.1;
            }
            if(direction == 5 || direction == 12 || direction == 13)
            {
                camera.Position.z += 0.1;
            }
            if(direction == 2 || direction == 4 || direction == 6)
            {
                camera.Position.x -= 0.1;
            }
            if(direction == 8 || direction == 9 || direction == 11)
            {
                camera.Position.x += 0.1;
            }
        }
        // helicopter mode
        else if(mode == 2)
        {
            if(timed % 150 == 0)
            {
                direction++;
            }
            if(direction > 13)
            {
                direction = 2;
            }

            if(direction == 1)
            {
                camera.Position.y += 0.03;
            }
            if(direction == 3 || direction == 7 || direction == 10)
            {
                camera.Position.z -= 0.1;
            }
            if(direction == 5 || direction == 12 || direction == 13)
            {
                camera.Position.z += 0.1;
            }
            if(direction == 2 || direction == 4 || direction == 6)
            {
                camera.Position.x -= 0.1;
            }
            if(direction == 8 || direction == 9 || direction == 11)
            {
                camera.Position.x += 0.1;
            }
        }
        // free mode
        else
        {
            if(camera.Position.y < 0)
            {
                camera.Position.y = 0;
            }
            if(camera.Position.x < -99)
            {
                camera.Position.x = -99;
            }
            if(camera.Position.x > 99)
            {
                camera.Position.x = 99;
            }
            if(camera.Position.z < -99)
            {
                camera.Position.z = -99;
            }
            if(camera.Position.z > 99)
            {
                camera.Position.z = 99;
            }
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO_1);
    glDeleteVertexArrays(1, &cubeVAO_2);
    glDeleteVertexArrays(1, &cubeVAO_3);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(mode == 3)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

