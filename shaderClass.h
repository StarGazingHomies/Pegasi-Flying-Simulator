#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of shaders
	GLuint ID = 0;
	// Default constructor
	Shader();
	// compiles and links shader files
	void Compile(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr, const char* tessControlFile = nullptr, const char* tessEvaluationFile = nullptr);

	// Activates the shader
	void Activate();
	// Deactivates the shader
	void Delete();

private:
	// Check if shaders have compiled correctly
	void compileErrors(unsigned int shader, const char* type);
};

#endif