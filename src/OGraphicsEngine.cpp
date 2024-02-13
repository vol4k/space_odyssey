#include "OGraphicsEngine.hpp"

OGraphicsEngine::OGraphicsEngine()
:OWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
  glewInit();

	viewportWidth = DEFAULT_WINDOW_WIDTH;
	viewportHeight = DEFAULT_WINDOW_HEIGHT;

  glViewport(0, 0, viewportWidth, viewportHeight);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	camera = OCamera(
		/* eye */ glm::vec3(-1.8f,-0.7f,0.0f),
		/* lookat */ glm::vec3(-2.0f,0.0f,0.0f),
		/* upVector */ glm::vec3(0.0f,1.0f,0.0f)
	);
}

GLFWwindow* OGraphicsEngine::getWindow()
{
	return window;
}

void OGraphicsEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

glm::mat4 OGraphicsEngine::createPerspectiveMatrix()
{
	glfwGetWindowSize(window, &viewportWidth, &viewportHeight);
	glm::mat4 perspective = glm::perspectiveFov(fov, (float)viewportWidth, (float)viewportHeight, 0.1f, 1000.f);
	
	return perspective;
}

void OGraphicsEngine::updateCamera()
{	
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

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
	glfwSetCursorPos(window, viewportWidth/2, viewportHeight/2);
	lastMousePos.x = viewportWidth/2;
	lastMousePos.y = viewportHeight/2;
}

void OGraphicsEngine::render(OGameObject& spaceship, spheres planetStore, aster asteroidStore, goal goldStore, OGameObject& sun, OGameObject& skybox)
{
  // init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // calculations
	updateCamera();
	camera.UpdateViewMatrix();

	float time = glfwGetTime();

	spaceship.side = glm::normalize(glm::cross(spaceship.dir, spaceship.up));

	glm::mat4 spaceshipCameraRotationMatrix = glm::mat4(
    glm::vec4(spaceship.side, 0),
    glm::vec4(spaceship.up, 0),
    glm::vec4(-spaceship.dir, 0),
    glm::vec4(0, 0, 0, 1)
);

	spotlightPos = spaceship.pos + 0.2 * spaceship.dir;
	spotlightConeDir = spaceship.dir;

  // draw skybox
	glDisable(GL_DEPTH_TEST);
	drawObjectSkyBox(
    skybox,
    	glm::translate(glm::mat4(1.f), spaceship.pos) *
			glm::eulerAngleY(time / 50) * 
      glm::scale(glm::mat4(1.f), glm::vec3(1.f)),
			3.0
	);
	glEnable(GL_DEPTH_TEST);

	// draw objects
	drawObjectPBR(
    spaceship,
			glm::translate(glm::mat4(1.f), spaceship.pos) *
			spaceshipCameraRotationMatrix *
      glm::eulerAngleY(glm::pi<float>()) *
      glm::scale(glm::mat4(1.f), glm::vec3(0.01)),
			0.2, 1.0, 20.0
	);

	for(auto planet:planetStore.planets)
		drawObjectPBR(
			*planet,
			planet->getModelMatrix(time),
			0.7, 0.0, 1.0
		);

	for(auto asteroid:asteroidStore.asteroids)
		drawObjectPBR(
			*asteroid,
			asteroid->getModelMatrix(time),
			0.7, 0.0, 3.0
		);

	for(auto gold:goldStore.gold_arr)
	{
		if(gold->show)
			drawObjectPBR(
				*gold,
				gold->getModelMatrix(time),
				0.7, 0.0, 10.0
			);
	}

	drawObjectPBR(
    sun,
    sun.getModelMatrix(time),
		0.3, 0.0, 5.0
	);

	glfwSwapBuffers(window);
}

void OGraphicsEngine::drawObjectPBR(OGameObject& obj, glm::mat4 modelMatrix, float roughness, float metallic, float exposition) {
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	glUniform1f(glGetUniformLocation(program, "exposition"), exposition);

	glUniform1f(glGetUniformLocation(program, "roughness"), roughness);
	glUniform1f(glGetUniformLocation(program, "metallic"), metallic);

	OResourceUnit::SetActiveTexture(obj.texture, "diffuseTexture", program, 0);
	OResourceUnit::SetActiveTexture(obj.normal, "normalTexture", program, 1);

	glUniform3f(glGetUniformLocation(program, "cameraPos"), camera.GetEye().x, camera.GetEye().y, camera.GetEye().z);

	glUniform3f(glGetUniformLocation(program, "sunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform3f(glGetUniformLocation(program, "sunColor"), sunColor.x, sunColor.y, sunColor.z);

	glUniform3f(glGetUniformLocation(program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);

	glUniform3f(glGetUniformLocation(program, "spotlightConeDir"), spotlightConeDir.x, spotlightConeDir.y, spotlightConeDir.z);
	glUniform3f(glGetUniformLocation(program, "spotlightPos"), spotlightPos.x, spotlightPos.y, spotlightPos.z);
	glUniform3f(glGetUniformLocation(program, "spotlightColor"), spotlightColor.x, spotlightColor.y, spotlightColor.z);
	glUniform1f(glGetUniformLocation(program, "spotlightPhi"), spotlightPhi);

	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);
}

void OGraphicsEngine::drawObjectSkyBox(OGameObject& obj, glm::mat4 modelMatrix, float exposition) {
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform1f(glGetUniformLocation(program, "exposition"), exposition);
	OResourceUnit::SetActiveCubeTexture(obj.texture, "skybox", program, 0);
	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);
}