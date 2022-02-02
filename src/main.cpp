#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "pong.h"
#include "paddel.h"
#include "ball.h"

void GLAPIENTRY
MessageCallback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *message,
				const void *userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
}

// Input Event GLFW Callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	//Left Paddel
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		_leftPaddel.MoveUp();
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		_leftPaddel.Stop();
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		_leftPaddel.MoveDown();
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		_leftPaddel.Stop();

	//Right Paddel
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		_rightPaddel.MoveUp();
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		_rightPaddel.Stop();
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		_rightPaddel.MoveDown();
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		_rightPaddel.Stop();

	//Ball
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		_ball.StartMovement();
}

int main(void)
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Width, Height, "LiberPong", NULL, NULL);
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

	ResetGame();

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
		_leftPaddel.Update(deltaTime);
		_rightPaddel.Update(deltaTime);
		_ball.Update(deltaTime);
		_ball.CollidedWithPad(_leftPaddel);
		_ball.CollidedWithPad(_rightPaddel);
		const COLLIDED collided = _ball.CollidedWithTerrain();
		if (collided == COLLIDED::LEFT || collided == COLLIDED::RIGHT)
			ResetGame();

		//Render
		_leftPaddel.Draw();
		_rightPaddel.Draw();
		_ball.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}