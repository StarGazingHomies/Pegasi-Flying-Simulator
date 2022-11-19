/*
* This file contains the class that represents an object.
*
*/

#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Mesh.h"
#include"debugCamera.h"

// Base object for all physics objects
class OBB {
public:
	// Rendering 
	Mesh mesh;
	// Physics
	double mass;
	glm::vec3 half_size;
	glm::quat rotation, rotVelocity;
	glm::vec3 position, velocity;
	glm::mat3 momentOfInertia = glm::mat3(1.0f);
	OBB();
	void Draw(Shader& shader, debugCamera camera, double time);
	bool checkCollision();
	void handleCollision();
	std::vector<glm::vec3> getVertices();
};

bool checkOBBCollision(OBB box1, OBB box2);
glm::vec3 getLeastSeparatingAxis(OBB box1, OBB box2);
