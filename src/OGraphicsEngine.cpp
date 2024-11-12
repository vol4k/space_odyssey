#include "OGraphicsEngine.hpp"

OGraphicsEngine::OGraphicsEngine()
		: OWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
	glewInit();

	viewportWidth = DEFAULT_WINDOW_WIDTH;
	viewportHeight = DEFAULT_WINDOW_HEIGHT;

	glViewport(0, 0, viewportWidth, viewportHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	camera = OCamera(
			/* eye */ glm::vec3(-1.8f, -0.7f, 0.0f),
			/* lookat */ glm::vec3(-2.0f, 0.0f, 0.0f),
			/* upVector */ glm::vec3(0.0f, 1.0f, 0.0f));
}

GLFWwindow *OGraphicsEngine::getWindow()
{
	return window;
}

void OGraphicsEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height)
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
void OGraphicsEngine::updateCamera(const OGameObject &spaceship)
{
	float cameraDistance = 0.15f;
	glm::vec3 cameraPos = spaceship.pos - spaceship.dir * cameraDistance + spaceship.up * 0.025f;
	glm::vec3 lookAtPos = spaceship.pos + spaceship.dir * 0.025f;
	glm::vec3 upVector = spaceship.up;
	camera.SetCameraView(cameraPos, lookAtPos, upVector);
	camera.UpdateViewMatrix();
}

void OGraphicsEngine::render(OGameObject &spaceship, OGameObject &spaceshipEngine, spheres planetStore, aster asteroidStore, OGameObject &goal, OGameObject &sun, OGameObject &skybox)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateCamera(spaceship);

	float time = glfwGetTime();

	spaceship.side = glm::normalize(glm::cross(spaceship.dir, spaceship.up));
	spaceship.up = glm::normalize(glm::cross(spaceship.side, spaceship.dir));

	glm::mat4 spaceshipCameraRotationMatrix = glm::mat4(
			glm::vec4(spaceship.side, 0),
			glm::vec4(spaceship.up, 0),
			glm::vec4(-spaceship.dir, 0),
			glm::vec4(0, 0, 0, 1));

	spotlightPos = spaceship.pos + 0.2 * spaceship.dir;
	spotlightConeDir = spaceship.dir;

	// draw skybox
	glDisable(GL_DEPTH_TEST);
	drawObjectSkyBox(
			skybox,
			glm::translate(glm::mat4(1.f), spaceship.pos) *
					glm::eulerAngleY(time / 50) *
					glm::scale(glm::mat4(1.f), glm::vec3(1.f)),
			3.0);
	glEnable(GL_DEPTH_TEST);

	// draw objects
	drawObjectPBR(
			spaceship,
			glm::translate(glm::mat4(1.f), spaceship.pos) *
					spaceshipCameraRotationMatrix *
					glm::eulerAngleY(glm::pi<float>()) *
					glm::scale(glm::mat4(1.f), spaceship.scale),
			0.2, 1.0, 20.0);

	drawObjectPBR(
			spaceshipEngine,
			glm::translate(glm::mat4(1.f), spaceshipEngine.pos) *
					spaceshipCameraRotationMatrix *
					glm::eulerAngleY(glm::pi<float>()) *
					glm::scale(glm::mat4(1.f), glm::vec3(spaceshipEngine.scale)),
			0.5, 0.0, spaceshipEngine.exposition);

	for (auto planet : planetStore.planets)
		drawObjectPBR(
				*planet,
				planet->getModelMatrix(time),
				0.7, 0.0, 1.0);

	for (auto asteroid : asteroidStore.asteroids)
		drawObjectPBR(
				*asteroid,
				asteroid->getModelMatrix(time),
				0.7, 0.0, 3.0);

	drawObjectPBR(
			goal,
			goal.getModelMatrix(time),
			0.7, 0.0, 10);

	drawObjectPBR(
			sun,
			sun.getModelMatrix(time),
			0.5, 0.0, 10);

	glfwSwapBuffers(window);
}

void OGraphicsEngine::drawObjectPBR(OGameObject &obj, glm::mat4 modelMatrix, float roughness, float metallic, float exposition)
{
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);

	glm::vec3 sunDir = -glm::normalize(obj.currentPos);

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float *)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float *)&modelMatrix);

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

	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);
}

void OGraphicsEngine::drawObjectSkyBox(OGameObject &obj, glm::mat4 modelMatrix, float exposition)
{
	GLuint program = obj.getShader().getProgram();
	glUseProgram(program);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * camera.GetViewMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float *)&transformation);
	glUniform1f(glGetUniformLocation(program, "exposition"), exposition);
	OResourceUnit::SetActiveCubeTexture(obj.texture, "skybox", program, 0);
	OResourceUnit::DrawContext(obj.getContext());
	glUseProgram(0);
}