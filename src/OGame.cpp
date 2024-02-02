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
    std::cerr << "Error: " << e.what() << std::endl;
    is_ok = false;
    return;
  }
  initResources();
  is_ok = true;
}

OGame::~OGame()
{
  delete shader;
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
  GLFWwindow* window = engine->getWindow();

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceship->dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::vec3(0.f, 1.f, 0.f);
	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		spaceship->pos += spaceship->dir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		spaceship->pos -= spaceship->dir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		spaceship->pos += spaceshipSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		spaceship->pos -= spaceshipSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		spaceship->pos += spaceshipUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		spaceship->pos -= spaceshipUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		spaceship->dir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceship->dir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		spaceship->dir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceship->dir, 0));

	engine->cameraPos = spaceship->pos - 1.5 * spaceship->dir + glm::vec3(0, 1, 0) * 0.5f;
	engine->cameraDir = spaceship->dir;
}

void OGame::renderScene()
{
	engine->render(*spaceship);
}

void OGame::initResources()
{
  try
  {
    shader = new OShaderUnit("res/shaders/default.vert", "res/shaders/default.frag");
    spaceship = new OGameObject(shader, glm::vec3(-4.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
    OGraphicsEngine::loadModelToContext("res/models/spaceship.obj", spaceship->getContext());
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}