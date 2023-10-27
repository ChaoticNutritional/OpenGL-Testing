#include "EBO.h"

EBO::EBO(GLuint* indexBuffer, GLsizeiptr size)
{
	glGenBuffers(1, &objectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectID); //linkage

	// takes in the info from the element buffer array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indexBuffer, GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &objectID);
}