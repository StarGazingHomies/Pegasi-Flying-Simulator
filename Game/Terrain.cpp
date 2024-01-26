#include "Terrain.h"

HeightmapTerrain::HeightmapTerrain() :
height(-1),
width(-1),
resX(-1),
resY(-1),
posX(-1),
posY(-1),
scale(-1),
shift(-1)
{}

void HeightmapTerrain::Load(const char* file) {
	// TODO
}

void HeightmapTerrain::Generate(int x, int y, int w, int h, int rx, int ry,
	std::function<float(float, float)> genFunc) {

	if (!vertices.empty()) vertices.clear();
	if (!indices.empty()) indices.clear();
	if (!heights.empty()) heights.clear();

	posX = x; posY = y;
	width = w; height = h;
	resX = rx; resY = ry;

	// VBO + heightMap

	for (int col = 0; col <= rx; col++) {
		float xCoord = (float)col / rx * w + x;

		for (int row = 0; row <= ry; row++) {
			float yCoord = (float)row / ry * h + y;

			// Height is stored in a texture, not the actual vertex data
			// Consider maybe removing this 0 and using a vec2, then converting into vec3 in shaders?
			vertices.push_back(xCoord);
			vertices.push_back(0); 
			vertices.push_back(yCoord);
			// Texcoords
			vertices.push_back((float)col / rx);
			vertices.push_back((float)row / ry);

			float h = genFunc(xCoord, yCoord);
			heights.push_back(h);
		}
	}

	terrainVAO.Bind();
	terrainVBO = VBO(vertices);
	terrainVBO.Bind();

	terrainVAO.LinkAttrib(terrainVBO, 0, 3, GL_FLOAT, 5 * sizeof(GL_FLOAT), (void*) 0);
	terrainVAO.LinkAttrib(terrainVBO, 1, 2, GL_FLOAT, 5 * sizeof(GL_FLOAT), (void*) (3*sizeof(GL_FLOAT)));
	heightMap = Texture::hmapTexture(heights, rx+1, ry+1);

	terrainVBO.Unbind();
	terrainVAO.Unbind();

	//heightMap = Texture(heights.data());

	// EBO
	// +--> +y
	// |
	// |
	// v +x
	// 
	//    0     1    2    3    4  ...  w-1    w  (each row has w+1 elements)
	//  w+1   w+2  w+3  w+4  w+5  ...  2w  2w+1
	// 2w+2  2w+3  ... 
	// we have 0, w+1, 1, w+2 to cover each quad

	for (int col = 0; col < rx; col++) {
		for (int row = 0; row < ry; row++) {
			int curIndex = col * (ry + 1) + row;
			indices.push_back(curIndex);
			indices.push_back(curIndex + ry + 1);
			indices.push_back(curIndex + 1);
			indices.push_back(curIndex + ry + 2);
		}
	}

	terrainEBO = EBO(indices);
}

int HeightmapTerrain::getIndex(int x, int y) {
	return 5 * (x * (resY + 1) + y);
}

std::vector<float> HeightmapTerrain::getCoord(int x, int y) {
	return std::vector<float>(vertices.begin() + getIndex(x, y), vertices.begin() + getIndex(x, y) + 3);
}
void HeightmapTerrain::setCoord(int x, int y, std::vector<float> vals) {

}

