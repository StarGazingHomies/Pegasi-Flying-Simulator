#include <iostream>
#include <queue>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Phys_Object.h"
#include"Shader.h"
#include"Phys.h"
#include"Player.h"
#include"resourceManager.h"
#include"Terrain.h"

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

std::queue<int> keyEvents;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyEvents.push(key);
	keyEvents.push(scancode);
	keyEvents.push(action);
	keyEvents.push(mods);
}

int newWidth = -1, newHeight = -1;
void windowResizeCallback(GLFWwindow* window, int scrWidth, int scrHeight)
{
	newWidth = scrWidth;
	newHeight = scrHeight;
	//cam.windowResizeCallback(scrWidth, scrHeight);
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
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glfwSwapInterval(1);

	std::unique_ptr<OBB> e = std::make_unique<OBB>();
	std::unique_ptr<OBB> f = std::make_unique<OBB>();
	e->position = glm::vec3(0.0f, 5.0f, 0.0f);
	f->position = glm::vec3(0.0f, 0.0f, 0.0f);
	std::unique_ptr<Physics> physEngine = std::make_unique<Physics>();
	physEngine->objects.push_back(*e);
	physEngine->objects.push_back(*f);
	//f.rotVelocity = glm::quat();
	//e.rotVelocity = glm::quat();

	Shader default_shader = resourceManager::loadShader("default","shaders/default.vert", "shaders/default.frag", "shaders/default.geom");

	Shader debugGridShader = resourceManager::loadShader(
		"debugGrid",
		"shaders/debug_grid.vert",
		"shaders/debug_grid.frag",
		"shaders/debug_grid.geom",
		"shaders/debug_grid.tesc",
		"shaders/debug_grid.tese");


	double curTime = glfwGetTime(), frameTime;

	Terrain terrain;
	terrain.Generate(-32, -32, 64, 64, 1, 1);

	std::unique_ptr <Player> p = std::make_unique<Player>();
	p->windowResizeCallback(800, 600);

	Font font = resourceManager::loadFont("celestiaRedux", "fonts/CelestiaRedux.ttf",72,800,600);
	Shader textShader = resourceManager::loadShader("text", "shaders/text.vert", "shaders/text.frag");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.18f, 0.02f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		frameTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();

		p->Tick(window, frameTime);
		glm::mat4 proj = p->getProjMatrix();
		glm::mat4 view = p->getViewMatrix();
		p->Draw(default_shader);
		p->debugText(font, textShader);

		default_shader.Activate();
		if (checkOBBCollision(*e, *f)) {
			glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)1);
			//glm::vec3 lsa = getLeastSeparatingAxis(e, f);
		}
		else {
			glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)0);
		}

		//physEngine->Draw(default_shader, proj, view);
		physEngine->Tick(frameTime);

		// Render the grid
		terrain.Draw(debugGridShader, proj, view, p->camPos);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (!keyEvents.empty()) {
			int key = keyEvents.front(); keyEvents.pop();
			int scancode = keyEvents.front(); keyEvents.pop();
			int action = keyEvents.front(); keyEvents.pop();
			int mods = keyEvents.front(); keyEvents.pop();
			p->keyCallback(window, key, scancode, action, mods);
		}

		if (newHeight != -1) {
			glViewport(0, 0, newWidth, newHeight);
			p->windowResizeCallback(newWidth, newHeight);
			font.updateScreenSize(newWidth, newHeight);
			//std::cout << newWidth << ", " << newHeight << "\n";
			newHeight = -1;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}