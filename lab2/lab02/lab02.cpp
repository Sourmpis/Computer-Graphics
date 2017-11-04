// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>   // glm::to_string

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();
void cubeCross(float);
void cubeVshape(float);
void cubeInt(float, int);
void ten();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Lab 02"

// Global variables
GLFWwindow* window;
GLuint shaderProgram;
GLuint MVPLocation;
GLuint triangleVAO, cubeVAO;
GLuint triangleVerticiesVBO, triangleColorsVBO, cubeVerticiesVBO, cubeColorsVBO;

void createContext()
{
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("transformation.vertexshader",
        "simple.fragmentshader");

    // Task: Get a pointer location to model matrix in the vertex shader
	MVPLocation = glGetUniformLocation(shaderProgram, "MVP"); 

    // triangle VAO
    glGenVertexArrays(1, &triangleVAO);
    glBindVertexArray(triangleVAO);

    // vertex VBO
    static const GLfloat triangleVertices[] = {
        0.0f, 0.2f, 0.0f,
        0.2f, -0.2f, 0.0f,
        -0.2f, -0.2f, 0.0f
    };
    glGenBuffers(1, &triangleVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    static const GLfloat triangleColors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    glGenBuffers(1, &triangleColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat cubeVertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

		1.0f,1.0f, 1.0f,
		-1.0f,1.0f, 1.0f,
		0.0f, 1.5f, 0.0f,

		1.0f,1.0f, 1.0f,
		1.0f,1.0f, -1.0f,
		0.0f, 1.5f, 0.0f,

		1.0f,1.0f, -1.0f,
		-1.0f,1.0f, -1.0f,
		0.0f, 1.5f, 0.0f,

		-1.0f,1.0f, 1.0f,
		-1.0f,1.0f, -1.0f,
		0.0f, 1.5f, 0.0f,
		
    };
    glGenBuffers(1, &cubeVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    static const GLfloat cubeColors[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f,
    };
    glGenBuffers(1, &cubeColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void free()
{
    glDeleteBuffers(1, &triangleVerticiesVBO);
    glDeleteBuffers(1, &triangleColorsVBO);
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &cubeVerticiesVBO);
    glDeleteBuffers(1, &cubeColorsVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
}

void mainLoop()
{
    // Task: transformation
    GLfloat matrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f, // first column
        0.0f, 1.0f, 0.0f, 0.0f, // second column
        0.0f, 0.0f, 1.0f, 0.0f, // third column
        0.0f, 0.0f, 0.0f, 1.0f	// fourth column
    };

 //   // Task: triangle scale
	//mat4 triangleScaling = glm::scale(mat4(), vec3(0.5, 10., 0.5));
 //   // cout << glm::to_string(triangleScaling) << endl;

 //   // Task: triangle rotate
	//mat4 triangleRotation = glm::rotate(mat4(), 3.14f / 3.0f, vec3(0, 0,1));
 //   // cout << glm::to_string(triangleRotation) << endl;

 //   // Task: triangle translate
	//mat4 triangleTranslation = glm::translate(mat4(), vec3(0.5, 0, 0));
 //   // cout << glm::to_string(triangleTranslation) << endl;

    // Task: cube model matrix
 /*   mat4 cubeScaling = glm::scale(mat4(), vec3(0.1,0.8,0.1));
    mat4 cubeRotation1 = glm::rotate(mat4(),3.14f/4,vec3(0,0,1));
    mat4 cubeTranslation = glm::translate(mat4(),vec3(0.5,0,0));
	mat4 cubeRotation2 = glm::rotate(mat4(), 3.14f / 4, vec3(0, 0, -1));*/

    // Task: projection
    // Projection matrix: 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //mat4 projection = perspective(
    // Or, for an ortho camera: -x:x, -y:y, -z:z
    // mat4 Projection = ortho(

    // Task: view
    // Camera view matrix
    // mat4 view = lookAt(
	float i=0;
	int j = 0;
    do
    {
        // Task: depth test  | GL_DEPTH_BUFFER_BIT
        // Clear the screen (color and depth)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // draw triangle
        glBindVertexArray(triangleVAO);

        // Task: triangle MVP
        // compute model, view projection operator
		mat4 triangleScaling = glm::scale(mat4(), vec3(0.05, 1., 0.05));
		mat4 triangleRotation = glm::rotate(mat4(), i , vec3(0, 0, 1));
		mat4 triangleTranslation = glm::translate(mat4(), vec3(0, 0.2, 0));
		mat4 triangleModel =  triangleRotation * triangleTranslation * triangleScaling ;

		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &triangleModel[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		mat4 triangleScaling1 = glm::scale(mat4(), vec3(0.05, 0.5, 0.05));
		mat4 triangleRotation1 = glm::rotate(mat4(), i/12 , vec3(0, 0, 1));
		mat4 triangleTranslation1 = glm::translate(mat4(), vec3(0, 0.2, 0));
		triangleModel = triangleRotation1 * triangleScaling1 * triangleTranslation1;

		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &triangleModel[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (j % 5 == 0) {
			i = i - 3.14f / 60;
			if (j == 100)j = 0;
		}
		j++;
		
		
        
		// draw clock
        glBindVertexArray(cubeVAO);
		
		//ONE
		cubeInt(1,0);
		//TWO
		cubeInt(2, 0);
		cubeInt(2, 1);
		//THREE 
		cubeInt(3, -1);
		cubeInt(3, 0);
		cubeInt(3, 1);
		//FOUR
		cubeInt(4, -1);
		cubeVshape(4);
		//FIVE
		cubeVshape(5);
		//SIX
		cubeInt(6, 1);
		cubeVshape(6);
		//SEVEN
		cubeInt(7, 2);
		cubeInt(7, 1);
		cubeVshape(7);
		//EIGHT
		cubeInt(8, 3);
		cubeInt(8, 2);
		cubeInt(8, 1);
		cubeVshape(8);
		//NINE
		cubeInt(9, -1);
		cubeCross(9);
		//TEN
		cubeCross(10);
		//ELEVEN
		cubeInt(11, 1);
		cubeCross(11);
		//TWELVE
		cubeInt(12, 1);
		cubeInt(12, 2);
		cubeCross(12);

		
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

void cubeCross(float a) 
{
	a = 3.14f / 6 * (3 - a);
	mat4 cubeScaling = glm::scale(mat4(), vec3(0.01, 0.08, 0.01));
	mat4 cubeRotation11 = glm::rotate(mat4(), 3.14f / 6 - a, vec3(0, 0, 1));
	mat4 cubeRotation12 = glm::rotate(mat4(), a, vec3(0, 0, 1));
	mat4 cubeTranslation = glm::translate(mat4(), vec3(0.5, 0, 0));
	mat4 cubeRotation21 = glm::rotate(mat4(), -3.14f / 6 -a, vec3(0, 0, 1));
	mat4 cubeRotation22 = glm::rotate(mat4(), a, vec3(0, 0, 1));
	mat4 cubeModel = cubeRotation12 * cubeTranslation * cubeRotation11 * cubeScaling;
	// Task: cube MVP
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeModel[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


	cubeModel = cubeRotation22 * cubeTranslation * cubeRotation21 * cubeScaling;
	// Task: cube MVP
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeModel[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

void cubeVshape(float a)
{	
	a = 3.14f / 6 * (3 - a);
	mat4 cubeScaling = glm::scale(mat4(), vec3(0.01, 0.08, 0.01));
	mat4 cubeRotation11 = glm::rotate(mat4(), 3.14f / 10 - a, vec3(0, 0, 1));
	mat4 cubeRotation12 = glm::rotate(mat4(), a, vec3(0, 0, 1));
	mat4 cubeTranslation1 = glm::translate(mat4(), vec3(-0.024, 0, 0));
	mat4 cubeTranslation2 = glm::translate(mat4(), vec3(0.5, 0, 0));
	mat4 cubeTranslation3 = glm::translate(mat4(), vec3(0.024, 0, 0));
	mat4 cubeRotation21 = glm::rotate(mat4(), -3.14f / 10 - a, vec3(0, 0, 1));
	mat4 cubeRotation22 = glm::rotate(mat4(), a, vec3(0, 0, 1));
	mat4 cubeModel = cubeRotation12 * cubeTranslation2 *  cubeRotation11 * cubeTranslation1 * cubeScaling;
	// Task: cube MVP
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeModel[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


	cubeModel = cubeRotation22 * cubeTranslation2 * cubeRotation21 * cubeTranslation3  * cubeScaling;
	// Task: cube MVP
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeModel[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

void cubeInt(float a, int b) 
{
	a = 3.14f / 6 * (3 - a);
	mat4 cubeScaling = glm::scale(mat4(), vec3(0.01, 0.08, 0.01));
	mat4 cubeRotation1 = glm::rotate(mat4(), - a, vec3(0, 0, 1));
	mat4 cubeTranslation1 = glm::translate(mat4(), vec3(0.5, 0, 0));
	mat4 cubeRotation2 = glm::rotate(mat4(), a, vec3(0, 0, 1));
	mat4 cubeTranslation2 = glm::translate(mat4(), vec3(0.07 * b, 0, 0));
	mat4 cubeModel = cubeTranslation2 * cubeRotation2 * cubeTranslation1 * cubeRotation1 * cubeScaling;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeModel[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

void initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		throw runtime_error("Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);  // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

																   // Open a window and create its OpenGL context
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set background color (gray) [r, g, b, a]
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	// Log
	logGLParameters();
}

int main(void)
{
    try
    {
        initialize();
        createContext();
        mainLoop();
        free();
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}