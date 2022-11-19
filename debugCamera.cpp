#include"debugCamera.h"

debugCamera::debugCamera()
{
	debugCamera::width = 1;
	debugCamera::height = 1;
	Position = glm::vec3(0, 0, 0);
	memset(keymap, false, sizeof(keymap));
}

debugCamera::debugCamera(int width, int height, glm::vec3 position)
{
	debugCamera::width = width;
	debugCamera::height = height;
	Position = position;
	memset(keymap, false, sizeof(keymap));
}

void debugCamera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matricies, otherwise they will be null
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction
	view = glm::lookAt(Position, Position + Orientation, Up);
	viewMatrix = view;
	// Adds perspective
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
	projectionMatrix = projection;
}
void debugCamera::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the key in keymap to true whenever a key is pressed, and false whenever it is released.
	if (action == GLFW_PRESS)
	{
		keymap[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keymap[key] = false;
	}
	// Debug
	// std::cout << key << "\n";
}

void debugCamera::windowResizeCallback(int newWidth, int newHeight)
{
	debugCamera::width = newWidth;
	debugCamera::height = newHeight;
}

void debugCamera::Inputs(GLFWwindow* window, double frameTime)
{
	// If control is pressed, speed goes higher
	if (keymap[GLFW_KEY_LEFT_CONTROL])
	{
		speed = 2.0f;
	}
	else
	{
		speed = 0.5f;
	}
	// Movement keys
	if (keymap[GLFW_KEY_W])
	{
		Position += speed * (float)frameTime * Orientation;
	}
	if (keymap[GLFW_KEY_A])
	{
		Position += speed * (float)frameTime * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (keymap[GLFW_KEY_S])
	{
		Position += speed * (float)frameTime * -Orientation;
	}
	if (keymap[GLFW_KEY_D])
	{
		Position += speed * (float)frameTime * glm::normalize(glm::cross(Orientation, Up));
	}
	if (keymap[GLFW_KEY_SPACE])
	{
		Position += speed * (float)frameTime * Up;
	}
	if (keymap[GLFW_KEY_LEFT_SHIFT])
	{
		Position += speed * (float)frameTime * -Up;
	}

	// Roll keys (Q, E controls roll)
	float rollSpeed = 25.0f;
	if (keymap[GLFW_KEY_Q])
	{
		Up = glm::rotate(Up, (float)glm::radians(-rollSpeed * frameTime), glm::normalize(Orientation));
	}
	if (keymap[GLFW_KEY_E])
	{
		Up =  glm::rotate(Up, (float)glm::radians(rollSpeed * frameTime), glm::normalize(Orientation));
	}

	// Mouse button
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
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		// By taking the dot product of the cross product of the original and new orientations and the up vector
		// If the result is negative, the cross products are in different directions and therefore the orientations are on
		// different sides of the up vector.
		glm::vec3 originalCross = glm::cross(Orientation, Up);
		glm::vec3 newCross = glm::cross(newOrientation, Up);
		// Flip the 'Up' vec3 if going through the up vec3
		if (glm::dot(originalCross, newCross) < 0)
		{
			Up = -Up;
		}
		Orientation = newOrientation;

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

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
	// Debug position
//	printf("Position: %f, %f, %f\n", Position[0], Position[1], Position[2]);
}