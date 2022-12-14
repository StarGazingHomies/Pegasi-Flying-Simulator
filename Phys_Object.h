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

// Base object for all physics objects
class OBB {
public:
	// Rendering 
	Mesh mesh;
	// Physics
	double mass;
	glm::vec3 half_size;
	glm::quat rotation, rotVelocity, rotAcceleration;
	glm::vec3 position, velocity, acceleration; // Position should actually be center of gravity
	glm::mat3 momentOfInertia = glm::mat3(1.0f);
	OBB();
	void Draw(Shader& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void move(double time);
	void addForce(glm::vec3 force, glm::vec3 position);
	std::vector<glm::vec3> getVertices();
};

bool checkOBBCollision(OBB box1, OBB box2);
int getLeastSeparatingAxis(OBB box1, OBB box2);

// Also need to intersect object with cone and cone frustrum for selection
