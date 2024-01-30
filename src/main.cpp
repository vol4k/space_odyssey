#include "OWindow.hpp"

int main(int argc, char** argv)
{
	const int width = 800;
	const int height = 600;

	OWindow main_window = OWindow(width, height);
	
	main_window.init();
	main_window.renderLoop();
	
	return EXIT_SUCCESS;
}