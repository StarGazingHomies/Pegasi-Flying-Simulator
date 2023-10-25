#include"Phys_Object.h"

OBB::OBB() {
	std::vector<Vertex> vertices = {
		Vertex {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

		Vertex {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
								  		 
		Vertex {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
								  		 
		Vertex {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
								  		 
		Vertex {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
								  		 
		Vertex {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		Vertex {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	};

	std::vector<GLuint> indices = {
		0,1,2,1,2,3,
		4,5,6,5,6,7,
		8,9,10,9,10,11,
		12,13,14,13,14,15,
		16,17,18,17,18,19,
		20,21,22,21,22,23
	};

	std::vector<Texture> textures;

	mass = 1.0;

	mesh = Mesh(vertices, indices, textures);
	rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	// TODO: change moment of inertia based on half_size
	momentOfInertia = glm::mat3(1.0f);
	rotVelocity = glm::quat(glm::vec3(1.0f, 1.0f, 1.0f));
	velocity = glm::vec3(0.0f);
	half_size = glm::vec3(0.5f, 0.5f, 0.5f);
}

void OBB::Draw(Shader& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix) {

	glm::mat4 projection = projMatrix;
	glm::mat4 view = viewMatrix;
	mesh.Draw(shader, projection, view, glm::vec3(0.0f), glm::mat4(1.0f), position, rotation);
}

void OBB::move(double time) {
	// Not gonna use slerp cause that seems wrong
	/*
	while (time > 1) {
		rotation *= rotVelocity;
		time--;
	}
	rotation = glm::slerp(rotation, rotation * rotVelocity, (float)time);
	*/

	rotation *= glm::quat(glm::eulerAngles(rotVelocity) * (float)time + 0.5f * glm::eulerAngles(rotAcceleration) * (float)time * (float)time);
	rotVelocity *= glm::quat(glm::eulerAngles(rotAcceleration) * (float)time);
	rotAcceleration = glm::quat();

	position += velocity * (float)time + 0.5f * acceleration * (float)time * (float)time;
	velocity += acceleration;
	acceleration = glm::vec3(0.0f);
}

void OBB::addForce(glm::vec3 force, glm::vec3 position) {
	acceleration += force / (float)mass;
	rotAcceleration *= glm::quat(glm::cross(force, position) * momentOfInertia);
}



// Collision stuffs
std::pair<double, double> getInterval(glm::vec3 axis, OBB box) {
	std::vector<glm::vec3> verts = box.getVertices();

	double minOnAxis = glm::dot(axis, verts[0]);
	double maxOnAxis = minOnAxis;

	for (int i = 1; i < 8; i++) {
		double t = glm::dot(axis, verts[i]);
		minOnAxis = std::min(t, minOnAxis);
		maxOnAxis = std::max(t, maxOnAxis);
	}
	return std::pair<double, double>(minOnAxis, maxOnAxis);
}

bool checkSeparatingAxis(glm::vec3 axis, OBB box1, OBB box2) {
	std::pair<double, double> i1 = getInterval(axis, box1);
	std::pair<double, double> i2 = getInterval(axis, box2);
	if (i1.first <= i2.second && i2.first <= i1.second) return true;
	else return false;
}

bool checkOBBCollision(OBB box1, OBB box2) {
	glm::mat4 b1_rotMat = glm::mat4_cast(box1.rotation);
	glm::vec3 b1_x = b1_rotMat[0], b1_y = b1_rotMat[1], b1_z = b1_rotMat[2];
	glm::mat4 b2_rotMat = glm::mat4_cast(box2.rotation);
	glm::vec3 b2_x = b2_rotMat[0], b2_y = b2_rotMat[1], b2_z = b2_rotMat[2];
	return (
		checkSeparatingAxis(b1_x, box1, box2) &&
		checkSeparatingAxis(b1_y, box1, box2) &&
		checkSeparatingAxis(b1_z, box1, box2) &&
		checkSeparatingAxis(b2_x, box1, box2) &&
		checkSeparatingAxis(b2_y, box1, box2) &&
		checkSeparatingAxis(b2_z, box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_x, b2_x), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_x, b2_y), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_x, b2_z), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_y, b2_x), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_y, b2_y), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_y, b2_z), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_z, b2_x), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_z, b2_y), box1, box2) &&
		checkSeparatingAxis(glm::cross(b1_z, b2_z), box1, box2));
}

double checkSeparatingAmount(glm::vec3 axis, OBB box1, OBB box2) {
	std::pair<double, double> i1 = getInterval(axis, box1);
	std::pair<double, double> i2 = getInterval(axis, box2);
	return std::min(i2.second - i1.first, i2.first - i1.second);
}

int getLeastSeparatingAxis(OBB box1, OBB box2) {
	glm::mat4 b1_rotMat = glm::mat4_cast(box1.rotation);
	glm::vec3 b1_x = b1_rotMat[0], b1_y = b1_rotMat[1], b1_z = b1_rotMat[2];
	glm::mat4 b2_rotMat = glm::mat4_cast(box2.rotation);
	glm::vec3 b2_x = b2_rotMat[0], b2_y = b2_rotMat[1], b2_z = b2_rotMat[2];

	std::vector<glm::vec3> axes = {
		b1_x, b1_y, b1_z, b2_x, b2_y, b2_z,
		glm::cross(b1_x, b2_x), glm::cross(b1_x, b2_y), glm::cross(b1_x, b2_z),
		glm::cross(b1_y, b2_x), glm::cross(b1_y, b2_y), glm::cross(b1_y, b2_z),
		glm::cross(b1_z, b2_x), glm::cross(b1_z, b2_y), glm::cross(b1_z, b2_z)
	};

	double leastSep = checkSeparatingAmount(axes[0], box1, box2);
	int leastAxis = 0;
	for (int i = 1; i < 15; i++) {
		double curAmt = checkSeparatingAmount(axes[i], box1, box2);
		if (curAmt < leastSep) {
			leastSep = curAmt;
			leastAxis = i;
		}
	}
	return leastAxis;
}

std::vector<glm::vec3> OBB::getVertices() {
	return {
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(half_size.x,  half_size.y,  half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(-half_size.x,  half_size.y,  half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(half_size.x, -half_size.y,  half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(-half_size.x, -half_size.y,  half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(half_size.x,  half_size.y, -half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(-half_size.x,  half_size.y, -half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(half_size.x, -half_size.y, -half_size.z, 1.0f))),
		glm::vec3(position + glm::vec3(glm::mat4_cast(rotation) * glm::vec4(-half_size.x, -half_size.y, -half_size.z, 1.0f)))
	};
}