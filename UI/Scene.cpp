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

bool Scene::mouseEvent(MouseEvent mouseEvent) {
	for (auto& obj : objects) {
		if (obj->mouseEvent(mouseEvent)) {
			// Move to front
			objects.splice(objects.begin(), objects, std::find(objects.begin(), objects.end(), obj));
			return true;
		}
	}
	return false;
}

bool Scene::keyboardEvent(KeyEvent keyEvent) {
	for (auto& obj : objects) {
		if (obj->keyboardEvent(keyEvent)) {
			// Move to front
			objects.splice(objects.begin(), objects, std::find(objects.begin(), objects.end(), obj));
			return true;
		}
	}
	return false;
}

bool Scene::textEvent(unsigned int c) {
	for (auto& obj : objects) {
		if (obj->textEvent(c)) {
			// Move to front
			objects.splice(objects.begin(), objects, std::find(objects.begin(), objects.end(), obj));
			return true;
		}
	}
	return false;
}
