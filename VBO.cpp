#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &objectID);
	glBindBuffer(GL_ARRAY_BUFFER, objectID);

	// actually PUTS the vertex data into the current buffer (which we specified above is also the VBO
	// 4th parameter specifies how to use the data from the buffer
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

}

void VBO::Bind()
{

	// tells OpenGL that operations will apply to VBO, and it should be considered as the current GL_ARRAY_BUFFER
	// preparation to send vertex data to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, objectID);
}

void VBO::Unbind()
{
	// unbinds the GL_ARRAY_BUFFER so that in case we do any calls later to it, so that we don't mistakenly change our VBO data, which was bound to it before
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &objectID);
}