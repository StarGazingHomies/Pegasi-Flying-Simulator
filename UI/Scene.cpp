#include "Scene.h"

Scene::Scene(std::string name) {
	this->name = name;
}

void Scene::addObject(std::shared_ptr<UIObject> obj) {
	objects.push_back(obj);
}

void Scene::draw() {
	for (auto& obj : objects) {
		obj->draw();
	}
}

bool Scene::mouseEvent(double mouseX, double mouseY, int buttonStatus) {
	for (auto& obj : objects) {
		if (obj->mouseEvent(mouseX, mouseY, buttonStatus)) {
			objects.splice(objects.begin(), objects, std::find(objects.begin(), objects.end(), obj));
			return true;
		}
	}
	return false;
}

bool Scene::keyboardEvent(int key, int status) {
	return false;
}

bool Scene::textEvent(unsigned int c) {
	return false;
}
