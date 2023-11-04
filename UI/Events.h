#pragma once

struct KeyEvent {
	int key;
	int scancode;
	int action;
	int mods;
};

struct MouseEvent {

	MouseEvent(double xPos, double yPos) {
		this->type = false; // Movement
		this->xPos = xPos;
		this->yPos = yPos;
		// Unused
		this->action = -1;
		this->button = -1;
		this->mods = -1;
	}

	MouseEvent(double xPos, double yPos, int button, int action, int mods) {
		this->type = true; // Button
		this->button = button;
		this->action = action;
		this->mods = mods;
		// Copy from last movement event
		this->xPos = xPos;
		this->yPos = yPos;
	}

	bool type; // True = button, False = movement
	double xPos;
	double yPos;
	int button;
	int action;
	int mods;
};