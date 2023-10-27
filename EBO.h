#ifndef EBO_CLASS_H
#define EBO_CLASS_H

/*
	I thought about creating a base class of Buffer Object or Element Object that VAO, EBO and BHO could extend from, however I think as I progress I wonder what making these functions virtual would have on performance as we begin creating multiple VBOs and EBOs? 
*/

#include<glad/glad.h>

class EBO
{
public:
	GLuint objectID;
	// constructor taking in a vertices array and the size in bytes of that array
	EBO(GLuint* indexArray, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !EBO_CLASS_H

