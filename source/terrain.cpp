#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <includes/stb_image.h>
#include <includes/glm/glm.hpp>
#include <includes/glm/gtc/matrix_transform.hpp>
#include <includes/glm/gtc/type_ptr.hpp>

#include <filesystem.h>
#include <shader_s.h>
#include <terraingen.h>

#include <iostream>
#include <cmath>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
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
    std::vector<glm::vec4> cubePositions = TerrainGen::getCoords();

    // set up VBO, EBO, VAO
    // --------------------
    unsigned int VBO, VAO, EBO;
    // initialize vertex buffer object
    glGenBuffers(1, &VBO);
    // initialize vertex array object
    glGenVertexArrays(1, &VAO);
    // initialize element buffer object
    glGenBuffers(1, &EBO);
    // bind vertex array object
    glBindVertexArray(VAO);
    // bind newly created buffer with GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    // copies the previously defined vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // now do the same for sides, top, bottom
    unsigned int VBO_SIDES, VAO_SIDES, EBO_SIDES;
    // initialize vertex buffer object
    glGenBuffers(1, &VBO_SIDES);
    // initialize vertex array object
    glGenVertexArrays(1, &VAO_SIDES);
    // initialize element buffer object
    glGenBuffers(1, &EBO_SIDES);
    // bind vertex array object
    glBindVertexArray(VAO_SIDES);
    // bind newly created buffer with GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO_SIDES);  
    // copies the previously defined vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(sides), sides, GL_STATIC_DRAW);
        // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    unsigned int VBO_TOP, VAO_TOP, EBO_TOP;
    // initialize vertex buffer object
    glGenBuffers(1, &VBO_TOP);
    // initialize vertex array object
    glGenVertexArrays(1, &VAO_TOP);
    // initialize element buffer object
    glGenBuffers(1, &EBO_TOP);
    // bind vertex array object
    glBindVertexArray(VAO_TOP);
    // bind newly created buffer with GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TOP);  
    // copies the previously defined vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);


/*     
    // bind the EBO and copy the indices into the buffer 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
*/

    // how to interpret vertex data
    // ----------------------------
    // (attribute, size of attr, type of data, normalize, stride (space between consecutive vertex attribute sets), offset)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load textures 1 and 2
    // ---------------------
    unsigned int dirt, grass_side, grass_top;
    int width, height, nrChannels;
    // create texture 1 
    glGenTextures(1, &dirt);
    // bind so any subsequent texture commands will configure the currently bound texture
    glBindTexture(GL_TEXTURE_2D, dirt);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    // s, t, r are the coordinates for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load the texture
    unsigned char *data = stbi_load(FileSystem::getPath("source/textures/dirt.png").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // generate(target, mipmap level, format, w, h, 0, src_format, src_datatype, img_data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
 
    // create texture 2
    glGenTextures(1, &grass_side);
    // bind so any subsequent texture commands will configure the currently bound texture
    glBindTexture(GL_TEXTURE_2D, grass_side);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    // s, t, r are the coordinates for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load the texture
    // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load(FileSystem::getPath("source/textures/grass_side.png").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // generate(target, mipmap level, format, w, h, 0, src_format, src_datatype, img_data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // free the image memory
    stbi_image_free(data);

    // create texture 3
    glGenTextures(1, &grass_top);
    // bind so any subsequent texture commands will configure the currently bound texture
    glBindTexture(GL_TEXTURE_2D, grass_top);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    // s, t, r are the coordinates for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load the texture
    // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load(FileSystem::getPath("source/textures/grass_top.png").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // generate(target, mipmap level, format, w, h, 0, src_format, src_datatype, img_data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // free the image memory
    stbi_image_free(data);
 

    // activate shader before setting uniforms
    ourShader.use();
    // set it manually like so:
    //glUniform1i(glGetUniformLocation(ourShader.ID, "dirt"), 0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // define the transformation matrix
        // init to identity matrix
        glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); 

        ourShader.use();

        // model matrix: rotate around all axes
        glm::mat4 model;
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // view matrix: move the scene backwards
        glm::mat4 view;
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, -5.0f, -30.0f)); 
        //view = glm::rotate(view, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // projection matrix: (fov, aspect, near, far)
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        unsigned int projectionLoc  = glGetUniformLocation(ourShader.ID, "projection");
        // pass them to the shaders (2 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // (do not need to be set each frame)
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        int len = cubePositions.size();
        for(unsigned int i = 0; i < len; i++) {
            glm::mat4 model;
            glm::vec4 translation = cubePositions[i];
            model = glm::translate(model, glm::vec3(translation.x, translation.y, translation.z));
            //printf("x: %d y: %d z: %d", translation.x, translation.y, translation.z);
            float angle = 20.0f * (i + 1.0f); 
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            if (translation.w == 0) {
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, dirt);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            } else if (translation.w == 1) {
                glBindVertexArray(VAO_SIDES);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, grass_side);
                glDrawArrays(GL_TRIANGLES, 0, 24);
                glBindVertexArray(VAO_TOP);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, grass_top);
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
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO_SIDES);
    glDeleteBuffers(1, &VBO_SIDES);
    glDeleteBuffers(1, &EBO_SIDES);
    glDeleteVertexArrays(1, &VAO_TOP);
    glDeleteBuffers(1, &VBO_TOP);
    glDeleteBuffers(1, &EBO_TOP);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}