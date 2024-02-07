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
  delete skybox;
  delete skyboxShader;
  delete spaceship;
  delete spaceshipShader;
  delete sphere;
  delete sphereShader;
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
  static bool lockShaders = false;

  GLFWwindow* window = engine->getWindow();

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceship->dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::vec3(0.f, 1.f, 0.f);
  glm::vec3 spaceshipPrevPos = spaceship->pos;
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
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    if(!lockShaders)
      {
      lockShaders = true;
      reloadShaders();
      }
  else
    lockShaders = false;

  glm::vec3 deltaPos = spaceship->pos - spaceshipPrevPos;

  engine->camera.SetCameraView(engine->camera.GetEye() + deltaPos, spaceship->pos, engine->camera.GetUpVector());
	//engine->cameraDir = spaceship->dir;
}

void OGame::renderScene()
{
	engine->render(*spaceship, *sphere, *skybox);
}

void OGame::reloadShaders()
{
  delete skyboxShader;
  delete sphereShader;
  delete spaceshipShader;

  skyboxShader = new OShaderUnit("skybox.vert", "skybox.frag");
  sphereShader = new OShaderUnit("default.vert", "default.frag");
  spaceshipShader = new OShaderUnit("default.vert", "default.frag");

  sphere->updateShader(sphereShader);
  spaceship->updateShader(spaceshipShader);
}

void OGame::initResources()
{
  try
  {
    skyboxShader = new OShaderUnit("skybox.vert", "skybox.frag");
    sphereShader = new OShaderUnit("default.vert", "default.frag");
    spaceshipShader = new OShaderUnit("default.vert", "default.frag");

    skybox = new OGameObject(
      skyboxShader,
      "cube.obj",
      "skybox",
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(-1.f, 0.f, 0.f)
    );

    spaceship = new OGameObject(
      spaceshipShader,
      "spaceship.obj",
      "spaceship.jpg",
      "spaceship_normal.jpg",
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(-1.f, 0.f, 0.f)
    );

    sphere = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "earth.png",
      "earth_normal.png",
      glm::vec3(-4.f, 0.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f)
    );
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}