#include "VAO.h"

VAO::VAO()
{
	// creating placeholders in GPU memory to store the configuration for and actual vertex data, respectively
	glGenVertexArrays(1, &objectID);
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
	// call the bind function of the passed VBO first
	VBO.Bind();
	// Configure the Vertex Attribute so that OpenGL knows how to Read the VBO
		// 1st param: Index of the vertex attribute
		// 2nd param: specifies how many components per vertex attribute
		// 3rd param: Specifies that each component is a GL float type
		// 4th param: Specifies the "stride". Because each vertex in our array has 3 components of type FLOAT, the stride is the total number of bytes that 3 float type variables would occupy
			// Stride: in memory, how much memory in bytes does our attribute take up, and when should we explect to find the "next" one
		// 5th param: pointer to the first component of the first attribute. The offset in bytes from the beginning of each vertex in the buffer
			// (void*)0
	// this also essentially links our VBO's data INTO our VAO, and HOW OpenGL should do that linking
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// specifies that we're looking at vertex attribute in idex 0, which is typically VERTEX POSITION
	glEnableVertexAttribArray(layout); //enables this attribute so OpenGL knows to use it
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(objectID);//This means that any subsequent OpenGL calls that deal with vertex data will use the currently bound VAO as their context.
}

void VAO::Unbind()
{
	// unbinds the VAO
		// ensure this next one is done AFTER unbinding VAO
	glBindVertexArray(0); 
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &objectID);
}