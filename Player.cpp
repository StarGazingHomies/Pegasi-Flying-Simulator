#include"Player.h"

const double PI = 3.14159265358979323846;

void debugVec3(std::string name, glm::vec3 vec) {
	printf("%s: <%.2f, %.2f, %.2f>\n", name.c_str(), vec.x, vec.y, vec.z);
}

std::string debugVec3Str(std::string name, glm::vec3 vec) {
	return std::format("{}: <{:.2f}, {:.2f}, {:.2f}> / {:.2f}", name.c_str(), vec.x, vec.y, vec.z, glm::length(vec));
}

std::string debugQuatStr(std::string name, glm::quat quat) {
	glm::vec3 eulerAngles = glm::eulerAngles(quat);
	return std::format("{}: <{:.4f}, {:.4f}, {:.4f}, {:.4f}> / <{:.2f}, {:.2f}, {:.2f}>",
		name.c_str(), quat.x, quat.y, quat.z, quat.w, eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

std::string debugFloatStr(std::string name, float num) {
	return std::format("{}: {:.2f}", name.c_str(), num);
}

std::string debugIntStr(std::string name, int num) {
	return std::format("{}: {}", name.c_str(), num);
}

// Non player specific helpers
void debugMat4(std::string name, glm::mat4 mat4) {
	printf("%s:\n<%.2f, %.2f, %.2f, %.2f>\n<%.2f, %.2f, %.2f, %.2f>\n<%.2f, %.2f, %.2f, %.2f>\n<%.2f, %.2f, %.2f, %.2f>\n",
		name.c_str(),
		mat4[0][0],
		mat4[0][1],
		mat4[0][2],
		mat4[0][3],
		mat4[1][0],
		mat4[1][1],
		mat4[1][2],
		mat4[1][3],
		mat4[2][0],
		mat4[2][1],
		mat4[2][2],
		mat4[2][3],
		mat4[3][0],
		mat4[3][1],
		mat4[3][2],
		mat4[3][3]);
}

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -0.999f) {
		// Opposite directions --> Axis doesn't really matter
		rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		if (glm::length(rotationAxis) < 0.01)
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	// trig stuff
	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

double rotationalInterpolation(double a, double b, double md) {
	// Direction
	constexpr double PI = glm::pi<double>();
	// Regular distance + dir
	double rdst = abs(a - b);
	double rdir = (b - a) / abs(a - b);

	// Alternate (through 180deg) distance + dir
	double adst = abs(PI - abs(a));
	double bdst = abs(PI - abs(b));
	double altdst = adst + bdst;
	double altdir = a / abs(a);

	if (altdst < rdst) {
		if (altdst < md) return b;
		double result = a + altdir * md;
		if (result < -PI) return result + PI * 2;
		if (result > +PI) return result - PI * 2;
		return result;
	}
	else {
		if (rdst < md) return b;
		return a + rdir * md;
	}
}

glm::quat scale(glm::quat q, float factor) {
	return glm::angleAxis(glm::angle(q) * factor, glm::axis(q));
}

// Player class
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

	// Initialize keymap
	for (int i = 0; i < 1024; i++) {
		keymap.push_back(keyStatus{ -1, 0.0, 0 });
	}

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
	std::vector<float> vertices = {
		  0.0f,  0.0f, 0.0f,
		  0.1f, -0.1f, 0.2f,
		  0.1f,  0.1f, 0.2f,
		 -0.1f, -0.1f, 0.2f,
		 -0.1f,  0.1f, 0.2f,
			0.0f, 0.0f, 0.0f,
	};

	std::vector<GLuint> indices = {
		0,1,2,
		0,2,4,
		0,4,3,
		0,3,1
	};

	debugCamVAO.Bind();
	debugCamVBO = VBO(vertices);
	debugCamVAO.LinkAttrib(debugCamVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	debugCamVAO.LinkAttrib(debugCamVBO, 1, 3, GL_FLOAT, 0, (void*)(15 * sizeof(float)));
	debugCamVAO.LinkAttrib(debugCamVBO, 2, 3, GL_FLOAT, 0, (void*)(15 * sizeof(float)));
	debugCamVAO.LinkAttrib(debugCamVBO, 3, 2, GL_FLOAT, 0, (void*)(15 * sizeof(float)));

	debugCamEBO = EBO(indices);

	std::vector<float> bodyVertices = {
		-0.5f, 0.5f, -0.2f,
		 0.5f, 0.5f, -0.2f,
		-0.5f, 0.5f,  0.2f,
		 0.5f, 0.5f,  0.2f,
		-0.5f, 0.7f, -0.2f,
		 0.5f, 0.7f, -0.2f,
		-0.5f, 0.7f,  0.2f,
		 0.5f, 0.7f,  0.2f,
		 0.0f, 0.0f,  0.0f
	};

	std::vector<GLuint> bodyIndices = {
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		0,1,4,0,4,5,
		2,3,6,2,6,7,
		0,2,4,0,4,6,
		1,3,5,1,5,7
	};

	debugBodyVAO.Bind();
	debugBodyVBO = VBO(bodyVertices);
	debugBodyVAO.LinkAttrib(debugBodyVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	debugBodyVAO.LinkAttrib(debugBodyVBO, 1, 3, GL_FLOAT, 0, (void*)(24 * sizeof(float)));
	debugBodyVAO.LinkAttrib(debugBodyVBO, 2, 3, GL_FLOAT, 0, (void*)(24 * sizeof(float)));
	debugBodyVAO.LinkAttrib(debugBodyVBO, 3, 2, GL_FLOAT, 0, (void*)(24 * sizeof(float)));

	debugBodyEBO = EBO(bodyIndices);
}

std::string Player::getControlStateString() {
	switch (controlState)
	{
	case ControlState::GROUND_WALKING:
		return "Ground/Walking";
	case ControlState::GROUND_RUNNING:
		return "Ground/Running";
	case ControlState::GROUND_TIPTOE:
		return "Ground/Tiptoe";
	case ControlState::GROUND_OBJAVOID:
		return "Ground/ObjectAvoid";
	case ControlState::WATER_SURFACE:
		return "Water/Swimming";
	case ControlState::WATER_DIVING:
		return "Water/Diving";
	case ControlState::FLIGHT:
		return "Flight";
	case ControlState::FREEFALL:
		return "CarpetPonking";
	case ControlState::LEVITATION:
		return "Starlight";
	case ControlState::FREECAM:
		return "FreeCam";
	}
	return "Unknown";
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

double Player::wingEfficiency(float angleOfAttack)
{
	float divingEfficiency = 0.8f;
	float divingCriticalAoa = -30.0f;
	float criticalAoaMin = 20.0f;
	float criticalAoaMax = 25.0f;
	if (angleOfAttack > 90 or angleOfAttack < -90)
	{
		// What the fuck are you doing to fly backwards ?
		// At this stage, only flying will save you.
		// Oh, and remember that you might spin out of control.That will be implemented some other time.
		return 0.2f;
	}

	if (angleOfAttack < divingCriticalAoa)
		// 0.8
		return divingEfficiency;
	else if (angleOfAttack < 0)
		// 0.8 ~ 1
		return 1.0f - (1.0f - divingEfficiency) * (angleOfAttack / divingCriticalAoa);
	else if (angleOfAttack < criticalAoaMin)
		// 1 ~ 1.2
		return 1 + angleOfAttack / 5 / criticalAoaMin;
	else if (angleOfAttack <= criticalAoaMax)
		// 1.2
		return 1.2f;
	else
		// At 50 degrees the efficiency is 0.2 (the minumum, for now)
		return std::max(1.2f - (angleOfAttack - criticalAoaMax) / 25, 0.2f);
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



void Player::Draw(Shader& shader) {
	// Debug graphics! (A bunch of lines)
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix * viewMatrix));

	if (controlState == ControlState::FREECAM) {
		glm::quat camRot = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), camOrientation);
		// camUp isn't actually 90 degrees to camera!
		// Cross it twice so it's perpendicular to camOrientation
		glm::vec3 desiredUp = glm::normalize(glm::cross(glm::cross(camUp, camOrientation), camOrientation));

		// Make sure it's upright
		glm::vec3 newUp = camRot * glm::vec3(0.0f, 1.0f, 0.0f);
		glm::quat camRot2 = RotationBetweenVectors(newUp, desiredUp);

		// First thing is applied last
		camRot = camRot2 * camRot;

		//debugVec3("Rotation EulerAngles: ", glm::eulerAngles(camRot));
		glm::mat4 camRotMat = glm::mat4_cast(camRot);
		//debugMat4("Rotation Mat4: ", camRotMat);
		glm::mat4 translation = glm::mat4(1.0f);
		translation = glm::translate(translation, position + camUp);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(camRotMat));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(translation));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

		debugCamVAO.Bind();
		debugCamVBO.Bind();
		debugCamEBO.Bind();

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}

	glm::mat4 bodyRotMat = glm::mat4_cast(rot);
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, position);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(bodyRotMat));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(translation));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	debugBodyVAO.Bind();
	debugBodyVBO.Bind();
	debugBodyEBO.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	return;
}

