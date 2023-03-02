#include "Cloud.h"

bool isValidSphere(std::vector<CloudNode> nodes, CloudNode newNode) {
	return true;
}

Cloud::Cloud(glm::vec3 pos, int branchNum, float initialRadius, float radiusDecay, int numOfIterations) {
	
	this->pos = pos;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0, 2 * glm::pi<float>());

	nodes.push_back(CloudNode{ glm::vec3(0.0f), initialRadius });
	int curStart = 0, curEnd = 1;
	float radius = initialRadius;

	for (int iteration = 1; iteration < numOfIterations; iteration++) {
		radius *= radiusDecay;
		for (int i = curStart; i < curEnd; i++) {
			glm::vec3 parentPos = nodes[i].pos;
			float parentRadius = nodes[i].radius;
			for (int n = 0; n < branchNum; n++) {
				glm::vec3 finalPos;
				for (int tryCnt = 0; tryCnt < 5; tryCnt++) {
					// This will make the points more dense @ top&bottom.
					// Limit range of possible top/bottom deviations
					float r1 = distribution(generator) / 4 + glm::pi<float>() * 0.15;
					float r2 = distribution(generator);
					float x = sin(r1) * cos(r2);
					float y = cos(r1);
					float z = sin(r1) * sin(r2);
					float rFactor = distribution(generator) / glm::pi<float>()
						* radiusDecay / 2 + (1 - radiusDecay / 2);
					finalPos = parentPos + glm::vec3(x, y, z) * parentRadius * rFactor;
					// Check if the resulting sphere collides?
					if (isValidSphere) break;
				}

				// If after 5 tries it's still not valid, add anyway.
				nodes.push_back(CloudNode{ finalPos, radius });
			}
		}

		// Next iteration, expand upon the new nodes
		curStart = curEnd;
		curEnd = nodes.size();
	}

}

Clouds::Clouds() {

}

void Clouds::Generate(int amount, int branchNum, float initialRadius, float radiusDecay, int numOfIterations) {
	// Generate clouds
	for (int i = 0; i < amount; i++) {
		cloudList.push_back(Cloud(glm::vec3(0.0f), branchNum, initialRadius, radiusDecay, numOfIterations));
	}

	// Generate sphere vertex data

	int vertSegs = 18;
	int hrztSegs = 36;

	// vec3 pos
	// No need for texcoord, since position is effectively a vector that points in that direction!


	// First layer, one point.
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	for (int i = 1; i < vertSegs; i++) {
		double pitch = (double)i / vertSegs;
		for (int j = 0; j < hrztSegs; j++) {
			double yaw = (double)j / hrztSegs * 2;
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

	sphereVBO = VBO(vertices);

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

	sphereEBO = EBO(indices);

	sphereVAO.Bind();
	sphereVAO.LinkAttrib(sphereVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);


	// Next, get the instanced data

	for (Cloud c : cloudList) {
		for (CloudNode n : c.nodes) {
			glm::vec3 absPos = n.pos + c.pos;
			instanceData.push_back(absPos.x);
			instanceData.push_back(absPos.y);
			instanceData.push_back(absPos.z);
			instanceData.push_back(n.radius);
		}
	}

	//for (int i = 0; i < instanceData.size(); i++) std::cout << i << ": " << instanceData[i] << std::endl;

	cloudsInstancedVBO = VBO(instanceData);
	sphereVAO.LinkAttribInstanced(cloudsInstancedVBO, 1, 1, 3, GL_FLOAT, 4 * sizeof(float), (void*)0);
	sphereVAO.LinkAttribInstanced(cloudsInstancedVBO, 1, 2, 1, GL_FLOAT, 4 * sizeof(float), (void*)(3 * sizeof(float)));

}

void Clouds::Draw(Shader& cloudShader, glm::mat4 proj, glm::mat4 view) {
	cloudShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(cloudShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	// Cast view to mat3 and then back to mat4 to remove translation
	glUniformMatrix4fv(glGetUniformLocation(cloudShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));


	sphereVAO.Bind();
	sphereEBO.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, cloudList.size() * cloudList[0].nodes.size());

}