/*
* This file contains the class that represents a basic object (OBB).
*/

#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>

#include"../../Graphics/Mesh.h"
#include"../../Graphics/GraphicsPrimitives.h"

// TODO: Actually use this
enum class DebugDrawMode {
	VERTICES,
	WIREFRAME,
	SOLID,
};

inline std::string rotToString(glm::vec3 rot) {
	return std::to_string(glm::length(rot)) + " / " + glm::to_string(normalize(rot));
}

// Also need to think of a debug colouring system - where should the colours be applied?
// Very far down the TODO list though.

//class PhysicsObject {
//public:
//	glm::vec3 position, velocity, acceleration;
//	glm::vec3 rotation, rotVelocity, rotAcceleration;
//	float mass;
//	glm::mat3 momentOfInertia;
//
//	// Geometry methods
//	glm::mat4 getRotationMatrix();
//	glm::mat4 getTranslationMatrix();
//	glm::vec3 inObjectFrame(glm::vec3 point);
//	glm::vec3 inWorldFrame(glm::vec3 point);
//
//	// Basic physics processed here
//	void tick(double time);
//
//	// Collision helpers
//	void addForce(glm::vec3 force, glm::vec3 position);
//	glm::vec3 getAngularVelocityAt(glm::vec3 pos);
//	glm::vec3 getVelocityAt(glm::vec3 pos);
//
//	// Debug methods
//	void debugDraw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
//	std::string toString();
//};

// Base object for most objects
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
	std::string toString();
};

bool checkOBBCollision(OBB box1, OBB box2);
int getLeastSeparatingAxis(OBB box1, OBB box2);

//class OBB2 : public PhysicsObject { // Rewriting
//private:
//	// Sharing a common mesh, w/ transformation matrix
//	static bool initialized;
//	static Mesh debugMesh;
//public:
//	glm::vec3 half_size;
//	OBB2();
//};

// Also need to intersect object with cone and cone frustrum for selection
