#include "OGame.hpp"

int main(int argc, char** argv)
{
	OGame game = OGame(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	game.run();
	
	return EXIT_SUCCESS;
}