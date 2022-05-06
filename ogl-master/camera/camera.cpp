#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"

glm::mat4 MVPFromTime(double time) {
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 ViewMatrix = glm::lookAt(
            glm::vec3(glm::sin(time), 0, glm::cos(time)), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    return MVP;
}

void drawtriangle(GLuint ID, GLuint vertex, GLuint matrix, GLfloat *MVP) {
    // Use our shader
    glUseProgram(ID);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, MVP);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    glDisableVertexAttribArray(0);
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Home Work 1", NULL, NULL);
    if( window == nullptr ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // белый фон
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    GLuint VertexArrayID[2];
    glGenVertexArrays(2, VertexArrayID);
    glBindVertexArray(VertexArrayID[0]);
    glBindVertexArray(VertexArrayID[1]);

    // Create and compile our GLSL program from the shaders
    GLuint redID = LoadShaders( "SimpleVertexShader.vertexshader", "red.fragmentshader" );
    GLuint blueID = LoadShaders( "SimpleVertexShader.vertexshader", "blue.fragmentshader" );

    GLuint redmatrix = glGetUniformLocation(redID, "MVP");
    GLuint bluematrix = glGetUniformLocation(blueID, "MVP");

    static const GLfloat red_triangle[] = {
            0.15f, 0.0f, 0.0f,
            -0.45f, 0.45f, 0.0f,
            -0.45f, -0.45f, 0.0f, // red triangle
    };
    static const GLfloat blue_triangle[] ={
            -0.15f, 0.0f, 0.0f,
            0.45f, 0.45f, 0.0f,
            0.45f,  -0.45f, 0.0f, // blue triangle
    };

    GLuint vertexbuffer[2];
    glGenBuffers(2, &vertexbuffer[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(red_triangle), red_triangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blue_triangle), blue_triangle, GL_STATIC_DRAW);\


    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime() / 3;
        glm::mat4 MVP = MVPFromTime(time);
        drawtriangle(redID, vertexbuffer[0], redmatrix, &MVP[0][0]);
        drawtriangle(blueID, vertexbuffer[1], bluematrix, &MVP[0][0]);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO
    glDeleteBuffers(2, vertexbuffer);
    glDeleteVertexArrays(2, VertexArrayID);
    glDeleteProgram(redID);
    glDeleteProgram(blueID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}