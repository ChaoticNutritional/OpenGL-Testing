#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h" // also included as we'll need to link a VBO to a VAO

class VAO
{
public:
	GLuint objectID;
	VAO();

	void LinkVBO(VBO VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};

#endif