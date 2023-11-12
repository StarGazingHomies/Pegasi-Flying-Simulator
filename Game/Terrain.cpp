#include "Terrain.h"

Terrain::Terrain() {}

void Terrain::Load(const char* file) {

}

void Terrain::Generate(int x, int y, int w, int h, int rx, int ry,
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

int Terrain::getIndex(int x, int y) {
	return 5 * (x * (resY + 1) + y);
}

std::vector<float> Terrain::getCoord(int x, int y) {
	return std::vector<float>(vertices.begin() + getIndex(x, y), vertices.begin() + getIndex(x, y) + 3);
}
void Terrain::setCoord(int x, int y, std::vector<float> vals) {

}


void Terrain::Draw(const Shader& terrainShader, glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
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
}

void Chunk::draw(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	surfaceNet.draw(projMatrix, viewMatrix);
}

std::shared_ptr<Chunk> Terrain2::getChunk(int x, int y, int z) {
	return getChunk(glm::ivec3(x, y, z));
}

std::shared_ptr<Chunk> Terrain2::getChunk(glm::i32vec3 pos) {
	if (chunks.find(pos) == chunks.end()) {
		// Generate and/or load the chunk.
		// Loading from file not implemented yet, so it's generate for now
		generateChunk(pos.x, pos.y, pos.z);
	}
	return chunks[pos];
}

void Terrain2::generateChunk(int x, int y, int z) {
	// Generate the chunk data
	glm::vec3 pos1 = glm::vec3(chunkSize) * glm::vec3(x, y, z);
	glm::vec3 pos2 = pos1 + glm::vec3(chunkSize) * (float)(1.0 + 1.0 / chunkPrecision);
	printf("Generating chunk from (%f, %f, %f) to (%f, %f, %f)\n", pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);

	Arr3D<double> data = Arr3D<double>(chunkPrecision + 2, chunkPrecision + 2, chunkPrecision + 2);
	for (int i = 0; i < chunkPrecision + 2; i++) {
		for (int j = 0; j < chunkPrecision + 2; j++) {
			for (int k = 0; k < chunkPrecision + 2; k++) {
				glm::vec3 pos = pos1 + glm::vec3(i, j, k) / (float)(chunkPrecision + 1) * (pos2 - pos1);
				data.set(i, j, k, pos.y + 1);
			}
		}
	}

	chunks[glm::ivec3(x, y, z)] = std::make_shared<Chunk>( x, y, z, data );
}

void Terrain2::draw(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	for (auto& [pos, chunk] : chunks) {
		if (chunk.get() == nullptr) continue;
		chunk->draw(projMatrix, viewMatrix);
	}
}
