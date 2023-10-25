#include<iostream>
#include<glad/glad.h>
#include<glfw/glfw3.h>

int main()
{
	// start
	glfwInit();

	// Specify OpenGL version to GLFW package
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Specify usage of CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	// a rendering context is a state where all the OpenGL commands are applied
	// To draw graphics, you need to have a current rendering context
	// glfwMakeContextCurrent is called to specify which window's rendering context you want to make current
	// telling GLFW that you want to work with OpenGL in the context of a specific window.
	// All subsequent OpenGL operations will affect this window until you set a different context with glfwMakeContextCurrent.
	//  if you have multiple windows, you'll call glfwMakeContextCurrent for each window before rendering to it
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL

	/*
	// In modern OpenGL, you don't directly call OpenGL functions like glDrawArrays or glGenBuffers. 
	// Instead, you retrieve function pointers to these OpenGL functions from the OpenGL driver at runtime
	// calling this queries the graphics driver for the addresses of ALL OpenGL functions in your specified version defined above
	*/
	gladLoadGL();

	// specify the viewport of OpenGL in the Window
	// this is different than just the size of the window or at least it CAN be.
	// this is so that you can have a smaller window but make use of OpenGL in just a part of it
	// useful in certain scenario's like: split screen gaming, or rendering only a portion of the window for post-processing effects
	glViewport(0, 0, 800, 800);

	glClearColor(0.07f, 0.13f, 0.17f, 5.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// end logic
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}