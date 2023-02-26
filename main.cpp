#include"Game.h"

int main(int argc, char* argv[]) {

	// Init stuff
	Game g;
	if (g.init() == -1) return -1;
	g.run();
	g.cleanup();
}