#include "shaderClass.h"

// file reader function
std::string get_file_contents(const char* filename)
{
	// read in the passed parameter 'filename' using binary mode
	std::ifstream input(filename, std::ios::binary);
	if (input)
	{
		std::string fileContents;
		// set the position where the read operation will occur
			// 1st param: offset value
			// 2nd param: specified reference point from which the offset is measured
		// 0 characters offset from the end of the file
		input.seekg(0, std::ios::end);

		// resizes the string we'll use to the size (in bytes) of passed parameter
			// tellg() returns streampos object holding the current position of the file pointer in the input stream
		// make the string large enough to hold all of the file
		fileContents.resize(input.tellg());

		// places the pointer 0 bytes offset from the beginning of the file
		input.seekg(0, std::ios::beg);

		// have the input reader pass items in the read file into the fileContents string starting at the beginning of it to the end of it
		input.read(&fileContents[0], fileContents.size());

		// close file
		input.close();
		// return the file in a string
		return(fileContents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

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

	// sets value to objectID (created in header file!)
	objectID = glCreateProgram();

	// similar deal, attaching the vertex and fragment shaders to the shader program
	glAttachShader(objectID, vertexShader);
	glAttachShader(objectID, fragmentShader);

	// LINKING is different than attaching. Really combines the attached shaders to the program to be executed on the GPU.
	glLinkProgram(objectID);

	// deletes the shaders from taking up memory since the linking essentially "copies" all the data it needs to from them to put them in one program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// we will be using this shader program, passing openGL the ID for our object
void Shader::Activate()
{
	glUseProgram(objectID);
}

// delete our shader program by passing OpenGL the ID for our object
void Shader::Delete()
{
	glDeleteProgram(objectID);
}