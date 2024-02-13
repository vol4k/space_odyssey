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
  delete sun;
  delete sunShader;
  delete skybox;
  delete skyboxShader;
  delete spaceship;
  delete spaceshipShader;
  delete sphereShader;
  delete engine;
  
  for(auto planet : planetStore.planets)
    delete planet;
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
  static bool cameraLock = false;

  GLFWwindow* window = engine->getWindow();

  glm::vec3 spaceshipPrevPos = spaceship->pos;
	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;
  const float accelerationLimit = 0.025f;
  float rollAngle = glm::radians(5.f);

  spaceship->pos += spaceship->dir * acceleration;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		acceleration += moveSpeed/100;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		acceleration -= moveSpeed/100;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    glm::quat rollQuat = glm::angleAxis(-rollAngle, spaceship->dir);
    spaceship->up = glm::normalize(rollQuat * spaceship->up);
  }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    glm::quat rollQuat = glm::angleAxis(rollAngle, spaceship->dir);
    spaceship->up = glm::normalize(rollQuat * spaceship->up);
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    if(!lockShaders)
      {
      lockShaders = true;
      reloadShaders();
      }
  else
    lockShaders = false;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    if(!cameraLock)
      cameraLock = true;
    else
      cameraLock = false;

  if(!cameraLock)
    spaceship->dir = engine->camera.GetViewDir() + glm::vec3(0.f,0.5f,0.f);

  if (acceleration > accelerationLimit)
    acceleration = accelerationLimit;

  acceleration -= moveSpeed/500;
  if (acceleration < 0.001) 
    acceleration = 0.001;

  engine->fov = 45.f + (acceleration / accelerationLimit) / 1.5;

  glm::vec3 deltaPos = spaceship->pos - spaceshipPrevPos;
  engine->camera.SetCameraView(engine->camera.GetEye() + deltaPos, spaceship->pos, engine->camera.GetUpVector());
}

void OGame::renderScene()
{
	engine->render(*spaceship, planetStore, *sun, *skybox);
}

void OGame::reloadShaders()
{
  delete sunShader;
  delete skyboxShader;
  delete sphereShader;
  delete spaceshipShader;

  sunShader = new OShaderUnit("sun.vert", "sun.frag");
  skyboxShader = new OShaderUnit("skybox.vert", "skybox.frag");
  sphereShader = new OShaderUnit("default.vert", "default.frag");
  spaceshipShader = new OShaderUnit("default.vert", "default.frag");

  sun->updateShader(sunShader);
  skybox->updateShader(skyboxShader);
  spaceship->updateShader(spaceshipShader);

  for(auto planet : planetStore.planets)
    planet->updateShader(sphereShader);    
}

void OGame::initResources()
{
  try
  {
    sunShader = new OShaderUnit("sun.vert", "sun.frag");
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
      "spaceship.png",
      "spaceship_normal.png",
      glm::vec3(-2.f, -0.8f, 0.f),
      glm::vec3(-1.f, 0.f, 0.f)
    );

    sun = new OGameObject(
      sunShader, 
      "sphere.obj",
      "sun.jpg",
      "sun_normal.jpg",
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(1.f, 0.f, 0.f),
      50.f,
      1.5f
    );

    // planets

    planetStore.planet.mercury = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "mercury.jpg",
      "mercury_normal.png",
      glm::vec3(-3.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.16f
    );
    planetStore.planet.venus = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "venus.jpg",
      "venus_normal.png",
      glm::vec3(-8.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.32f
    );
    planetStore.planet.earth = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "earth.png",
      "earth_normal.png",
      glm::vec3(-10.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.4f
    );
    planetStore.planet.mars = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "mars.jpg",
      "mars_normal.png",
      glm::vec3(-12.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.32f
    );

    planetStore.planet.jupiter = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "jupiter.jpg",
      "jupiter_normal.png",
      glm::vec3(-14.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.8f
    );

    planetStore.planet.saturn = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "saturn.jpg",
      "saturn_normal.png",
      glm::vec3(-16.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.48f
    );
    planetStore.planet.uranus = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "uranus.jpg",
      "uranus_normal.png",
      glm::vec3(-18.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.48f
    );
    planetStore.planet.neptune = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "neptune.jpg",
      "neptune_normal.png",
      glm::vec3(-20.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.4f
    );
    planetStore.planet.pluto = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "pluto.jpg",
      "pluto_normal.png",
      glm::vec3(-22.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.08f
    );
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}