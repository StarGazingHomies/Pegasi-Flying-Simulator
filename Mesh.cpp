#include"Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	// Copy initialization inputs into the object
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	// Bind VAO
	VAO.Bind();
	// Create Vertex Buffer Object and Element Buffer Object
	VBOobj = VBO(vertices);
	EBOobj = EBO(indices);
	// Link the vertex part of the VBO to the VAO
	VAO.LinkAttrib(VBOobj, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	// Link the colors to the VAO
	VAO.LinkAttrib(VBOobj, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// Link the texture coordinates to the VAO
	VAO.LinkAttrib(VBOobj, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// Link the normal vector to the VAO
	VAO.LinkAttrib(VBOobj, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind the VAO, VBO and EBO so that they are not accidentally modified
	VAO.Unbind();
	VBOobj.Unbind();
	EBOobj.Unbind();
}

void Mesh::Draw
(
	Shader& shader,
	glm::mat4 projection,
	glm::mat4 view,
	glm::vec3 camPos,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	// Activate shader and bind VAO
	shader.Activate();
	VAO.Bind();

	// Keeps track of the number of diffuse and specular textures
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Loop through all textures and load them
	// Naming scheme: diffuse0, diffuse1, ... , specular0, specular1, ...
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	// Feed camera position to the shader
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camPos.x, camPos.y, camPos.z);
	// Feed camera position to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(projection * view));

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);


	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Push transformation matricies to shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw triangles
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
	// Delete the VAO
	VAO.Delete();
	// Delete the VBO
	VBOobj.Delete();
	// Delete the EBO
	EBOobj.Delete();
	// Iterate through all textures and delete them
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i].Delete();
	}
}