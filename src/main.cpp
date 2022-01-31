#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <filesystem>

#include <iostream>

#include "paddel.h"
#include "ball.h"

#define WIDTH 800
#define HEIGHT 600

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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

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

	idk::vec4 color = idk::vec4();
	Paddel leftPaddel(0, 0, color);
	leftPaddel._model = idk::scale(leftPaddel._model, idk::vec3(0.2f, 1.0f, 1.0f));
	leftPaddel._model = idk::translate(leftPaddel._model, idk::vec3(-0.8f, 0.0f, 0.0f));

	Paddel rightPaddel(0, 0, color);
	rightPaddel._model = idk::scale(rightPaddel._model, idk::vec3(0.2f, 1.0f, 1.0f));
	rightPaddel._model = idk::translate(rightPaddel._model, idk::vec3(0.8f, 0.0f, 0.0f));

	Ball ball(0.0f, 0.0f, 0.0f, 1.0f, 6,color);
	ball._model = idk::scale(ball._model, idk::vec3(0.05f));


	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.8f, 0.5f, 0.2f, 1.0f);

		leftPaddel.Draw();
		rightPaddel.Draw();
		ball.Draw();
		/* ball.Update(); */

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