void Player::debugText(Font& font, Shader& fontShader) {
	font.renderLine("Control state: " + getControlStateString(), 0, 580, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(debugVec3Str("CamPos: ", camPos), 0, 560, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(debugVec3Str("Position: ", position), 0, 540, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(debugVec3Str("Velocity: ", velocity), 0, 520, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(debugVec3Str("Temp Debug Field 1: ", debugTempVec1), 0, 500, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(debugVec3Str("Temp Debug Field 2: ", debugTempVec2), 0, 480, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderLine(std::format("Stamina: {:.2f} / {:.2f}", stamina, maxStamina), 0, 460, 20, glm::vec3(1.0f, 1.0f, 0.0f));
	font.renderAll(fontShader);
}

void Player::Tick(GLFWwindow* window, float time) {

	// Roll keys (Q, E controls roll)
	float rollSpeed = 25.0f;
	if (keymap[GLFW_KEY_Q].status == 1 && keymap[GLFW_KEY_E].status == 1) {
		// Reset
		camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// When the keys are unpressed, there should not be any more rot.
		keyClear(GLFW_KEY_Q);
		keyClear(GLFW_KEY_E);
	}
	else if (keymap[GLFW_KEY_Q].status == 1)
	{
		camUp = glm::rotate(camUp, (float)glm::radians(-rollSpeed * time), glm::normalize(camOrientation));
	}
	else if (keymap[GLFW_KEY_E].status == 1)
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

	// Gravity!
	const double G = 6.67e-11;
	const double EQUUSMASS = 5.972e24;
	const double EQUUSRADIUS = 6.371e6;
	double gravityMagnitude;

	// F1 to switch to levitation
	if (keymap[GLFW_KEY_F1].status) {
		if (controlState != ControlState::FREECAM) {
			prevControlState = controlState;
			controlState = ControlState::FREECAM;
		}
		else {
			controlState = prevControlState;
		}
		keyClear(GLFW_KEY_F1);
	}

	if (keymap[GLFW_KEY_F5].status) {
		if (camState == CameraState::FIRST_PERSON) camState = CameraState::THIRD_PERSON;
		else if (camState == CameraState::THIRD_PERSON) camState = CameraState::FIRST_PERSON;
		keyClear(GLFW_KEY_F5);
	}

	// Determine if control regime needs to switch

	// For now, use a constant 0 ground y
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
		printf("Flight is not implemented yet!\n");
		controlState = ControlState::FREEFALL;
		break;
		glm::vec3 totalForce = glm::vec3(0.0f);
		glm::vec3 totalRotForce = glm::vec3(0.0f);

		gravityMagnitude = G * EQUUSMASS * mass / pow(EQUUSRADIUS + position.y, 2);
		totalForce += glm::vec3(0.0f, -gravityMagnitude, 0.0f);

		// No flapping for now, only gliding

		std::vector<glm::vec3> axes = getAxes();
		glm::vec3 axisX = axes[0], axisY = axes[1], axisZ = axes[2];
		float primaryAngle = 0, secondaryAngle = 0;
		float PRIMARY_LIFT_COEFFICIENT = 1.3;
		float SECONDARY_LIFT_COEFFICIENT_CLOSED = 1.1;
		float SECONDARY_LIFT_COEFFICIENT_OPEN = -0.1;
		float air_density = 1;

		//Primary
		glm::vec3 primaryAirDirection = -velocity;
		glm::vec3 primaryLiftDirection = glm::rotate(glm::rotate(primaryAirDirection, glm::radians(-80.0f), axisZ), primaryAngle,
			axisX);
		float primaryLiftMagnitude = 0.5f * PRIMARY_LIFT_COEFFICIENT * (
			0.66 * (wingSpan - humurusLength) * primaryFeatherLength) * \
			air_density * glm::length(primaryAirDirection);
		glm::vec3 primaryLiftForce = primaryLiftDirection * primaryLiftMagnitude;
		totalForce += primaryLiftForce;

		//Secondary
		glm::vec3 secondaryAirDirection = -velocity;
		glm::vec3 secondaryLiftDir = glm::rotate(glm::rotate(secondaryAirDirection, glm::radians(-80.0f), axisZ), secondaryAngle,
			axisX);
		float secondaryLiftMagnitude = 0.5 * SECONDARY_LIFT_COEFFICIENT_CLOSED * (humurusLength * primaryFeatherLength) * \
			air_density * glm::length(secondaryAirDirection);
		glm::vec3 secondaryLiftForce = secondaryLiftDir * secondaryLiftMagnitude;
		totalForce += secondaryLiftForce;


		glm::vec3 finalAcceleration = totalForce / (float)mass;
		glm::vec3 finalRotAcceleration = totalRotForce * momentOfInertia;
		debugVec3("Acc", finalAcceleration);
		debugVec3("RotAcc", finalRotAcceleration);

		// Assume const acceleration for the frame
		position += velocity * time + finalAcceleration * time * time * 0.5f;
		//rot *= glm::quat(glm::eulerAngles(rotVelocity) * time + finalRotAcceleration * time * time);
		velocity += finalAcceleration * time;
		//rotVelocity *= finalRotAcceleration * time;

		camPos = position + camRelPos;
	}
	if (keymap[GLFW_KEY_1].status == 1) {
		position = glm::vec3(0.0f, 100.0f, 0.0f);
	}
	break;

	case (ControlState::GROUND_WALKING): {
		// Ground requires little use of velocity/acceleration.
		// (This will be different from running, which uses velo/acc)


		// Target direction
		glm::vec3 target = glm::vec3(0.0f);
		if (keymap[GLFW_KEY_W].status == 1) {
			target += glm::normalize(camOrientation);
		}
		if (keymap[GLFW_KEY_A].status == 1) {
			target += -glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_S].status == 1) {
			target += -glm::normalize(camOrientation);
		}
		if (keymap[GLFW_KEY_D].status == 1) {
			target += glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (stamina <= 0) {
			target = glm::vec3(0.0f);
		}

		// You can't walk up
		target.y = 0;
		float directionFactor = 1;
		// Normalize the target direction, if the target isn't stopping
		if (target != glm::vec3(0.0f)) {
			target = glm::normalize(target);
			// You go a bit faster if you (and your body) face in the right direction
			directionFactor =
				(1 - walkingLookImportance - walkingBodyImportance)
				+ walkingLookImportance * glm::dot(target, camOrientation)
				+ walkingBodyImportance * glm::dot(target, getAxes()[0]);
		}
		// Find target velocity
		target *= walkingTargetSpeed * directionFactor;

		// Natural velocity loss, and walking acceleration to keep up / change direction
		glm::vec3 retainedVelocity = velocity * (1 - (1 - walkingRetention) * time);
		glm::vec3 diffVelocity = target - retainedVelocity;
		float diffLength = glm::length(diffVelocity);
		float resultAccMagnitude = 0;
		double percentAcc = 0;
		glm::vec3 acceleration = glm::vec3(0.0f);
		if (target == glm::vec3(0.0f)) {
			// If you are stopping
			// (Stamina cost of stopping is 0, for simplification)
			if (diffLength > walkingInstantStop) {
				// If the diff is beeg
				acceleration = glm::normalize(diffVelocity) * walkingStopAcceleration;
			}
			else {
				// If the diff is smol
				acceleration = diffVelocity / time;
			}
		}
		else {
			// If the difference is too large to be ignored:
			resultAccMagnitude = glm::min(diffLength / time, walkingMaxAcceleration * directionFactor);
			if (resultAccMagnitude != 0)
				acceleration = glm::normalize(diffVelocity) * resultAccMagnitude;

			// Consume stamina using resultAccMagnitude and directionFactor
			// (Going backwards takes a lot!)
			percentAcc = resultAccMagnitude / walkingMaxAcceleration;
			double staminaFactor = percentAcc * std::pow(directionFactor, -walkingDirectionStaminaImpact);
			stamina -= staminaFactor * walkingMaxAccStaminaCost * time;
		}

		// Ground handling - ignored for now
		float groundSlope = 0;




		// Jumping, Pt. 1
		glm::quat originalRot{};
		if (keymap[GLFW_KEY_SPACE].status == 1) {
			// Record some data so we know what exactly happened in the last frame we had on the ground
			originalRot = rot;
		}

		// Linearly interpolate between velocities
		position += (velocity + retainedVelocity) * time * 0.5f + acceleration * time * time * 0.5f;
		position.y = groundY; // Crude ground handling for now
		velocity = retainedVelocity + acceleration * time;

		// Turning
		glm::vec3 targetEulerAngles = glm::vec3(0.0f);
		glm::vec3 originalEulerAngles = glm::eulerAngles(rot);
		if (walkingAutoTurn && velocity != glm::vec3(0.0f) && target != glm::vec3(0.0f)) {
			// TODO: Need to check if originalEulerAngles is flipped upside down...
			if (abs(originalEulerAngles.x) > glm::pi<double>() && abs(originalEulerAngles.z) > glm::pi<double>()) {
				if (originalEulerAngles.y < 0) originalEulerAngles.y = - glm::pi<double>() - originalEulerAngles.y;
				else originalEulerAngles.y = glm::pi<double>() - originalEulerAngles.y;
			}
			targetEulerAngles.y = RHSignedAngle(
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::normalize(
					walkingVelocityDirWeight * velocity +
					walkingDirWeight * getAxes()[0] +
					walkingTargetDirWeight * target),
				glm::vec3(0.0f, 1.0f, 0.0f));

			debugTempVec1 = originalEulerAngles;
			debugTempVec2 = targetEulerAngles;

			targetEulerAngles.y = rotationalInterpolation(originalEulerAngles.y, targetEulerAngles.y, walkingMaxTurnSpeed*time);
			rot = glm::quat(targetEulerAngles);
		}

		// Regen stamina
		stamina = std::min(stamina + staminaRegen * time, maxStamina);

		// Turn body / update camera accordingly
		if (camState == CameraState::FIRST_PERSON) {
			camPos = position + camRelPos;
		}
		else {
			camPos = position + camRelPos - camOrientation * camThirdPersonDistance;
		}

		// Jumping, Pt. 2
		if (keymap[GLFW_KEY_SPACE].status == 1) {

			// Don't want to instantly switch to flying, so space needs to be pressed again
			keyClear(GLFW_KEY_SPACE);

			// Add velocity for jump, since acceleration will be almost instantaneous anyway.
			// TODO: At some point, make holding space charge your jump
			velocity = retainedVelocity + glm::vec3(0.0f, jumpVelocity, 0.0f); // Replace with ground vec3 when I get there



			// The rotation this frame
			rotVelocity = rot * scale(originalRot, -1);

			// The rotation per second
			rotVelocity = glm::angleAxis(glm::angle(rotVelocity) / time, glm::axis(rotVelocity));

			std::cout << debugQuatStr("originalRot", originalRot) << "\n";
			std::cout << debugQuatStr("rot", rot) << "\n";
			std::cout << debugQuatStr("rotVelocity", rotVelocity) << "\n";

			// Now we are not on the ground anymore
			controlState = ControlState::FREEFALL;
			break;
		}

		// Debug stuff
	}
	break;

	case (ControlState::FREEFALL):
	{
		// TODO: You can only really flail around so much (wasd has very limited movement potential)
		// in the future this will be replaced by orientation control and drag will make u move

		// Gravity!
		glm::vec3 totalForce = glm::vec3(0.0f);
		glm::vec3 totalRotForce = glm::vec3(0.0f);

		gravityMagnitude = G * EQUUSMASS * mass / pow(EQUUSRADIUS + position.y, 2);
		totalForce += glm::vec3(0.0f, -gravityMagnitude, 0.0f);

		glm::vec3 finalAcceleration = totalForce / (float)mass;
		glm::vec3 finalRotAcceleration = totalRotForce * momentOfInertia;

		// TODO: Add drag! (No lift, I don't wanna simulate friend-shaped ICBMs)


		/*printf("Acc: <%lf, %lf, %lf>\nRotAcc: <%lf, %lf, %lf>\n---------\n",
			finalAcceleration.x, finalAcceleration.y, finalAcceleration.z,
			finalRotAcceleration.x, finalRotAcceleration.y, finalRotAcceleration.z
		);*/

		//std::cout << debugQuatStr("rotVelocity", rotVelocity) << std::endl;

		position += velocity * time + finalAcceleration * time * time * 0.5f;
		rot *= scale(rotVelocity, time) * scale(finalRotAcceleration, time * time * 0.5f);
		velocity += finalAcceleration * time;
		rotVelocity *= scale(finalRotAcceleration, time);

		camPos = position + camRelPos;
	}

	// Spread your wings and fly!
	if (keymap[GLFW_KEY_SPACE].status == 1) {
		controlState = ControlState::FLIGHT;
		break;
	}
	break;

	case (ControlState::FREECAM):
	{
		// Basically Minecraft freecam hax
		// Does not sync position - after this ends, position returns to physics pos

		float speed = 2.5f;
		// If control is pressed, speed goes higher (Yeah, minecraft inf-drag creative flying)
		if (keymap[GLFW_KEY_LEFT_CONTROL].status == 1)
		{
			speed = 20.0f;
		}

		// Movement keys
		if (keymap[GLFW_KEY_W].status == 1)
		{
			camPos += speed * (float)time * camOrientation;
		}
		if (keymap[GLFW_KEY_A].status == 1)
		{
			camPos += speed * (float)time * -glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_S].status == 1)
		{
			camPos += speed * (float)time * -camOrientation;
		}
		if (keymap[GLFW_KEY_D].status == 1)
		{
			camPos += speed * (float)time * glm::normalize(glm::cross(camOrientation, camUp));
		}
		if (keymap[GLFW_KEY_SPACE].status == 1)
		{
			camPos += speed * (float)time * camUp;
		}
		if (keymap[GLFW_KEY_LEFT_SHIFT].status == 1)
		{
			camPos += speed * (float)time * -camUp;
		}

	}
	break;
	}

	// Now that the player has moved, update the matricies!
	Player::updateMatrix();
}

void Player::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	double time = glfwGetTime();
	if (action == GLFW_PRESS)
	{
		keymap[key].status = true;
		if (time - keymap[key].lastPress < doubleClickTime) {
			keymap[key].consecutiveClicks++;
		}
		else {
			keymap[key].consecutiveClicks = 1;
		}
		//Debug
		//printf("%d is pressed! The key's last press is at %lf, so it has %d consecutive clicks.\n", 
		//	key, keymap[key].lastPress, keymap[key].consecutiveClicks);

		keymap[key].lastPress = time;
	}
	else if (action == GLFW_RELEASE)
	{
		keymap[key].status = false;
	}
}

void Player::keyClear(int key) {
	keymap[key].consecutiveClicks = 0;
	keymap[key].status = false;
}

void Player::updateMatrix() {
	projMatrix = glm::perspective(glm::radians(FOVdeg), (double)width / height, nearPlane, farPlane);
	//viewMatrix = glm::lookAt(camPos, camPos + camOrientation * glm::mat3(glm::mat4_cast(rot)), camUp);
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
