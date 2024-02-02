#include "OGraphicsEngine.hpp"

OGraphicsEngine::OGraphicsEngine()
:OWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
  glewInit();
  glViewport(0, 0, width, height);
  aspectRatio = width / float(height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
}

OGraphicsEngine::~OGraphicsEngine()
{
  // TODO
}

GLFWwindow* OGraphicsEngine::getWindow()
{
	return window;
}

void OGraphicsEngine::render(OGameObject& spaceship)
{
  // init
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(spaceship.getShader().getProgram());
  
  // calculations
  glm::mat4 transformation;
	float time = glfwGetTime();

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceship.dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceship.dir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
		spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
		spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
		-spaceship.dir.x,-spaceship.dir.y,-spaceship.dir.z,0,
		0.,0.,0.,1.,
		});

  // draw objects
	drawObjectColor(
    spaceship,
    glm::translate(glm::mat4(1.f), 
      spaceship.pos) * 
      specshipCameraRotrationMatrix * 
      glm::eulerAngleY(glm::pi<float>()) * 
      glm::scale(glm::mat4(1.f), 
    glm::vec3(0.1)), 
    glm::vec3(0.3, 0.3, 0.5)
	);

	glUseProgram(0);
	glfwSwapBuffers(window);
}

void OGraphicsEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OGraphicsEngine::loadModelToContext(std::string path, ORenderUnit::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

glm::mat4 OGraphicsEngine::createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f,1.f,0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide,cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(glm::mat4(1.f), -cameraPos);

	return cameraMatrix;
}

glm::mat4 OGraphicsEngine::createPerspectiveMatrix()
{
	
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,aspectRatio,0.,0.,
		0.,0.,(f+n) / (n - f),2*f * n / (n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

void OGraphicsEngine::drawObjectColor(OGameObject& obj, glm::mat4 modelMatrix, glm::vec3 color)
{
  GLuint prog = obj.getShader().getProgram();
	glUseProgram(prog);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(prog, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(prog, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3f(glGetUniformLocation(prog, "color"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(prog, "lightPos"), 0,0,0);
	ORenderUnit::DrawContext(obj.getContext());
	glUseProgram(0);

}