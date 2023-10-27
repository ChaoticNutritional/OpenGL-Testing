#include<iostream>
#include<glad/glad.h>
#include<glfw/glfw3.h>

// * NOTE: all OpenGL objects are accessed by References!!

// Vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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


	// OpenGL variable version type of a uint ("positive" integer). Sort of like a code number to refer to that shader now
		// Creates a GLuint variable to reference the vertex shader CREATED BY the glCreateShader function
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// GL shader source specifies the shader source to our shader object (GLuint one)
		// 1st param is the shader object created (GLuint)
		// 3rd param is a reference to the definition we made at the top of the program

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// must be compiled NOW into machine code so that it can be used by the GPU
	glCompileShader(vertexShader);

		// as above
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// as above
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// as above
	glCompileShader(fragmentShader);

	// creates a shader program
	GLuint shaderProgram = glCreateProgram();
	// similar deal, attaching the vertex and fragment shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	// LINKING is different than attaching. Really combines the attached shaders to the program to be executed on the GPU.
	glLinkProgram(shaderProgram);

	// deletes the shaders from taking up memory since the linking essentially "copies" all the data it needs to from them to put them in one program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Creates containers to store the Vertex Array Object and Vertex Buffer Object
	GLuint VAOs[1], VBOs[1], EBO; // vertex buffer object: stores vertex data
	//VAO: a blueprint for rendering vertex data
	//VBO: stores the ACTUAL vertex data

	// creating placeholders in GPU memory to store the configuration for and actual vertex data, respectively
	glGenVertexArrays(1, VAOs); // make sure do gen this before the vBo
	glGenBuffers(1, VBOs);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAOs[0]); //This means that any subsequent OpenGL calls that deal with vertex data will use the currently bound VAO as their context.

	// tells OpenGL that operations will apply to VBO, and it should be considered as the current GL_ARRAY_BUFFER
	// preparation to send vertex data to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	// actually PUTS the vertex data into the current buffer (which we specified above is also the VBO
	// 4th parameter specifies how to use the data from the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to Read the VBO
		// 1st param: Index of the vertex attribute
		// 2nd param: specifies how many components per vertex attribute
		// 3rd param: Specifies that each component is a GL float type
		// 4th param: Specifies the "stride". Because each vertex in our array has 3 components of type FLOAT, the stride is the total number of bytes that 3 float type variables would occupy
			// Stride: in memory, how much memory in bytes does our attribute take up, and when should we explect to find the "next" one
		// 5th param: pointer to the first component of the first attribute. The offset in bytes from the beginning of each vertex in the buffer
			// (void*)0
	// this also essentially links our VBO's data INTO our VAO, and HOW OpenGL should do that linking
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// specifies that we're looking at vertex attribute in idex 0, which is typically VERTEX POSITION
	glEnableVertexAttribArray(0); // 

	// unbinds the GL_ARRAY_BUFFER so that in case we do any calls later to it, so that we don't mistakenly change our VBO data, which was bound to it before
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinds the VBO
	glBindVertexArray(0); // unbinds the VAO
	// ensure this next one is done AFTER unbinding VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbinds the EBO
	
	// binding: making a certain object the CURRENT object. So whenever we use a function that would modify this TYPE of object, it modifies the current one

	while (!glfwWindowShouldClose(window))
	{
		// RGBA of the color buffer
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// clears the color buffer of the FRAME buffer (sets all pixels in buffer to the CLEAR color we've set above
		glClear(GL_COLOR_BUFFER_BIT);
		// specifies our shader program will be used
		glUseProgram(shaderProgram);
		// loads our VAO which contains the organized version of our VBO
		glBindVertexArray(VAOs[0]);

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
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	// end logic
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}