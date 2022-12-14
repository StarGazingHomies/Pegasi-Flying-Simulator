#include"Player.h"

void debugVec3(std::string name, glm::vec3 vec) {
	printf("%s: <%.2f, %.2f, %.2f>", name.c_str(), vec.x, vec.y, vec.z);
}

Player::Player() {
	// Initialize location
	position = glm::vec3(0.0f, 2.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	rot = glm::quat();
	rotVelocity = glm::quat();
	camPos = glm::vec3(0.0f, 2.0f, -3.0f);
	camOrientation = glm::vec3(0.0f, 0.0f, 1.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Window width/height
	width = 800;
	height = 600;

	// View and proj matricies
	updateMatrix();

	// Some other stuff, not currently used
	leftWingMotion = WingMotion::IDLE;
	rightWingMotion = WingMotion::IDLE;
	leftWingPosition = 0.0f;
	rightWingPosition = 0.0f;
	leftWingVelocity = 0.0f;
	rightWingVelocity = 0.0f;



	// Initialize debug graphics

}

void Player::windowResizeCallback(int width, int height) {
	this->width = width;
	this->height = height;
}

double Player::tempFromAltitude(double altitude) {
	if (altitude <= 0) {
		// TODO: vary this temp based on current pos and other conditions of the surrounding area
		return 15.04;
	}
	else if (altitude < 11000) {
		// Troposphere
		return 15.04 - 0.00649 * altitude;
	}
	else if (altitude < 25000) {
		// Tropopause
		return -56.46;
	}
	else if (altitude < 44000) {
		// Stratosphere
		return -131.21 + 0.00299 * altitude;
	}
	// Mostly estimates from this point on
	else if (altitude < 52000) {
		// Stratopause
		return 0.35;
	}
	else if (altitude < 80000) {
		// Mesosphere
		return 162.59 - 0.00312 * altitude;
	}
	else if (altitude < 85000) {
		// Mesopause
		return -87.01;
	}
	else if (altitude < 600000) {
		// TODO: Thermosphere, which idk what to do about because of wild fluctuations :twimerp:
		return 0;
	}
	else {
		// Exosphere / deep space
		return -273.15;
	}
}

double Player::pressureFromAltitude(double altitude) {
	// TODO: Just can't be bothered right now 22/11/19
	return 100000;
}

double Player::RHSignedAngle(glm::vec3 a, glm::vec3 b, glm::vec3 n) {
	n = glm::normalize(n);
	return atan2(glm::dot(glm::cross(a, b), n), glm::dot(a, b));
}

double Player::LHSignedAngle(glm::vec3 a, glm::vec3 b, glm::vec3 n) {
	n = glm::normalize(n);
	return atan2(glm::dot(glm::cross(b, a), n), glm::dot(a, b));
}

glm::vec3 Player::wingLiftForce() {



	return glm::vec3(0.0f);
}

glm::vec3 Player::wingStrokeForce() {
	glm::vec3 leftForce, rightForce;
	glm::mat4 rotMatrix = glm::mat4_cast(rot);

	if (leftWingMotion == WingMotion::DOWN) {

	}
	else {
		leftForce = glm::vec3(0.0f);
	}


	return glm::vec3(0.0f);
}

glm::vec3 Player::wingAcceleration(double time) {
	return glm::vec3(0.0f);
}



void Player::Draw() {
	// Debug graphics! (A bunch of lines)

	return;
}

void Player::Tick(GLFWwindow* window, float time) {

	// Gravity!
	const double G = 6.67e-11;
	const double EQUUSMASS = 5.972e24;
	const double EQUUSRADIUS = 6.371e6;
	double gravityMagnitude;

	// F1 to switch to levitation
	if (keymap[GLFW_KEY_F1].first && !hasDebugSwitched) {
		if (controlState != ControlState::FREECAM) {
			prevControlState = controlState;
			controlState = ControlState::FREECAM;
		}
		else {
			controlState = prevControlState;
		}
		hasDebugSwitched = true;
	}
	if (!keymap[GLFW_KEY_F1].first) {
		hasDebugSwitched = false;
	}

	// Determine if control regime needs to switch

	int groundY = 0;
	if (position.y < groundY && controlState != ControlState::GROUND_WALKING) {
		velocity.y = 0;
		controlState = ControlState::GROUND_WALKING;
	}
	else if (position.y > groundY && controlState == ControlState::GROUND_WALKING) {
		controlState = ControlState::FREEFALL;
	}


	switch (controlState) {
	case (ControlState::FLIGHT): {
		break;
		// Temporarily skip this part
		glm::vec3 totalForce = glm::vec3(0.0f);
		glm::vec3 totalRotForce = glm::vec3(0.0f);

		gravityMagnitude = G * EQUUSMASS * mass / pow(EQUUSRADIUS + position.y, 2);
		totalForce += glm::vec3(0.0f, gravityMagnitude, 0.0f);

		glm::vec3 finalAcceleration = totalForce / (float)mass;
		glm::vec3 finalRotAcceleration = totalRotForce * momentOfInertia;
		printf("Acc: <%lf, %lf, %lf>\nRotAcc: <%lf, %lf, %lf>\n---------\n",
			finalAcceleration.x, finalAcceleration.x, finalAcceleration.x,
			finalRotAcceleration.x, finalRotAcceleration.x, finalRotAcceleration.x
		);

		// Assume const acceleration for the frame
		//position += velocity * time + finalAcceleration * time * time * 0.5f;
		//rot *= glm::quat(glm::eulerAngles(rotVelocity) * time + finalRotAcceleration * time * time);
		//velocity += finalAcceleration * time;
		//rotVelocity *= finalRotAcceleration * time;
		}
		break;

	case (ControlState::GROUND_WALKING): {
		// Ground requires little use of velocity/acceleration.
		// (This will be different from running, which uses velo/acc)

		// Basically, what to do is
		// Based on current normal of ground, compute the x-z velocity
		// Then put the player there and make y match the ground y
		// Very simple.
		// Also makes walking while looking up/down a bit slower

		// For now, use a constant 0 ground y

		float speed = 0.5f;
		glm::vec3 newVelocity = glm::vec3(0.0f);
		// Movement keys - just wasd. No jumping
		if (keymap[GLFW_KEY_W].first)
		{
			newVelocity += speed * camOrientation;
		}
		if (keymap[GLFW_KEY_A].first)
		{
			newVelocity += speed * -glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_S].first)
		{
			newVelocity += speed * -camOrientation;
		}
		if (keymap[GLFW_KEY_D].first)
		{
			newVelocity += speed * glm::normalize(glm::cross(camOrientation, camUp));
		}

		// Changing velocity takes a teeny bit of time
		position += newVelocity * time + (newVelocity - velocity) * time * time * 0.5f;
		position.y = groundY;

		camPos = position + camRelPos;

		}
		break;

	case (ControlState::FREEFALL): 
		{
		// TEMP: You can only really flail around so much (wasd has very limited movement potential)
		// in the future this will be replaced by orientation control and drag will make u move


		// Gravity!
		glm::vec3 totalForce = glm::vec3(0.0f);
		glm::vec3 totalRotForce = glm::vec3(0.0f);

		gravityMagnitude = G * EQUUSMASS * mass / pow(EQUUSRADIUS + position.y, 2);
		totalForce += glm::vec3(0.0f, -gravityMagnitude, 0.0f);

		glm::vec3 finalAcceleration = totalForce / (float)mass;
		glm::vec3 finalRotAcceleration = totalRotForce * momentOfInertia;

		// TODO: Add drag! (UNREALISTIC, I don't wanna simulate friend-shaped ICBMs)


		//printf("Acc: <%lf, %lf, %lf>\nRotAcc: <%lf, %lf, %lf>\n---------\n",
		//	finalAcceleration.x, finalAcceleration.y, finalAcceleration.z,
		//	finalRotAcceleration.x, finalRotAcceleration.y, finalRotAcceleration.z
		//);

		position += velocity * time + finalAcceleration * time * time * 0.5f;
		//rot *= glm::quat(glm::eulerAngles(rotVelocity) * time + finalRotAcceleration * time * time);
		velocity += finalAcceleration * time;
		//rotVelocity *= finalRotAcceleration * time;

		camPos = position + camRelPos;
		}
		break;

	case (ControlState::FREECAM): 
		{
		// Basically Minecraft freecam hax
		// Does not sync position - after this ends, position returns to physics pos

		float speed = 0.5f;
		// If control is pressed, speed goes higher (Yeah, minecraft inf-drag creative flying)
		if (keymap[GLFW_KEY_LEFT_CONTROL].first)
		{
			speed = 2.0f;
		}

		// Movement keys
		if (keymap[GLFW_KEY_W].first)
		{
			camPos += speed * (float)time * camOrientation;
		}
		if (keymap[GLFW_KEY_A].first)
		{
			camPos += speed * (float)time * -glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_S].first)
		{
			camPos += speed * (float)time * -camOrientation;
		}
		if (keymap[GLFW_KEY_D].first)
		{
			camPos += speed * (float)time * glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_SPACE].first)
		{
			camPos += speed * (float)time * camUp;
		}
		if (keymap[GLFW_KEY_LEFT_SHIFT].first)
		{
			camPos += speed * (float)time * -camUp;
		}

		}
		break;
	}

	// Roll keys (Q, E controls roll)
	float rollSpeed = 25.0f;
	if (keymap[GLFW_KEY_Q].first)
	{
		camUp = glm::rotate(camUp, (float)glm::radians(-rollSpeed * time), glm::normalize(camOrientation));
	}
	if (keymap[GLFW_KEY_E].first)
	{
		camUp = glm::rotate(camUp, (float)glm::radians(rollSpeed * time), glm::normalize(camOrientation));
	}

	// Handles mouse buttons
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hide mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Get cursor position
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		// Doing this (along with rotating orientation) allows for the roll to change as we rotate.
		//Up = glm::rotate(Up, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		//Orientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Does not allow flipping: abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)

		// Calculate new orientation
		glm::vec3 newOrientation = glm::rotate(camOrientation, glm::radians(-rotX), glm::normalize(glm::cross(camOrientation, camUp)));
		// By taking the dot product of the cross product of the original and new orientations and the up vector
		// If the result is negative, the cross products are in different directions and therefore the orientations are on
		// different sides of the up vector.
		glm::vec3 originalCross = glm::cross(camOrientation, camUp);
		glm::vec3 newCross = glm::cross(newOrientation, camUp);
		// Flip the 'Up' vec3 if going through the up vec3
		if (glm::dot(originalCross, newCross) < 0)
		{
			camUp = -camUp;
		}
		camOrientation = newOrientation;

		// Rotates the Orientation left and right
		camOrientation = glm::rotate(camOrientation, glm::radians(-rotY), camUp);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

	// Now that the player has moved, update the matricies!
	Player::updateMatrix();
}

void Player::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		keymap[key] = std::pair(true, glfwGetTime());
	}
	else if (action == GLFW_RELEASE)
	{
		keymap[key] = std::pair(false, glfwGetTime());
	}
	// Debug
	// std::cout << key << "\n";
}

void Player::updateMatrix() {
	projMatrix = glm::perspective(glm::radians(FOVdeg), (double)width / height, nearPlane, farPlane);
	viewMatrix = glm::lookAt(camPos, camPos + camOrientation, camUp);
}

glm::mat4 Player::getProjMatrix() {
	return projMatrix;
}
glm::mat4 Player::getViewMatrix() {
	return viewMatrix;
}
std::vector<glm::vec3> Player::getAxes() {
	// Cast to a mat4 and then take the axes
	glm::mat4 rotMat = glm::mat3(glm::mat4_cast(rot));
	std::vector<glm::vec3> axes;
	axes.push_back(rotMat[0]);
	axes.push_back(rotMat[1]);
	axes.push_back(rotMat[2]);
	return axes;
}
