#include "OGame.hpp"
#include "OGraphicsEngine.hpp"

OGame::OGame(int width=DEFAULT_WINDOW_WIDTH, int height=DEFAULT_WINDOW_HEIGHT)
{
  try
  {
    engine = new OGraphicsEngine();
  }
  catch(std::runtime_error e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    is_ok = false;
    return;
  }
  is_ok = true;
}

OGame::~OGame()
{
  delete engine;
}

int OGame::run()
{
  if(!is_ok)
    return EXIT_FAILURE;

  while (!((OWindow*)engine)->windowShouldClose())
  {
    processInput();

    renderScene();
    glfwPollEvents();
  }

  return EXIT_SUCCESS;
}

void OGame::processInput()
{
	// TODO
}

void OGame::renderScene()
{
	engine->render();
}