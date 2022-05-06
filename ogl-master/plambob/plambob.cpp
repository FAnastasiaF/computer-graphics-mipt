// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

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
	window = glfwCreateWindow( 1024, 768, "Plambob", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
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

    // Dark blue background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
            -0.25f,  0.0f,  0.0f,
            -0.125f, 0.0f, 0.216506f,
            0.0f, -0.625f, 0.0f, //
            0.125f, 0.0f, 0.216506f,
            -0.125f, 0.0f, 0.216506f,
            0.0f, -0.625f, 0.0f, //
            0.125f, 0.0f, 0.216506f,
            0.25f, 0.0f, 0.0f,
            0.0f, -0.625f, 0.0f, //

            -0.25f,  0.0f,  0.0f,
            -0.125f, 0.0f, -0.216506f,
            0.0f, -0.625f, 0.0f, //
            0.125f, 0.0f, -0.216506f,
            -0.125f, 0.0f, -0.216506f,
            0.0f, -0.625f, 0.0f, //
            0.125f, 0.0f, -0.216506f,
            0.25f, 0.0f, 0.0f,
            0.0f, -0.625f, 0.0f, //

            -0.25f,  0.0f,  0.0f,
            -0.125f, 0.0f, 0.216506f,
            0.0f, 0.625f, 0.0f, //
            0.125f, 0.0f, 0.216506f,
            -0.125f, 0.0f, 0.216506f,
            0.0f, 0.625f, 0.0f, //
            0.125f, 0.0f, 0.216506f,
            0.25f, 0.0f, 0.0f,
            0.0f, 0.625f, 0.0f, //

            -0.25f,  0.0f,  0.0f,
            -0.125f, 0.0f, -0.216506f,
            0.0f, 0.625f, 0.0f, //
            0.125f, 0.0f, -0.216506f,
            -0.125f, 0.0f, -0.216506f,
            0.0f, 0.625f, 0.0f, //
            0.125f, 0.0f, -0.216506f,
            0.25f, 0.0f, 0.0f,
            0.0f, 0.625f, 0.0f, //
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
            0.338f, 0.66f, 0.2f,
            0.387f, 0.968f, 0.44f,
            0.288f, 0.6f, 0.144f,
            0.266f, 0.66f, 0.46f,
            0.207f, 0.526f, 0.324f,
            0.305f, 0.694f, 0.203f,
            0.48f, 0.768f, 0.278f,
            0.172f, 0.991f, 0.121f,
            0.236f, 0.529f, 0.276f,

            0.187f, 0.751f, 0.371f,
            0.253f, 0.894f, 0.432f,
            0.417f, 0.911f, 0.309f,
            0.275f, 0.573f, 0.338f,
            0.187f, 0.671f, 0.144f,
            0.3f, 0.741f, 0.488f,
            0.204f, 0.503f, 0.261f,
            0.373f, 0.867f, 0.214f,
            0.46f, 0.844f, 0.287f,

            0.355f, 0.625f, 0.494f,
            0.252f, 0.622f, 0.175f,
            0.315f, 0.642f, 0.19f,
            0.491f, 0.696f, 0.185f,
            0.175f, 0.852f, 0.302f,
            0.351f, 0.844f, 0.135f,
            0.34f, 0.659f, 0.347f,
            0.226f, 0.689f, 0.446f,
            0.419f, 0.519f, 0.42f,

            0.48f, 0.927f, 0.193f,
            0.317f, 0.557f, 0.114f,
            0.241f, 0.872f, 0.243f,
            0.413f, 0.711f, 0.491f,
            0.395f, 0.925f, 0.292f,
            0.468f, 0.963f, 0.238f,
            0.28f, 0.776f, 0.168f,
            0.277f, 0.808f, 0.309f,
            0.407f, 0.96f, 0.361f

	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        double time = glfwGetTime() / 3;
        glm::mat4 MVP = MVPFromTime(time);
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

