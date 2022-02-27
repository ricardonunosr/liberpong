#ifndef PONG_H
#define PONG_H

#include "paddel.h"
#include "ball.h"

#define WIDTH 800
#define HEIGHT 600

// Measured with pixels
#define PADDEL_WIDTH 25
#define PADDEL_HEIGHT 150

static const unsigned int Width = WIDTH;
static const unsigned int Height = HEIGHT;

static Paddel _leftPaddel;
static Paddel _rightPaddel;
static Ball _ball;

static void ResetGame()
{
	_leftPaddel = {{PADDEL_WIDTH, PADDEL_HEIGHT, 1.0f}, {-350.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};
	_rightPaddel = {{PADDEL_WIDTH, PADDEL_HEIGHT, 1.0f}, {350.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}};
	_ball = {0.0f, 0.0f, 0.0f, 3.5f, 260};

	_leftPaddel.Init();
	_rightPaddel.Init();
	_ball.Init();
};

#endif // PONG_H