#include "OGame.hpp"
#include "OGraphicsEngine.hpp"

OGame::OGame(int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT)
{
  try
  {
    engine = new OGraphicsEngine();
  }
  catch (std::runtime_error e)
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
  delete defaultShader;
  delete engine;
  delete goal;

  for (auto planet : planetStore.planets)
    delete planet;

  for (auto asteroid : asteroidStore.asteroids)
    delete asteroid;
}

int OGame::run()
{
  if (!is_ok)
    return EXIT_FAILURE;

  while (!((OWindow *)engine)->windowShouldClose())
  {
    processInput();
    collisionTest();

    renderScene();
    glfwPollEvents();
  }

  return EXIT_SUCCESS;
}

void OGame::collisionTest()
{
  if (!goalCounter)
    gameWin();
  if (spaceship->collisionTest(*sun))
    gameOver();
  for (auto &asteroid : asteroidStore.asteroids)
    if (spaceship->collisionTest(*asteroid))
      gameOver();
  for (auto &planet : planetStore.planets)
    if (spaceship->collisionTest(*planet))
      gameOver();
  if (spaceship->collisionTest(*goal))
  {
    goalCounter -= 1;
    std::cout << "Gold collected! " << goalCounter << " left" << std::endl;
    goal->shift = rand() % 1000;
  }
}

void OGame::processInput()
{
  static bool lockShaders = false;

  GLFWwindow *window = engine->getWindow();

  glm::vec3 spaceshipPrevPos = spaceship->pos;
  float angleSpeed = 0.05f;
  float moveSpeed = 0.05f;
  const float accelerationLimit = 0.03f;
  const float spineAccelerationLimit = 1.f;
  float pitchAngle = glm::radians(5.f);
  float rollAngle = glm::radians(5.f);
  float nozzleOffset = -0.055f;
  float engineExpositionConst = 100.f;

  glm::quat yawQuat = glm::angleAxis(accelerationXYZ.x * rollAngle, spaceship->up);
  glm::quat pitchQuat = glm::angleAxis(accelerationXYZ.y * pitchAngle, spaceship->side);
  glm::quat rollQuat = glm::angleAxis(accelerationXYZ.z * rollAngle, spaceship->dir);

  spaceship->updatePos(spaceship->pos + spaceship->dir * acceleration);
  spaceship->updateDir(glm::normalize(yawQuat * pitchQuat * spaceship->dir));
  spaceship->updateUp(glm::normalize(yawQuat * pitchQuat * rollQuat * spaceship->up));
  
  spaceshipEngine->updatePos(spaceship->pos + spaceship->dir * nozzleOffset);
  spaceshipEngine->updateDir(spaceship->dir);
  spaceshipEngine->updateUp(spaceship->up);
  spaceshipEngine->exposition = acceleration * engineExpositionConst;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    acceleration += moveSpeed / 100;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    accelerationXYZ.y -= moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    accelerationXYZ.y += moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    accelerationXYZ.x += moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    accelerationXYZ.x -= moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    accelerationXYZ.z -= moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    accelerationXYZ.z += moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    if (!lockShaders)
    {
      lockShaders = true;
      reloadShaders();
    }
    else
      lockShaders = false;

  if (acceleration > accelerationLimit)
    acceleration = accelerationLimit;

  acceleration -= moveSpeed / 500;
  if (acceleration < 0.001)
    acceleration = 0.001;

  accelerationXYZ -= accelerationXYZ * moveSpeed;
  if (accelerationXYZ.x > spineAccelerationLimit)
    accelerationXYZ.x = spineAccelerationLimit;
  if (accelerationXYZ.y > spineAccelerationLimit)
    accelerationXYZ.y = spineAccelerationLimit;
  if (accelerationXYZ.z > spineAccelerationLimit)
    accelerationXYZ.z = spineAccelerationLimit;

  engine->fov = 45.f + (acceleration / accelerationLimit) / 1.5;

  glm::vec3 deltaPos = spaceship->pos - spaceshipPrevPos;
  engine->camera.SetCameraView(engine->camera.GetEye() + deltaPos, spaceship->pos, engine->camera.GetUpVector());
}

void OGame::renderScene()
{
  engine->render(*spaceship, *spaceshipEngine, planetStore, asteroidStore, *goal, *sun, *skybox);
}

void OGame::reloadShaders()
{
  delete sunShader;
  delete skyboxShader;
  delete defaultShader;
  delete spaceshipShader;

  sunShader = new OShaderUnit("sun.vert", "sun.frag");
  skyboxShader = new OShaderUnit("skybox.vert", "skybox.frag");
  defaultShader = new OShaderUnit("default.vert", "default.frag");
  spaceshipShader = new OShaderUnit("default.vert", "default.frag");

  sun->updateShader(sunShader);
  skybox->updateShader(skyboxShader);
  spaceship->updateShader(spaceshipShader);
  spaceshipEngine->updateShader(sunShader);
  goal->updateShader(defaultShader);

  for (auto planet : planetStore.planets)
    planet->updateShader(defaultShader);

  for (auto asteroid : asteroidStore.asteroids)
    asteroid->updateShader(defaultShader);
}

