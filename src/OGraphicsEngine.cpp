#include "OGraphicsEngine.hpp"

OGraphicsEngine::OGraphicsEngine()
:OWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
  glewInit();
  glViewport(0, 0, viewportWidth, viewportHeight);
  aspectRatio = viewportWidth / float(viewportHeight);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	camera = OCamera(
		/* eye */ glm::vec3(-1.8f,-0.7f,0.0f), 
		/* lookat */ glm::vec3(-2.0f,0.0f,0.0f), 
		/* upVector */ glm::vec3(0.0f,1.0f,0.0f)
	);
}

OGraphicsEngine::~OGraphicsEngine()
{
  // TODO
}

GLFWwindow* OGraphicsEngine::getWindow()
{
	return window;
}

void OGraphicsEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

void OGraphicsEngine::updateCamera()
{	
	double xPos, yPos;
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	glfwGetCursorPos(window, &xPos, &yPos);
	if (state != GLFW_PRESS)
	{
		lastMousePos.x = xPos;
		lastMousePos.y = yPos;
	}

	// Get the homogenous position of the camera and pivot point
	glm::vec4 position(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1);
	glm::vec4 pivot(camera.GetLookAt().x, camera.GetLookAt().y, camera.GetLookAt().z, 1);

	// step 1 : Calculate the amount of rotation given the mouse movement.
	float deltaAngleX = (2 * M_PI / viewportWidth); // a movement from left to right = 2*PI = 360 deg
	float deltaAngleY = (M_PI / viewportHeight);  // a movement from top to bottom = PI = 180 deg
	float xAngle = (lastMousePos.x - xPos) * deltaAngleX;
	float yAngle = (lastMousePos.y - yPos) * deltaAngleY;

	// Extra step to handle the problem when the camera direction is the same as the up vector
	float cosAngle = dot(camera.GetViewDir(), camera.GetUpVector());
	if (cosAngle * sgn(deltaAngleY) > 0.99f)
			deltaAngleY = 0;

	// step 2: Rotate the camera around the pivot point on the first axis.
	glm::mat4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, camera.GetUpVector());
	position = (rotationMatrixX * (position - pivot)) + pivot;

	// step 3: Rotate the camera around the pivot point on the second axis.
	glm::mat4x4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, camera.GetRightVector());
	glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

	// Update the camera view (we keep the same lookat and the same up vector)
	camera.SetCameraView(finalPosition, camera.GetLookAt(), camera.GetUpVector());

	// Update the mouse position for the next rotation
	lastMousePos.x = xPos; 
	lastMousePos.y = yPos;
}

void OGraphicsEngine::render(OGameObject& spaceship, spheres planetStore, OGameObject& sun, OGameObject& skybox, OGameObject& cloud)
{
  // init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // calculations
	updateCamera();

	float time = glfwGetTime();

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceship.dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceship.dir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
		spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
		spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
		-spaceship.dir.x,-spaceship.dir.y,-spaceship.dir.z,0,
		0.,0.,0.,1.,
		});

  // draw skybox
	drawObjectSkyBox(
    skybox,
    	glm::translate(glm::mat4(1.f), spaceship.pos) * 
      glm::scale(glm::mat4(1.f), glm::vec3(1.f))
	);
	glEnable(GL_DEPTH_TEST);

	// draw cloud
	drawObjectProc(
    cloud,
		cloud.getModelMatrix(time)
	);

	// draw objects
	drawObjectProc(
    spaceship,
			glm::translate(glm::mat4(1.f), spaceship.pos) * 
			specshipCameraRotrationMatrix * 
      glm::eulerAngleY(glm::pi<float>()) * 
      glm::scale(glm::mat4(1.f), glm::vec3(0.01))
	);

	for(auto planet:planetStore.planets)
		drawObjectProc(
			*planet,
			planet->getModelMatrix(time)
		);

	drawObjectProc(
    sun,
    sun.getModelMatrix(time)
	);

	glfwSwapBuffers(window);
}

void OGraphicsEngine::drawObjectColor(OGameObject& obj, glm::mat4 modelMatrix, glm::vec3 color)
{
  GLuint prog = obj.getShader().getProgram();
	glUseProgram(prog);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(prog, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(prog, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3f(glGetUniformLocation(prog, "color"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(prog, "lightPos"), 0,0,0);
	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);

}

void OGraphicsEngine::drawObjectProc(OGameObject& obj, glm::mat4 modelMatrix) {
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3f(glGetUniformLocation(program, "lightPos"), 0, 0, 0);
	OResourceUnit::SetActiveTexture(obj.texture, "colorTexture", program, 0);
	OResourceUnit::DrawContext(obj.getContext());

	glUseProgram(0);
}

void OGraphicsEngine::drawObjectSkyBox(OGameObject& obj, glm::mat4 modelMatrix) {
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	OResourceUnit::SetActiveCubeTexture(obj.texture, "skybox", program, 0);
	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);
}