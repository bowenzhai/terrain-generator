#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <includes/glm/glm.hpp>
#include <includes/glm/gtc/matrix_transform.hpp>
#include <includes/glm/gtc/type_ptr.hpp>

#include <filesystem.h>
#include <shader.h>
#include <texture.h>
#include <terraingen.h>
#include <camera.h>

#include <iostream>
#include <vector>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 30.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 1.0f,  0.0f));

// view matrix: move the scene backwards
glm::mat4 view;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// initializes the VBO and VAO according to vertex attributes
void initVBOVAO(unsigned int *VBO, unsigned int *VAO, float vertices[], int size) {
    // initialize vertex buffer object
    glGenBuffers(1, VBO);
    // initialize vertex array object
    glGenVertexArrays(1, VAO);
    // bind vertex array object
    glBindVertexArray(*VAO);
    // bind newly created buffer with GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);  
    // copies the previously defined vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // position attribute
    // how to interpret vertex data
    // ----------------------------
    // (attribute, size of attr, type of data, normalize, stride (space between consecutive vertex attribute sets), offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

int main(int argc, char** argv) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) exit(EXIT_FAILURE);
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader(FileSystem::getPath("source/shaders/vertex.vs").c_str(), FileSystem::getPath("source/shaders/fragment.fs").c_str());

    // set up vertex data 
    // ------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float sides[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  -1.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  -1.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  -1.0f, -1.0f,
        0.5f,  0.5f,  0.5f,  -1.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };

    float bottom[] {
        // bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    };

    float top[] {
        // top
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // world space positions of our cubes
    std::vector<glm::vec4> cubePositions;
    if (argc == 1) {
        cubePositions = TerrainGen::getCoords();
    } else if (argc == 2) {
        std::string arg1(argv[1]);
        cubePositions = TerrainGen::getCoords(stoi(arg1));
    } else if (argc == 3) {
        std::string arg1(argv[1]);
        std::string arg2(argv[2]);
        cubePositions = TerrainGen::getCoords(stoi(arg1), stoi(arg2));
    }

    // set up VBO, VAO
    // ---------------
    unsigned int VBO, VAO;
    initVBOVAO(&VBO, &VAO, vertices, sizeof(vertices));

    // now do the same for sides, top
    unsigned int VBO_SIDES, VAO_SIDES;
    initVBOVAO(&VBO_SIDES, &VAO_SIDES, sides, sizeof(sides));
    
    unsigned int VBO_TOP, VAO_TOP;
    initVBOVAO(&VBO_TOP, &VAO_TOP, top, sizeof(top));

    // create textures 1, 2, 3
    // -----------------------
    Texture dirt;
    dirt.gen();
    dirt.bind();
    dirt.setOptions();
    dirt.load(FileSystem::getPath("source/textures/dirt.png").c_str());

    Texture grass_side;
    grass_side.gen();
    grass_side.bind();
    grass_side.setOptions();
    grass_side.load(FileSystem::getPath("source/textures/grass_side.png").c_str());

    Texture grass_top;
    grass_top.gen();
    grass_top.bind();
    grass_top.setOptions();
    grass_top.load(FileSystem::getPath("source/textures/grass_top.png").c_str());

    // activate shader before setting uniforms
    ourShader.use();

    // model matrix: rotate around all axes
    glm::mat4 model;
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    // note that we're translating the scene in the reverse direction of where we want to move
    view = camera.getViewMatrix(); 

    // projection matrix: (fov, aspect, near, far)
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
    unsigned int projectionLoc  = glGetUniformLocation(ourShader.ID, "projection");
    // pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // (do not need to be set each frame)
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // update view information
        view = camera.getViewMatrix();
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // draw each block
        int len = cubePositions.size();
        for (unsigned int i = 0; i < len; i++) {
            glm::mat4 model;
            glm::vec4 translation = cubePositions[i];
            model = glm::translate(model, glm::vec3(translation.x, translation.y, translation.z));
            float angle = 20.0f * (i + 1.0f); 
            unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            if (translation.w == 0) {
                // not a top block
                glBindVertexArray(VAO);
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                dirt.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            } else if (translation.w == 1) {
                // is a top block
                glBindVertexArray(VAO_SIDES);
                glActiveTexture(GL_TEXTURE0);
                grass_side.bind();
                glDrawArrays(GL_TRIANGLES, 0, 24);
                glBindVertexArray(VAO_TOP);
                glActiveTexture(GL_TEXTURE0);
                grass_top.bind();
                glDrawArrays(GL_TRIANGLES, 0, 12);
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // de-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO_SIDES);
    glDeleteBuffers(1, &VBO_SIDES);
    glDeleteVertexArrays(1, &VAO_TOP);
    glDeleteBuffers(1, &VBO_TOP);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboardInput(key_w);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboardInput(key_a);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboardInput(key_s);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboardInput(key_d);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
