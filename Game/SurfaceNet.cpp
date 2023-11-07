#include "SurfaceNet.h"

SurfaceNet::SurfaceNet() {
	arr = Arr3D<double>{ CHUNKSIZE+1, CHUNKSIZE+1, CHUNKSIZE+1 };


	// Seed stuffs? Maybe?
	unsigned int seed = 384729571;
	std::mt19937_64 gen(seed);
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	double xOffset = dist(gen) * 1000;
	double yOffset = dist(gen) * 1000;
	double zOffset = dist(gen) * 1000;
	double scale = 1.0 / 100.0;

	for (int x = 0; x < CHUNKSIZE+1; x++) {
		for (int y = 0; y < CHUNKSIZE+1; y++) {
			for (int z = 0; z < CHUNKSIZE+1; z++) {


				double a = x * scale + xOffset, b = y * scale + yOffset, c = z * scale + zOffset;

				int mid = 8, size = 6;
				double dist = sqrt((x - mid) * (x - mid) + (y - mid) * (y - mid) + (z - mid) * (z - mid)) - size;
				double noise = octavePerlin(a, b, c, 5, 0.7);
				arr.set(x, y, z, noise);

			}
		}
	}

	vao.Bind();

	generate();

	vbo.Bind();
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 9*sizeof(float), 0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 9*sizeof(float), (void*)(3*sizeof(float)));
	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 9*sizeof(float), (void*)(6*sizeof(float)));

	vbo.Unbind();
	vao.Unbind();
}

glm::i32vec3 edgeVertices[12][2] = {
	{glm::i32vec3(0, 0, 0), glm::i32vec3(1, 0, 0)},
	{glm::i32vec3(0, 1, 0), glm::i32vec3(1, 1, 0)},
	{glm::i32vec3(0, 0, 1), glm::i32vec3(1, 0, 1)},
	{glm::i32vec3(0, 1, 1), glm::i32vec3(1, 1, 1)},
	{glm::i32vec3(0, 0, 0), glm::i32vec3(0, 1, 0)},
	{glm::i32vec3(1, 0, 0), glm::i32vec3(1, 1, 0)},
	{glm::i32vec3(0, 0, 1), glm::i32vec3(0, 1, 1)},
	{glm::i32vec3(1, 0, 1), glm::i32vec3(1, 1, 1)},
	{glm::i32vec3(0, 0, 0), glm::i32vec3(0, 0, 1)},
	{glm::i32vec3(1, 0, 0), glm::i32vec3(1, 0, 1)},
	{glm::i32vec3(0, 1, 0), glm::i32vec3(0, 1, 1)},
	{glm::i32vec3(1, 1, 0), glm::i32vec3(1, 1, 1)}
};

