#ifndef DEBUG_CAMERA_CLASS_H
#define DEBUG_CAMERA_CLASS_H

#include<map>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"../Graphics/Shader.h"

class debugCamera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 1.0f;
	float sensitivity = 100.0f;

	bool keymap[1024];

	debugCamera();
	debugCamera(int width, int height, glm::vec3 position);

	// Gets the conversion matricies for the camera
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

	// Function to call whenever a key is pressed
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void Inputs(GLFWwindow* window, double frameTime);
	// Process window resize events
	void windowResizeCallback(int newWidth, int newHeight);
};

#endif