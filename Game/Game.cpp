#include "Game.h"

Game::Game() {}

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == 0x8251) return;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "* GL ERROR *" : ""),
		type, severity, message);
}

std::queue<int> Game::keyEvents;
bool Game::updateScreenSize = false;
int Game::scrWidth = 800;
int Game::scrHeight = 600;

// Unfortunately these can't be instance methods
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game::keyEvents.push(key);
	Game::keyEvents.push(scancode);
	Game::keyEvents.push(action);
	Game::keyEvents.push(mods);
}

void windowResizeCallback(GLFWwindow* window, int scrWidth, int scrHeight)
{
	Game::updateScreenSize = true;
	Game::scrWidth = scrWidth;
	Game::scrHeight = scrHeight;
}

int Game::init() {

	// GLFW & OpenGL setup
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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
	physEngine = std::make_unique<Physics>();
	physEngine->objects.push_back(*e);
	physEngine->objects.push_back(*f);

	Shader& default_shader = resourceManager::loadShader("default", "shaders/default.vert", "shaders/default.frag", "shaders/default.geom");

	Shader& debugGridShader = resourceManager::loadShader(
		"debugGrid",
		"shaders/debug_grid.vert",
		"shaders/debug_grid.frag",
		"shaders/debug_grid.geom",
		"shaders/debug_grid.tesc",
		"shaders/debug_grid.tese");


	p = std::make_unique<Player>();
	p->windowResizeCallback(800, 600);

	Font& font = resourceManager::loadFont("celestiaRedux", "fonts/CelestiaRedux.ttf", 72, 800, 600);
	Shader& textShader = resourceManager::loadShader("text", "shaders/text.vert", "shaders/text.frag");

	terrain = std::make_unique<Terrain>();
	terrain->Generate(-64, -64, 128, 128, 10, 10,
		[](float a, float b) { return 0;  (a * a - b * b) / 512; });

	Shader& buttonShader = resourceManager::loadShader("button", "shaders/button.vert", "shaders/button.frag");


	startScene = std::make_unique<Scene>("Start Menu");
	
	for (int i = 0; i < 8; i++) {
		std::shared_ptr<Button> startButton = std::make_shared<Button>("Button " + std::to_string(i),
		    i*100, 0, i*100+100, 100,
			"resources/debug_buttonDefault.png",
			"resources/debug_buttonHover.png",
			"resources/debug_buttonPress.png");

		std::function<void()> func = [this]() {
			this->gameState = GameState::IN_GAME;
			glfwSetCursorPos(this->window, (this->scrWidth / 2), (this->scrHeight / 2));
			};
		//startButton->setPressCallBack(func);

		startScene->addObject(move(startButton));
	}

	Shader& skyShader = resourceManager::loadShader("skydome", "shaders/skydome.vert", "shaders/skydome.frag", "shaders/skydome.geom");
	
	GLfloat stars[300];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis1(0, glm::pi<float>());
	std::uniform_real_distribution<> dis2(0, 2 * glm::pi<float>());
	for (int i = 0; i < 100; i++) {
		float theta = dis1(gen);
		float psi = dis2(gen);
		float X = sin(theta) * cos(psi);
		float Y = sin(theta) * sin(psi);
		float Z = cos(theta);
		stars[i * 3] = X;
		stars[i * 3 + 1] = Y;
		stars[i * 3 + 2] = Z;
	}
	skyShader.Activate();
	glUniform3fv(glGetUniformLocation(skyShader.ID, "stars"), 300, stars);

	tempSky = std::make_unique<Sky>();

	tempSky->Generate();

	Shader& cloudShader = resourceManager::loadShader("clouds", "shaders/sphere.vert", "shaders/sphere.frag", "shaders/sphere.geom");
	tempClouds = std::make_unique<Clouds>();
	tempClouds->Generate();

	Shader& debugVecShader = resourceManager::loadShader("debugVec", "shaders/debug_vector.vert", "shaders/debug_vector.frag");

	return 0;
}

