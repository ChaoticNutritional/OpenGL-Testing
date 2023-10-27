#include<iostream>
#include<glad/glad.h>
#include<glfw/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// * NOTE: all OpenGL objects are accessed by References!!



class Point3D {
public:
	float x, y, z = 0.0f;
	Point3D(float cX, float cY, float z = 0.0f) { x = cX, y = cY; };
	Point3D() { x = 0, y = 0, z = 0; };
};

class Triangle{
public:
	Point3D p1, p2, p3;
	Triangle(Point3D p1, Point3D p2, Point3D p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}

	GLfloat* vertArray(){
		GLfloat verts[] =
		{
			p1.x, p1.y, p1.z,
			p2.x, p2.y, p2.z,
			p3.x, p3.y, p3.z
		};
		return verts;
	}
};

int main()
{
	// start
	glfwInit();

	// Specify OpenGL version to GLFW package
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Specify usage of CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// coordinates of vertices
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // LEFT : BOTTOM
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // RIGHT : BOTTOM
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // ORIGIN (mid) : UPPER
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // mid between bottom left and upper mid
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // mid between bottom right and upper mid
		0.0f, -0.5f * float(sqrt(3)) / 3 ,0.0f // mid between bottom left and bottom right
	};

	GLuint indexBuffer[] =
	{
		// index 0, 3, and 5 when drawn in that order creates lower left triangle etc.
		0, 3, 5, // Lower left triangle 
		3, 2, 4, // Upper triangle
		5, 4, 1 // Lower right triangle
	};

	// Make sure we can create the window at all
	// DETAILS FOR BELOW FUNCTION
		// create new GLFWwindow object sized at 800 x 800 pixels with name: as 3rd param
		// 4th param: specifies which screen will host the window in full screen mode
			// NULL for none, but can be specified with a "glfwGetMonitors" query (which returns an array of monitors)
		// 5th param: specifies another window object that will share resources to THIS one
	GLFWwindow* window = glfwCreateWindow(800, 800, "openGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// DETAILS FOR BELOW FUNCTION
		// a rendering context is a state where all the OpenGL commands are applied
		// To draw graphics, you need to have a current rendering context
		// glfwMakeContextCurrent is called to specify which window's rendering context you want to make current
		// telling GLFW that you want to work with OpenGL in the context of a specific window.
		// All subsequent OpenGL operations will affect this window until you set a different context with glfwMakeContextCurrent.
		//  if you have multiple windows, you'll call glfwMakeContextCurrent for each window before rendering to it
	glfwMakeContextCurrent(window);


	// Load GLAD so it configures OpenGL
	// DETAILS FOR BELOW FUNCTION

		/*
		// In modern OpenGL, you don't directly call OpenGL functions like glDrawArrays or glGenBuffers.
		// Instead, you retrieve function pointers to these OpenGL functions from the OpenGL driver at runtime
		// calling this queries the graphics driver for the addresses of ALL OpenGL functions in your specified version defined above
		*/
	gladLoadGL();

	// DETAILS FOR BELOW FUNCTION

	// specify the viewport of OpenGL in the Window
	// this is different than just the size of the window or at least it CAN be.
	// this is so that you can have a smaller window but make use of OpenGL in just a part of it
	// useful in certain scenario's like: split screen gaming, or rendering only a portion of the window for post-processing effects
	glViewport(0, 0, 800, 800);

	// initialize a shader program using the default vertex and fragment shaders we've created
	Shader shaderProgram("default.vert", "default.frag");

	// Creates containers to store the Vertex Array Object and Vertex Buffer Object
	//VAO: a blueprint for rendering vertex data
	//VBO: stores the ACTUAL vertex data
	//EBO: stores int values corresponding to indices of the VBO array, specifying the order in which they should be processed
	VAO VAO1;
	VAO1.Bind();

	// Creates 
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indexBuffer, sizeof(indexBuffer));

	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	while (!glfwWindowShouldClose(window))
	{
		// RGBA of the color buffer
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// clears the color buffer of the FRAME buffer (sets all pixels in buffer to the CLEAR color we've set above
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Tell OpenGL which shader program we'll be using
		shaderProgram.Activate();
		
		// loads our VAO which contains the organized version of our VBO
		VAO1.Bind();

		// Renders our vertices
			// 1st param: specifies primitive to draw between vertices from the array
			// 2nd param: specifies the starting index within the bound Vertex Array.
			// 3rd param: specifies the number of vertices to render in total this way (in our case, the length of our vertices array)
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//Changing for glDrawElements
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


		// swaps the buffers
		glfwSwapBuffers(window);

		glfwPollEvents(); 
	}

	// cleanup!
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// end logic
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}