#include"Phys.h"

Physics::Physics() {
}

void Physics::Draw(Shader& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	for (auto obj : objects) {
		obj->Draw(shader, projMatrix, viewMatrix);
	}
}

void Physics::Tick(double time) {
	// See if any collisions happened
	if (objects.size() >= 2) {
		for (int i = 1; i < objects.size(); i++) {

			for (int j = 0; j < i; j++) {
				if (!checkOBBCollision(*objects[i], *objects[j])) {
					continue;
				}

				printf("Object %d and %d are colliding!", j, i);

			}
		}
	}

	for (int i = 0; i < objects.size(); i++) {
		// Obj Motion
		objects[i]->move(time);

		// Debug print
		printf("Object %d\n", i);
		printf("%s", objects[i]->toString().c_str());

	}
}