void OGame::initResources()
{
  const int SHIFT_RANGE = 10000;

  std::cout << "Initialization: resources\n";
  try
  {
    // Load shaders once and reuse
    std::cout << "Load shaders\n";
    sunShader = new OShaderUnit("sun.vert", "sun.frag");
    skyboxShader = new OShaderUnit("skybox.vert", "skybox.frag");
    defaultShader = new OShaderUnit("default.vert", "default.frag");
    spaceshipShader = new OShaderUnit("default.vert", "default.frag");

    // Load single objects with unique properties
    std::cout << "Load skybox\n";
    skybox = new OGameObject("skybox", skyboxShader, "cube.obj", "skybox", glm::vec3(0.f), glm::vec3(-1.f, 0.f, 0.f));

    std::cout << "Load spaceship\n";
    spaceship = new OGameObject("spaceship", defaultShader, "spaceship.obj", "spaceship.png", "spaceship_normal.png", glm::vec3(-2.f, -0.8f, 0.f), glm::vec3(-1.f, 0.f, 0.f), 1, 0.01f);
    spaceshipEngine = new OGameObject("spaceshipEngine", sunShader, "sphere.obj", "sun.jpg", "sun_normal.jpg", glm::vec3(-2.f, -0.8f, 0.f), glm::vec3(-1.f, 0.f, 0.f), 1, 0.005f, 1, true);

    std::cout << "Load sun\n";
    sun = new OGameObject("sun", sunShader, "sphere.obj", "sun.jpg", "sun_normal.jpg", glm::vec3(0.f), glm::vec3(1.f, 0.f, 0.f), 50.f, 1.5f, 1, true);

    // Planet data structure
    struct PlanetData
    {
      std::string name;
      std::string texture;
      std::string normal;
      glm::vec3 position;
      float scale;
    };

    std::cout << "Load planets\n";
    std::vector<PlanetData> planetsData = {
        {"mercury", "mercury.jpg", "mercury_normal.png", glm::vec3(-3.f, -1.f, 0.f), 0.16f},
        {"venus", "venus.jpg", "venus_normal.png", glm::vec3(-8.f, -1.f, 0.f), 0.32f},
        {"earth", "earth.png", "earth_normal.png", glm::vec3(-10.f, -1.f, 0.f), 0.4f},
        {"mars", "mars.jpg", "mars_normal.png", glm::vec3(-12.f, -1.f, 0.f), 0.32f},
        {"jupiter", "jupiter.jpg", "jupiter_normal.png", glm::vec3(-14.f, -1.f, 0.f), 0.8f},
        {"saturn", "saturn.jpg", "saturn_normal.png", glm::vec3(-16.f, -1.f, 0.f), 0.48f},
        {"uranus", "uranus.jpg", "uranus_normal.png", glm::vec3(-18.f, -1.f, 0.f), 0.48f},
        {"neptune", "neptune.jpg", "neptune_normal.png", glm::vec3(-20.f, -1.f, 0.f), 0.4f},
        {"pluto", "pluto.jpg", "pluto_normal.png", glm::vec3(-22.f, -1.f, 0.f), 0.08f}};

    for (size_t i = 0; i < planetsData.size(); i++)
    {
      const auto &planet = planetsData[i];
      planetStore.planets[i] = new OGameObject(
          planet.name,
          defaultShader,
          "sphere.obj",
          planet.texture,
          planet.normal,
          planet.position,
          glm::vec3(1.f, 0.f, 0.f),
          rand() % 150 + 25,
          planet.scale,
          rand() % SHIFT_RANGE),
      true;
    }

    // Load asteroids
    std::cout << "Load asteroids\n";
    for (auto &asteroid : asteroidStore.asteroids)
    {
      int textureNum = rand() % 6 + 1;
      asteroid = new OGameObject(
          "asteroid",
          defaultShader,
          "asteroid.obj",
          "Aster_Small_" + std::to_string(textureNum) + "_Color.png",
          "Aster_Small_" + std::to_string(textureNum) + "_NM.png",
          glm::vec3((rand() % 1000 - 500) / 50.f, (rand() % 1000 - 500) / 50.f, (rand() % 1000 - 500) / 50.f),
          glm::vec3(1.f, 0.f, 0.f),
          (rand() % 150 + 25) / 5.f,
          (rand() % 50 + 250) / 1000.f,
          rand() % SHIFT_RANGE);
    }

    // Load gold objects
    std::cout << "Load goal\n";

    goal = new OGameObject(
        "gold",
        sunShader,
        "asteroid.obj",
        "sun.jpg",
        "sun_normal.jpg",
        glm::vec3((rand() % 50 - 100) / 10.f,
                  (rand() % 5 - 10) / 10.f,
                  (rand() % 5 - 10) / 10.f),
        glm::vec3(1.f, 0.f, 0.f),
        150.f,
        0.25f,
        rand() % SHIFT_RANGE);

    sun = new OGameObject("sun", sunShader, "sphere.obj", "sun.jpg", "sun_normal.jpg", glm::vec3(0.f), glm::vec3(1.f, 0.f, 0.f), 50.f, 1.5f, 1, true);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
}

void OGame::gameOver()
{
  std::cout << "Game Over!" << std::endl;
  glfwSetWindowShouldClose(engine->getWindow(), true);
}

void OGame::gameWin()
{
  std::cout << "Game win!" << std::endl;
  glfwSetWindowShouldClose(engine->getWindow(), true);
}