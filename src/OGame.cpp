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
  static bool mouseControl = false;

  GLFWwindow* window = engine->getWindow();

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceship->dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::vec3(0.f, 1.f, 0.f);
  glm::vec3 spaceshipPrevPos = spaceship->pos;
	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;

  spaceship->pos += spaceship->dir * acceleration;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		acceleration += moveSpeed/100;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		acceleration -= moveSpeed/100;
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	//	spaceship->pos += spaceshipSide * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	//	spaceship->pos -= spaceshipSide * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//	spaceship->pos += spaceshipUp * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//	spaceship->pos -= spaceshipUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		spaceship->dir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceship->dir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		spaceship->dir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceship->dir, 0));
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    if(!mouseControl) mouseControl = true;
    else mouseControl = false;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    if(!lockShaders)
      {
      lockShaders = true;
      reloadShaders();
      }
  else
    lockShaders = false;

  if(mouseControl)
    spaceship->dir = engine->camera.GetViewDir() + glm::vec3(0.f,0.2f,0.f);

  acceleration -= moveSpeed/5000;
  if (acceleration < 0) acceleration = 0;

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
      "spaceship.jpg",
      "spaceship_normal.jpg",
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
      "earth_normal.png",
      glm::vec3(-3.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.16f
    );
    planetStore.planet.venus = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "venus.jpg",
      "earth_normal.png",
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
      "earth_normal.png",
      glm::vec3(-12.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.32f
    );

    planetStore.planet.jupiter = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "jupiter.jpg",
      "earth_normal.png",
      glm::vec3(-14.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.8f
    );

    planetStore.planet.saturn = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "saturn.jpg",
      "earth_normal.png",
      glm::vec3(-16.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.48f
    );
    planetStore.planet.uranus = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "uranus.jpg",
      "earth_normal.png",
      glm::vec3(-18.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.48f
    );
    planetStore.planet.neptune = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "neptune.jpg",
      "earth_normal.png",
      glm::vec3(-20.f, -1.f, 0.f), 
      glm::vec3(1.f, 0.f, 0.f),
      rand()%50+25,
      0.4f
    );
    planetStore.planet.pluto = new OGameObject(
      sphereShader, 
      "sphere.obj",
      "pluto.jpg",
      "earth_normal.png",
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