void SurfaceNet::generate() {

	Arr3D<SurfaceNetVertex> vertices{ CHUNKSIZE, CHUNKSIZE, CHUNKSIZE };
	std::vector<float> vertexData;

	vertexCount = 0;
	quadCount = 0;

	// Iterate through each cell
	for (int x = 0; x < CHUNKSIZE; x++) {
		for (int y = 0; y < CHUNKSIZE; y++) {
			for (int z = 0; z < CHUNKSIZE; z++) {
				// Iterate through each edge and check for sign changes
				// If there is a sign change, then there is an intersection point
				

				std::vector<glm::vec3> intersections;
				for (int i = 0; i < 12; i++) {
					// Get the two vertices that make up the edge
					glm::i32vec3 v1 = glm::i32vec3(x, y, z) + edgeVertices[i][0];
					glm::i32vec3 v2 = glm::i32vec3(x, y, z) + edgeVertices[i][1];

					// Get the values at each vertex
					double v1Val = arr.get(v1.x, v1.y, v1.z);
					double v2Val = arr.get(v2.x, v2.y, v2.z);

					// Check if there is a sign change
					if ((v1Val < 0 && v2Val > 0) || (v1Val > 0 && v2Val < 0)) {
						// Calculate the intersection point
						float t = v1Val / (v1Val - v2Val);
						glm::vec3 intersection = glm::vec3(v1) + t * glm::vec3(v2 - v1);

						// Add the intersection point to the list
						intersections.push_back(intersection);
					}
				}

				// If there are no intersections, just go away. Shoo!
				if (intersections.size() == 0) continue;

				// Average the intersection points to find the actual vertex
				glm::vec3 vertex = glm::vec3(0, 0, 0);
				for (int i = 0; i < intersections.size(); i++) {
					vertex += intersections[i];
				}
				vertex /= intersections.size();

				// Also find the normal
				glm::vec3 normal = glm::vec3(0, 0, 0);
				for (int i = 0; i < 12; i++) {
					// Get the two vertices that make up the edge
					glm::i32vec3 v1 = glm::i32vec3(x, y, z) + edgeVertices[i][0];
					glm::i32vec3 v2 = glm::i32vec3(x, y, z) + edgeVertices[i][1];
					// Get the direction of that edge
					glm::vec3 edge = v2 - v1;

					// Get the values at each vertex
					double v1Val = arr.get(v1.x, v1.y, v1.z);
					double v2Val = arr.get(v2.x, v2.y, v2.z);

					normal += edge * (float)(v2Val - v1Val);
				}
				normal = glm::normalize(normal);

				// Add the vertex to the list
				vertices.set(x, y, z, SurfaceNetVertex{ vertex, normal, glm::vec3(1.0f), vertexCount++ });
				vertexData.push_back(vertex.x);
				vertexData.push_back(vertex.y);
				vertexData.push_back(vertex.z);
				vertexData.push_back(normal.x);
				vertexData.push_back(normal.y);
				vertexData.push_back(normal.z);
				vertexData.push_back(1.0f);
				vertexData.push_back(1.0f);
				vertexData.push_back(1.0f);

				// Debug print
				// printf("Vertex %d: (%f, %f, %f)\n", vertexCount, vertex.x, vertex.y, vertex.z);
			}
		}
	}
	vbo = VBO{ vertexData };

	// Generate indices
	std::vector<unsigned int> indices;
	for (int x = 1; x < CHUNKSIZE; x++) {
		for (int y = 1; y < CHUNKSIZE; y++) {
			for (int z = 1; z < CHUNKSIZE; z++) {
				if (vertices.get(x, y, z).index == -1) continue;

				// +X direction - look at edge 0
				glm::i32vec3 v1 = glm::i32vec3(x, y, z) + edgeVertices[0][0];
				glm::i32vec3 v2 = glm::i32vec3(x, y, z) + edgeVertices[0][1];
				// Check that the edge has diff signs
				if (arr.get(v1.x, v1.y, v1.z) * arr.get(v2.x, v2.y, v2.z) < 0) {
					// Add quad
					indices.push_back(vertices.get(x, y  , z  ).index);
					indices.push_back(vertices.get(x, y-1, z  ).index);
					indices.push_back(vertices.get(x, y  , z-1).index);
					indices.push_back(vertices.get(x, y-1, z-1).index);
					indices.push_back(vertices.get(x, y  , z-1).index);
					indices.push_back(vertices.get(x, y-1, z  ).index);
					quadCount++;
				}

				// +Y direction - look at edge 4
				v1 = glm::i32vec3(x, y, z) + edgeVertices[4][0];
				v2 = glm::i32vec3(x, y, z) + edgeVertices[4][1];
				// Check that the edge has diff signs
				if (arr.get(v1.x, v1.y, v1.z) * arr.get(v2.x, v2.y, v2.z) < 0) {
					// Add quad
					indices.push_back(vertices.get(x  , y, z  ).index);
					indices.push_back(vertices.get(x-1, y, z  ).index);
					indices.push_back(vertices.get(x  , y, z-1).index);
					indices.push_back(vertices.get(x-1, y, z-1).index);
					indices.push_back(vertices.get(x  , y, z-1).index);
					indices.push_back(vertices.get(x-1, y, z  ).index);
					quadCount++;
				}

				// +Z direction - look at edge 8
				v1 = glm::i32vec3(x, y, z) + edgeVertices[8][0];
				v2 = glm::i32vec3(x, y, z) + edgeVertices[8][1];
				// Check that the edge has diff signs
				if (arr.get(v1.x, v1.y, v1.z) * arr.get(v2.x, v2.y, v2.z) < 0) {
					// Add quad
					indices.push_back(vertices.get(x  , y  , z).index);
					indices.push_back(vertices.get(x-1, y  , z).index);
					indices.push_back(vertices.get(x  , y-1, z).index);
					indices.push_back(vertices.get(x-1, y-1, z).index);
					indices.push_back(vertices.get(x  , y-1, z).index);
					indices.push_back(vertices.get(x-1, y  , z).index);
					quadCount++;
				}
			}
		}
	}
	ebo = EBO{ indices };
}

void SurfaceNet::draw(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	Shader& defaultShader = resourceManager::getShader("default");
	defaultShader.Activate();

	glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
	glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

	vao.Bind();
	ebo.Bind();

	glDrawElements(GL_TRIANGLES, quadCount*6, GL_UNSIGNED_INT, 0);
	ebo.Unbind();
	vao.Unbind();
}