void HeightmapTerrain::Draw(const Shader& terrainShader, glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	terrainShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(terrainShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(terrainShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3f(glGetUniformLocation(terrainShader.ID, "camPos"), camPos.x, camPos.y, camPos.z);

	terrainVAO.Bind();
	terrainEBO.Bind();
	heightMap.Bind();

	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_PATCHES, 0, 4);

}

Chunk::Chunk(int x, int y, int z, Arr3D<double> data) :
	surfaceNet(
		glm::vec3(chunkSize)*glm::vec3(x, y, z),
		glm::vec3(chunkSize)*(glm::vec3(x, y, z) + glm::vec3(1 + 1.0/chunkPrecision)),
		data) {
	chunkX = x; chunkY = y; chunkZ = z;

	// Random texture data, generated here for now
	std::vector<float> texture = std::vector<float>();
	srand(x * 73856093 ^ y * 19349663 ^ z * 83492791);
	PerlinNoiseGenerator perlin = PerlinNoiseGenerator(rand(), 6, 0.7, 2.0, 0.002);
	for (int i = 0; i < grassTextureSize; i++) {
		for (int j = 0; j < grassTextureSize; j++) {
			// Random colour between yellow and brown
			glm::vec3 yellow = glm::vec3(1.0, 1.0, 0.0);
			glm::vec3 brown = glm::vec3(0.0, 1.0, 0.0);
			float val = perlin.generate(i, j, 0.0);
			glm::vec3 colour = yellow * val + brown * (1 - val);
			texture.push_back(colour.x);
			texture.push_back(colour.y);
			texture.push_back(colour.z);
			texture.push_back((float)rand() / RAND_MAX);
		}
	}
	this->grass = Texture{ texture, grassTextureSize, grassTextureSize, 4, "grass", 0, false };

	grassLayerOffsets = std::vector<glm::vec3>(grassNumShells, glm::vec3(0.0f));
	for (int i = 0; i < grassNumShells; i++) {
		grassLayerOffsets[i] = glm::vec3(0.0f, (float)(i+1) / (float)grassNumShells, 0.0f);
		//printf("Offset %d: (%f, %f, %f)\n", i, grassLayerOffsets[i].x, grassLayerOffsets[i].y, grassLayerOffsets[i].z);
	}

	std::vector<float> grassOffsets = std::vector<float>();
	float maxBaseOffset = 0.5f;
	float maxTopOffset = 1.0f;
	for (int i = 0; i < grassTextureSize; i++) {
		for (int j = 0; j < grassTextureSize; j++) {
			grassOffsets.push_back((float)rand() / RAND_MAX * maxBaseOffset);
			grassOffsets.push_back((float)rand() / RAND_MAX * maxBaseOffset);
			grassOffsets.push_back((float)rand() / RAND_MAX * maxTopOffset);
			grassOffsets.push_back((float)rand() / RAND_MAX * maxTopOffset);
		}
	}
	
	this->grassOffsets = Texture{ grassOffsets, grassTextureSize, grassTextureSize, 4, "grassOffsets", 1, false };

	// Set up the grass VAO to have instanced attributes
	grassLayerVBO = VBO{};
	grassLayerVBO.Initialize();
	grassLayerVBO.Bind();
	static_assert(sizeof(glm::vec3) == sizeof(GLfloat) * 3, "Platform does not support glm::vec3 casting directly");
    glBufferData(GL_ARRAY_BUFFER, grassLayerOffsets.size() * sizeof(glm::vec3), grassLayerOffsets.data(), GL_DYNAMIC_DRAW);
	grassLayerVBO.Unbind();

	grassVAO.Bind();

	grassVAO.LinkAttrib(surfaceNet.vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
	grassVAO.LinkAttrib(surfaceNet.vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	grassVAO.LinkAttribInstanced(grassLayerVBO, 1, 2, 3, GL_FLOAT, 3 * sizeof(float), 0);
}

void Chunk::draw(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	if (surfaceNet.quadCount == 0) return;

	// Surface
	//std::cout << "Drawing chunk " << chunkX << ", " << chunkY << ", " << chunkZ << std::endl;
	Shader& terrainShader = resourceManager::getShader("Terrain/default");
	terrainShader.Activate();

	glUniformMatrix4fv(glGetUniformLocation(terrainShader.ID, "proj"), 1, GL_FALSE, glm::value_ptr(projMatrix));
	glUniformMatrix4fv(glGetUniformLocation(terrainShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glUniform1i(glGetUniformLocation(terrainShader.ID, "grass"), 0);
	grass.Bind();
	surfaceNet.vao.Bind();
	surfaceNet.ebo.Bind();

	glDrawElements(GL_TRIANGLES, surfaceNet.quadCount * 6, GL_UNSIGNED_INT, 0);

	surfaceNet.ebo.Unbind();
	surfaceNet.vao.Unbind();

	// Grass
	Shader& terrainGrassShader = resourceManager::getShader("Terrain/grass");
	terrainGrassShader.Activate();

	glUniformMatrix4fv(glGetUniformLocation(terrainGrassShader.ID, "proj"), 1, GL_FALSE, glm::value_ptr(projMatrix));
	glUniformMatrix4fv(glGetUniformLocation(terrainGrassShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glUniform1i(glGetUniformLocation(terrainGrassShader.ID, "grass"), 0);
	grass.Bind();
	glUniform1i(glGetUniformLocation(terrainGrassShader.ID, "grassOffsets"), 1);
	grassOffsets.Bind();

	grassVAO.Bind();
	surfaceNet.ebo.Bind();

	glDrawElementsInstanced(GL_TRIANGLES, surfaceNet.quadCount * 6, GL_UNSIGNED_INT, 0, grassLayerOffsets.size());

	surfaceNet.ebo.Unbind();
	grassVAO.Unbind();
}

void Chunk::tick(double deltaTime) {
	glm::vec3 wind = glm::vec3(0.0, 0.0, 1.0); // Const for now
	for (int i = grassNumShells - 1; i >= 1; i--) {
		// Grass tries to pull itself upright
		glm::vec3 pull = grassLayerOffsets[i - 1] - grassLayerOffsets[i];
		// Wind pushes it over
		glm::vec3 push = wind * (float)i / (float)grassNumShells;
		// Do the actual movement
		grassLayerOffsets[i] += (pull + push) * (float)deltaTime;
	}
}

double Chunk::getValue(int x, int y, int z) {
	return surfaceNet.arr.get(x, y, z);
}

std::shared_ptr<Chunk> SurfaceNetTerrain::getChunk(int x, int y, int z) {
	return getChunk(glm::ivec3(x, y, z));
}

SurfaceNetTerrain::SurfaceNetTerrain(int seed) :
	perlin(seed, 6, 0.7, 2.0, 0.002) {
}

std::shared_ptr<Chunk> SurfaceNetTerrain::getChunk(glm::i32vec3 pos) {
	if (chunks.find(pos) == chunks.end()) {
		// Generate and/or load the chunk.
		// Loading from file not implemented yet, so it's generate for now
		generateChunk(pos.x, pos.y, pos.z);
	}
	return chunks[pos];
}

void SurfaceNetTerrain::generateChunk(int x, int y, int z) {
	// Generate the chunk data
	glm::vec3 pos1 = glm::vec3(chunkSize) * glm::vec3(x, y, z);
	glm::vec3 pos2 = pos1 + glm::vec3(chunkSize) * (float)(1.0 + 1.0 / chunkPrecision);
	//printf("Generating chunk from (%f, %f, %f) to (%f, %f, %f)\n", pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);

	Arr3D<double> data = Arr3D<double>(chunkPrecision + 2, chunkPrecision + 2, chunkPrecision + 2);
	for (int i = 0; i < chunkPrecision + 2; i++) {
		for (int j = 0; j < chunkPrecision + 2; j++) {
			for (int k = 0; k < chunkPrecision + 2; k++) {
				glm::vec3 pos = pos1 + glm::vec3(i, j, k) / (float)(chunkPrecision + 1) * (pos2 - pos1);

				double val = pos.y - 50 * perlin.generate(pos.x, pos.y, pos.z);

				data.set(i, j, k, val);
			}
		}
	}

	chunks[glm::ivec3(x, y, z)] = std::make_shared<Chunk>( x, y, z, data );
}

void SurfaceNetTerrain::draw(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& [pos, chunk] : chunks) {
		if (chunk.get() == nullptr) continue;
		chunk->draw(projMatrix, viewMatrix);
	}
}

void SurfaceNetTerrain::tick(double deltaTime) {
	for (auto& [pos, chunk] : chunks) {
		if (chunk.get() == nullptr) continue;
		chunk->tick(deltaTime);
	}
}
