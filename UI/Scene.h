#pragma once

#include "UIObject.h"
#include <list>
#include <memory>

class Scene : virtual public UIObject {
public:
	std::string name;
	std::list<std::shared_ptr<UIObject>> objects;

	Scene(std::string name);

	void addObject(std::shared_ptr<UIObject> obj);

	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};

