#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Phys_Object.h"
#include"shaderClass.h"
#include"debugCamera.h"

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "* GL ERROR *" : ""),
		type, severity, message);
}

debugCamera cam(800, 600, glm::vec3(0.0f, 0.0f, -3.0f));

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cam.keyCallback(window, key, scancode, action, mods);
}

void windowResizeCallback(GLFWwindow* window, int scrWidth, int scrHeight)
{
	cam.windowResizeCallback(scrWidth, scrHeight);
}

int main(int argc, char* argv[]) {

	// Init stuff
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW windows" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 600);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glfwSetKeyCallback(window, keyCallback);

	glfwSwapInterval(1);

	OBB e, f;
	f.position = glm::vec3(0.0f, 0.0f, 0.0f);
	e.position = glm::vec3(0.0f, 5.0f, 0.0f);
	//f.rotVelocity = glm::quat();
	//e.rotVelocity = glm::quat();

	Shader default_shader;
	default_shader.Compile("shaders/default.vert", "shaders/default.frag", "shaders/default.geom");

	Shader debugGridShader;
	debugGridShader.Compile(
		"shaders/debug_grid.vert", 
		"shaders/debug_grid.frag", 
		"shaders/debug_grid.geom", 
		"shaders/debug_grid.tesc", 
		"shaders/debug_grid.tese");

	glEnable(GL_DEPTH_TEST);
	double curTime = glfwGetTime(), frameTime;

	GLuint terrainVAO, terrainVBO;
	std::vector<float> terrainVertices = {
		-32.0f, 0.0f, -32.0f, 0.0f, 0.0f,
		 32.0f, 0.0f, -32.0f, 0.0f, 0.0f,
		-32.0f, 0.0f,  32.0f, 0.0f, 0.0f,
		 32.0f, 0.0f,  32.0f, 0.0f, 0.0f,
	};

	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER,
		terrainVertices.size() * sizeof(float),       // size of vertices buffer
		&terrainVertices[0],                          // pointer to first element
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.18f, 0.02f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		frameTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();

		// Only basic graphics until most of the physics/game engine is done
		// TODO: Camera doesn't like (remote access) inputs
		cam.Inputs(window, frameTime);
		cam.updateMatrix(90.0f, 0.1f, 10000.0f);

		default_shader.Activate();
		if (checkOBBCollision(e, f)) {
			glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)1);
			glm::vec3 lsa = getLeastSeparatingAxis(e, f);
		}
		else {
			glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)0);
		}

		e.Draw(default_shader, cam, frameTime);
		f.Draw(default_shader, cam, frameTime);

		// Render the grid
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		debugGridShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(debugGridShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(cam.projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(debugGridShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(cam.viewMatrix));
		glUniform3f(glGetUniformLocation(debugGridShader.ID, "camPos"), cam.Position.x, cam.Position.y, cam.Position.z);

		glBindVertexArray(terrainVAO);
		glDrawArrays(GL_PATCHES, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}