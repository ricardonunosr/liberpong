#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <filesystem>

#include <iostream>

#include "pong.h"
#include "paddel.h"
#include "ball.h"

// Measured with pixels
#define PADDEL_WIDTH 25
#define PADDEL_HEIGHT 150

// Game Objects
static Paddel leftPaddel{ idk::vec3(PADDEL_WIDTH, PADDEL_HEIGHT, 1.0f), idk::vec3(-350.0f, 1.0f, 1.0f) };
static Paddel rightPaddel{ idk::vec3(PADDEL_WIDTH, PADDEL_HEIGHT, 1.0f), idk::vec3(350.0f, 1.0f, 1.0f) };
static Ball ball{ 0.0f, 0.0f, 0.0f, 3.5f, 260, {} };


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

// Input Event GLFW Callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Left Paddel
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		leftPaddel.MoveUp();
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		leftPaddel.Stop();
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		leftPaddel.MoveDown();
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		leftPaddel.Stop();

	//Right Paddel
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		rightPaddel.MoveUp();
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		rightPaddel.Stop();
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		rightPaddel.MoveDown();
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		rightPaddel.Stop();

	//Ball
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		ball.StartMovement();
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Pong::Width, Pong::Height, "LiberPong", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		printf("Failed to initialize OpenGL context\n");
		return -1;
	}

	// Successfully loaded OpenGL
	printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	leftPaddel.Init();
	rightPaddel.Init();
	ball.Init();

	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	float deltaTime, lastFrame = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Update
		leftPaddel.Update(deltaTime);
		rightPaddel.Update(deltaTime);
		ball.Update(deltaTime);
		//ball.CollidedWithPad(leftPaddel);
		ball.CollidedWithPad(rightPaddel);
		ball.CollidedWithTerrain();

		//Render
		leftPaddel.Draw();
		rightPaddel.Draw();
		ball.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}