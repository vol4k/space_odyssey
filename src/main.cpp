#include "OWindow.hpp"

int main(int argc, char** argv)
{
	OWindow main_window = OWindow();
	main_window.init(500,500);
	main_window.renderLoop();
	return EXIT_SUCCESS;
}