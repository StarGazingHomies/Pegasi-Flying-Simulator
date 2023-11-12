#include "Game.h"

Game::Game() {}

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {
	if (type == 0x8251) return;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "* GL ERROR *" : ""),
		type, severity, message);
}

std::queue<KeyEvent> Game::keyEvents;
std::queue<unsigned int> Game::textEvents;
std::queue<MouseEvent> Game::mouseEvents;
bool Game::updateScreenSize = false;
int Game::scrWidth = 800;
int Game::scrHeight = 600;
double Game::lastXPos = -1;
double Game::lastYPos = -1;

// Unfortunately these can't be instance methods
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game::keyEvents.push(KeyEvent{ key, scancode, action, mods });
}

void windowResizeCallback(GLFWwindow* window, int scrWidth, int scrHeight) {
	Game::updateScreenSize = true;
	Game::scrWidth = scrWidth;
	Game::scrHeight = scrHeight;
}

void charCallback(GLFWwindow* window, unsigned int codepoint) {
	Game::textEvents.push(codepoint);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	Game::mouseEvents.push(MouseEvent{ xpos, ypos });
	Game::lastXPos = xpos;
	Game::lastYPos = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Game::mouseEvents.push(MouseEvent{ Game::lastXPos, Game::lastYPos, button, action, mods });
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
	glfwSetCharCallback(window, charCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	Shader& defaultColorShader = resourceManager::loadShader("defaultColor", "shaders/default_color.vert", "shaders/default_color.frag");

	p = std::make_unique<Player>();
	p->windowResizeCallback(800, 600);

	Font& font = resourceManager::loadFont("celestiaRedux", "fonts/CelestiaRedux.ttf", 72, 800, 600);
	Shader& textShader = resourceManager::loadShader("text", "shaders/text.vert", "shaders/text.frag");

	terrain = std::make_unique<Terrain>();
	terrain->Generate(-64, -64, 128, 128, 10, 10,
		[](float a, float b) { return 0;  (a * a - b * b) / 512; });

	Shader& buttonShader = resourceManager::loadShader("button", "shaders/2D/button.vert", "shaders/2D/button.frag");
	Shader& colorShader = resourceManager::loadShader("color", "shaders/2D/color.vert", "shaders/2D/color.frag");

	startScene = std::make_unique<Scene>("Start Menu");

	std::shared_ptr<Button> startButton = std::make_shared<Button>("Start Button",
		0, 0, 100, 100,
		"resources/debug_buttonDefault.png",
		"resources/debug_buttonHover.png",
		"resources/debug_buttonPress.png");

	std::function<void()> func = [this]() {
		this->gameState = GameState::IN_GAME;
		glfwSetCursorPos(this->window, (this->scrWidth / 2), (this->scrHeight / 2));
		};
	startButton->setPressCallBack(func);

	startScene->addObject(startButton);

	//std::shared_ptr<StaticText> text = std::make_shared<StaticText>("Hello World", 0, 0, 60, glm::vec3(1.0f, 1.0f, 1.0f));
	//startScene->addObject(text);

	std::shared_ptr<TextBox> textBox = std::make_shared<TextBox>(
		"TextBox", 
		"Enter text here", 
		0, 200, 200, 230);
	startScene->addObject(textBox);

	Shader& skyShader = resourceManager::loadShader("skydome", "shaders/skydome.vert", "shaders/skydome.frag", "shaders/skydome.geom");


	resourceManager::loadShader("clouds", "shaders/sphere.vert", "shaders/sphere.frag", "shaders/sphere.geom");
	resourceManager::loadShader("debugVec", "shaders/debug_vector.vert", "shaders/debug_vector.frag");

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

	//tempClouds = std::make_unique<Clouds>();
	//tempClouds->Generate();


	//debugSurfaceNet = std::make_unique<SurfaceNet>(glm::vec3(0.0f), glm::vec3(10.0f), 40, 40, 40);

	terrain2 = std::make_unique<Terrain2>();
	int size = 0;
	for (int x = size; x <= size; x++) {
		for (int y = -1; y <= -1; y++) {
			for (int z = size; z <= size; z++) {
				printf("Generating chunk %d %d %d\n", x, y, z);
				terrain2->generateChunk(x, y, z);
				std::shared_ptr<Chunk> c = terrain2->getChunk(x, y, z);
				printf("Chunk has %d vertices and %d quads.\n", c->surfaceNet.vertexCount, c->surfaceNet.quadCount);
			}
		}
	}

	// Init "last" xPos and yPos
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	lastXPos = xpos;
	lastYPos = ypos;

	return 0;
}

void Game::startMenu_draw() {
	startScene->draw();
}

void Game::startMenu_tick(double frameTime) {
	while (!mouseEvents.empty()) {
		MouseEvent m = mouseEvents.front(); mouseEvents.pop();
		startScene->mouseEvent(m);
	}

	while (!keyEvents.empty()) {
		KeyEvent k = keyEvents.front(); keyEvents.pop();
		startScene->keyboardEvent(k);
	}
	while (!textEvents.empty()) {
		unsigned int c = textEvents.front(); textEvents.pop();
		startScene->textEvent(c);
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
	terrain2->draw(proj, view);

	// Render the sky
	tempSky->Tick();
	tempSky->Draw(skyShader, proj, view);

	//debugSurfaceNet->draw(proj, view);

	//tempClouds->Draw(cloudShader, proj, view);
	
	// Render text last
	p->debugText(font, textShader);
	p->debugVectors(debugVecShader);

}

void Game::inGame_tick(double frameTime) {
	p->Tick(window, frameTime);
	physEngine->Tick(frameTime);

	if (!keyEvents.empty()) {
		KeyEvent event = keyEvents.front(); keyEvents.pop();
		p->keyCallback(window, event.key, event.scancode, event.action, event.mods);
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

		// Draw overlayed stuff last
		Font& font = resourceManager::getFont("celestiaRedux");
		font.renderLine("FPS:" + std::to_string(framesPerSecond), DisplayPos{ Alignment::TOP_LEFT, 2, 2 }, 20, glm::vec3(1.0f, 0.0f, 1.0f));
		font.renderAll(resourceManager::getShader("text"));

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