#include"Shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader()
{
}

void Shader::Compile(const char* vertexFile, const char* fragmentFile, const char* geometryFile, const char* tessControlFile, const char* tessEvaluationFile)
{
	// Shader program
	ID = glCreateProgram();

	GLuint vertexShader, fragmentShader, geometryShader, tessControlShader, tessEvaluationShader;

	// Vertex shader
	std::string vertexCode = get_file_contents(vertexFile);
	const char* vertexSource = vertexCode.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attatch vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the shader into machine code
	glCompileShader(vertexShader);
	// Check if vertex shader compiled correctly
	compileErrors(vertexShader, "VERTEX");
	// Attach shader to program
	glAttachShader(ID, vertexShader);

	// Tesselation control shader
	if (tessControlFile != nullptr)
	{
		std::string tessControlCode = get_file_contents(tessControlFile);
		const char* tessControlSource = tessControlCode.c_str();
		tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		// Attatch tess control shader source to tess control shader object
		glShaderSource(tessControlShader, 1, &tessControlSource, NULL);
		// Compile the tess control shader into machine code
		glCompileShader(tessControlShader);
		// Attach shader to program
		glAttachShader(ID, tessControlShader);
	}

	// Tesselation evaluation shader
	if (tessEvaluationFile != nullptr)
	{
		std::string tessEvaluationCode = get_file_contents(tessEvaluationFile);
		const char* tessEvaluationSource = tessEvaluationCode.c_str();

		tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		// Attatch tess evaluation shader source to tess evaluation shader object
		glShaderSource(tessEvaluationShader, 1, &tessEvaluationSource, NULL);
		// Compile the tess evaluation shader into machine code
		glCompileShader(tessEvaluationShader);
		// Attach shader to program
		glAttachShader(ID, tessEvaluationShader);
	}

	// Geometry shader
	if (geometryFile != nullptr)
	{
		std::string geometryCode = get_file_contents(geometryFile);
		const char* geometrySource = geometryCode.c_str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		// Attatch geometry shader source to fragment shader object
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		// Compile the geometry shader into machine code
		glCompileShader(geometryShader);
		// Check if geometry shader compiled correctly
		compileErrors(geometryShader, "GEOMETRY");
		// Attach shader to program
		glAttachShader(ID, geometryShader);
	}

	// Fragment shader
	std::string fragmentCode = get_file_contents(fragmentFile);
	const char* fragmentSource = fragmentCode.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attatch fragment shader source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the fragment shader into machine code
	glCompileShader(fragmentShader);
	// Check if fragment shader compiled correctly
	compileErrors(fragmentShader, "FRAGMENT");
	// Attach shader to program
	glAttachShader(ID, fragmentShader);

	// Link all the shaders together into a program
	glLinkProgram(ID);
	// Check if shaders linked correctly
	compileErrors(ID, "PROGRAM");

	// Free up the shaders objects once they are linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryFile != nullptr)
		glDeleteShader(geometryShader);
	if (tessEvaluationFile != nullptr)
		glDeleteShader(tessEvaluationShader);
	if (tessControlFile != nullptr)
		glDeleteShader(tessControlShader);
}

void Shader::Activate()
{
	// Activate shaders
	glUseProgram(ID);
}

void Shader::Delete()
{
	// Delete Shader program
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}