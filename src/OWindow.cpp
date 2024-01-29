#include "OWindow.hpp"

void OWindow::shutdown(GLFWwindow* window)
{
  //shaderLoader.DeleteProgram(program);
}

OWindow::OWindow()
{
  // inicjalizacja glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


  // tworzenie okna za pomoca glfw
  window = glfwCreateWindow(500, 500, "Space Odysey", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    status = -1;
    return;
  }
  glfwMakeContextCurrent(window);
}

OWindow::~OWindow()
{
  shutdown(window);
  glfwTerminate();
}

void OWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OWindow::init(int height, int width)
{
  glewInit();
  glViewport(0, 0, width, height);
  aspectRatio = width / float(height);

  glfwSetFramebufferSizeCallback(window, OWindow::framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
  //program = shaderLoader.CreateProgram("shaders/shader_8_1.vert", "shaders/shader_8_1.frag");
  //programSun = shaderLoader.CreateProgram("shaders/shader_8_sun.vert", "shaders/shader_8_sun.frag");

  //loadModelToContext("./models/sphere.obj", sphereContext);
  //loadModelToContext("./models/spaceship.obj", shipContext);

}

void OWindow::renderLoop() 
{
  if(status) return;
  
  while (!glfwWindowShouldClose(window))
  {
    //processInput(window);

    //renderScene(window);
    glfwPollEvents();
  }
}