void Game::startMenu_draw() {
	startScene->draw();
}

void Game::startMenu_tick(double frameTime) {
	double xpos, ypos; glfwGetCursorPos(window, &xpos, &ypos);
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	startScene->mouseEvent(xpos, ypos, state==GLFW_PRESS?1:0);

	while (!keyEvents.empty()) {
		keyEvents.pop();
	}
	if (updateScreenSize) {
		glViewport(0, 0, scrWidth, scrHeight);
		p->windowResizeCallback(scrWidth, scrHeight);
		resourceManager::updateScreenSize(scrWidth, scrHeight);
		updateScreenSize = false;
	}
}

void Game::inGame_draw() {
	Shader& default_shader = resourceManager::getShader("default");
	Shader& textShader = resourceManager::getShader("text");
	Shader& debugGridShader = resourceManager::getShader("debugGrid");
	Font& font = resourceManager::getFont("celestiaRedux");
	Shader& skyShader = resourceManager::getShader("skydome");
	Shader& cloudShader = resourceManager::getShader("clouds");
	Shader& debugVecShader = resourceManager::getShader("debugVec");

	glm::mat4 proj = p->getProjMatrix();
	glm::mat4 view = p->getViewMatrix();
	p->Draw(default_shader);

	default_shader.Activate();
	if (checkOBBCollision(physEngine->objects[0], physEngine->objects[1])) {
		glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)1);
		//glm::vec3 lsa = getLeastSeparatingAxis(e, f);
	}
	else {
		glUniform1i(glGetUniformLocation(default_shader.ID, "debug_tmpvar"), (GLuint)0);
	}

	//physEngine->Draw(default_shader, proj, view);

	// Render the grid
	terrain->Draw(debugGridShader, proj, view, p->camPos);

	// Render the sky
	tempSky->Tick();
	tempSky->Draw(skyShader, proj, view);

	//tempClouds->Draw(cloudShader, proj, view);
	
	// Render text last
	p->debugText(font, textShader);
	p->debugVectors(debugVecShader);

}

void Game::inGame_tick(double frameTime) {
	p->Tick(window, frameTime);
	physEngine->Tick(frameTime);

	if (!keyEvents.empty()) {
		int key = keyEvents.front(); keyEvents.pop();
		int scancode = keyEvents.front(); keyEvents.pop();
		int action = keyEvents.front(); keyEvents.pop();
		int mods = keyEvents.front(); keyEvents.pop();
		p->keyCallback(window, key, scancode, action, mods);
	}

	if (updateScreenSize) {
		glViewport(0, 0, scrWidth, scrHeight);
		p->windowResizeCallback(scrWidth, scrHeight);
		resourceManager::updateScreenSize(scrWidth, scrHeight);
		updateScreenSize = false;
	}
}

int Game::run() {

	double curTime = glfwGetTime(), frameTime, lastFPSUpdate = glfwGetTime();
	int framesPerSecond = 0, frameCounter = 0;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.18f, 0.02f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Frames counter
		frameTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();
		frameCounter++;
		if (lastFPSUpdate + 1 < curTime) {
			framesPerSecond = frameCounter;
			frameCounter = 0;
			lastFPSUpdate = curTime;
		}

		Font& font = resourceManager::getFont("celestiaRedux");
		font.renderLine("FPS:" + std::to_string(framesPerSecond), DisplayPos{Alignment::TOP_RIGHT, 2, 2}, 20, glm::vec3(1.0f, 0.0f, 1.0f));

		switch (gameState) {
		case (GameState::START_MENU): 
			startMenu_tick(frameTime);
			startMenu_draw();
			break;
		case (GameState::PAUSE_MENU):
			break; 
		case (GameState::SETTINGS):
			break; 
		case (GameState::IN_GAME):
			inGame_tick(frameTime);
			inGame_draw();
			break;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	return 0;
}

int Game::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}