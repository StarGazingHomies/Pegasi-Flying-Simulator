#include "Sky.h"

//glm::vec3 Sky::skyColourFromTime(double time) {
//	return glm::vec3(1.0f);
//}

Sky::Sky() {}

void Sky::Generate() {
	// Obviously need to draw a sphere

	int vertSegs = 180;
	int hrztSegs = 360;

	// vec3 pos
	// No need for texcoord, since position is effectively a vector that points in that direction!
	

	// First layer, one point. Gimbal lock yay
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	for (int i = 1; i < vertSegs; i++) {
		double pitch = (double) i  / vertSegs;
		for (int j = 0; j < hrztSegs; j++) {
			double yaw = (double) j / hrztSegs * 2;
			double y = cos(pitch * glm::pi<double>());
			double x = sin(pitch * glm::pi<double>()) * cos(yaw * glm::pi<double>());
			double z = sin(pitch * glm::pi<double>()) * sin(yaw * glm::pi<double>());
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			//std::cout << x << ", " << y << ", " << z << ", " << pitch << ", " << yaw << std::endl;
		}
	}

	// Last layer, one point
	vertices.push_back(0.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);

	skyVBO = VBO(vertices);


	
	// EBO needs to do less for first & last layers

	for (int j = 0; j < hrztSegs; j++) {
		indices.push_back(0);
		indices.push_back(j + 1);
		indices.push_back((j + 1) % hrztSegs + 1); // May seem complicated but it's necessary
	}

	for (int i = 1; i < vertSegs - 1; i++) {
		int topLayerBegin = (i - 1) * hrztSegs + 1;
		int bottomLayerBegin = i * hrztSegs + 1;
		for (int j = 0; j < hrztSegs; j++) {
			indices.push_back(topLayerBegin + j);
			indices.push_back(bottomLayerBegin + j);
			indices.push_back(bottomLayerBegin + (j + 1) % hrztSegs);

			indices.push_back(topLayerBegin + j);
			indices.push_back(bottomLayerBegin + (j + 1) % hrztSegs);
			indices.push_back(topLayerBegin + (j + 1) % hrztSegs);
		}
	}

	int lastIndex = (vertSegs - 1) * hrztSegs + 1;
	int lastRowBegin = (vertSegs - 2) * hrztSegs + 1;
	for (int j = 0; j < hrztSegs; j++) {
		indices.push_back(lastIndex);
		indices.push_back(lastRowBegin + j);
		indices.push_back(lastRowBegin + (j + 1) % hrztSegs);
	}

	skyEBO = EBO(indices);

	skyVAO.Bind();
	skyVBO.Bind();
	skyVAO.LinkAttrib(skyVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*) 0 );

	skyVBO.Unbind();
	skyVAO.Unbind();

	sunTex = Texture("resources/sun.png", "sunTex", 0);
	moonTex = Texture("resources/moon.png", "moonTex", 1);
}

void Sky::Draw(Shader& skyShader, glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	glDepthFunc(GL_LEQUAL);

	skyShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(skyShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
	// Cast view to mat3 and then back to mat4 to remove translation
	glUniformMatrix4fv(glGetUniformLocation(skyShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(viewMatrix))));
	glUniform1i(glGetUniformLocation(skyShader.ID, "sunTex"), 0);	
	glUniform1i(glGetUniformLocation(skyShader.ID, "moonTex"), 1);

	skyVAO.Bind();
	skyEBO.Bind();
	sunTex.Bind();
	moonTex.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	skyEBO.Unbind();
	skyVAO.Unbind();

	glDepthFunc(GL_LESS);
}

void Sky::Tick() {

}