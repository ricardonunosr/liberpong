/*
	_        __           _  __     __                                          __  __
   (_)  ____/ /___  ____ ( )/ /_   / /______  ____ _      __   ____ ___  ____ _/ /_/ /_
  / /  / __  / __ \/ __ \|// __/  / //_/ __ \/ __ \ | /| / /  / __ `__ \/ __ `/ __/ __ \
 / /  / /_/ / /_/ / / / / / /_   / ,< / / / / /_/ / |/ |/ /  / / / / / / /_/ / /_/ / / /
/_/   \__,_/\____/_/ /_/  \__/  /_/|_/_/ /_/\____/|__/|__/  /_/ /_/ /_/\__,_/\__/_/ /_/

*/
#pragma once

#define IDK_PI 3.14159265
#include <math.h>

namespace idk {
	struct vec3 {
		// First Element
		union {
			float x, r;
		};

		// Second Element
		union {
			float y, g;
		};

		// Third Element
		union {
			float z, b;
		};

		vec3() :x(0), y(0), z(0) {}

		vec3(float value) :x(value), y(value), z(value) {}

		vec3(float xVal, float yVal, float zVal) :x(xVal), y(yVal), z(zVal) {}

		vec3(const vec3& vector) :x(vector.x), y(vector.y), z(vector.z) {}

		// NOTE(Ricardo): Maybe we can have negative rows but for now its only positive rows.
		float& operator[](unsigned int row)
		{
			float* address = (float*)this;
			return address[row];
		};

		vec3 operator*(float value) {
			vec3 result;
			result.x = x * value;
			result.y = y * value;
			result.z = z * value;

			return result;
		}

		vec3& operator+=(vec3& vector) {

			x += vector.x;
			y += vector.y;
			z += vector.z;

			return *this;
		}

		vec3 operator-(vec3& vector) {

			vec3 result;
			result.x = x - vector.x;
			result.y = y - vector.y;
			result.z = z - vector.z;

			return result;
		}

		vec3 operator/(float value) {

			vec3 result;
			result.x = x / value;
			result.y = y / value;
			result.z = z / value;

			return result;
		}

		//
		float SquareLength() {
			return (x * x) + (y * y);
		}
	};

	struct vec4 {
		// First Element
		union {
			float x, r;
		};

		// Second Element
		union {
			float y, g;
		};

		// Third Element
		union {
			float z, b;
		};

		// Fourth Element
		union {
			float w, a;
		};

		vec4() :x(0), y(0), z(0), w(0) {}

		vec4(float value) :x(value), y(value), z(value), w(value) {}

		vec4(float xVal, float yVal, float zVal, float wVal) :x(xVal), y(yVal), z(zVal), w(wVal) {}

		// NOTE(Ricardo): Maybe we can have negative rows but for now its only positive rows.
		float& operator[](unsigned int row)
		{
			float* address = (float*)this;
			return address[row];
		};
	};

	struct mat4 {
		vec4 columns[4];

		mat4()
		{
			columns[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			columns[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			columns[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
			columns[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		mat4(float value)
		{
			columns[0] = vec4(value, 0.0f, 0.0f, 0.0f);
			columns[1] = vec4(0.0f, value, 0.0f, 0.0f);
			columns[2] = vec4(0.0f, 0.0f, value, 0.0f);
			columns[3] = vec4(0.0f, 0.0f, 0.0f, value);
		}

		// NOTE(Ricardo): Maybe we can have negative columns but for now its only positive columns.
		vec4& operator[](unsigned int column)
		{
			return columns[column];
		};

	};

	// Perspective projection matrix
	inline mat4 perspective(float fovy, float aspectRatio, float nearPlan, float farPlane);

	// Orthographic projection matrix without near or far plan parameters
	inline mat4 ortho2D(float left, float right, float bottom, float top);

	// matrix scale
	inline mat4 scale(mat4& matrix, vec3& vector);

	// matrix translate
	inline mat4 translate(mat4& matrix, vec3& vector);

	//
	inline vec3 normalize(vec3& vector);

	/*
		  _____                 _                           _        _   _
		 |_   _|               | |                         | |      | | (_)
		   | |  _ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _| |_ _  ___  _ __
		   | | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` | __| |/ _ \| '_ \
		  _| |_| | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | |_| | (_) | | | |
		 |_____|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\__|_|\___/|_| |_|
						 | |
						 |_|
	*/

	inline mat4 perspective(float fovy, float aspectRatio, float nearPlan, float farPlane) {
		mat4 matrix = mat4();

		return matrix;
	}

	inline mat4 ortho2D(float left, float right, float bottom, float top)
	{
		mat4 mat = mat4();
		mat[0][0] = 2.0f / (right - left);
		mat[1][1] = 2.0f / (top - bottom);
		// near= -1.0f far= 1.0f
		mat[2][2] = -1.0f;
		mat[3][0] = -(right + left) / (right - left);
		mat[3][1] = -(top + bottom) / (top - bottom);
		return mat;
	}

	inline mat4 scale(mat4& matrix, vec3& vector)
	{
		mat4 mat = mat4();
		mat[0][0] = vector.x * matrix[0][0];
		mat[1][1] = vector.y * matrix[1][1];
		mat[2][2] = vector.z * matrix[2][2];
		return mat;
	}

	inline mat4 translate(mat4& matrix, vec3& vector)
	{
		mat4 mat = mat4();
		mat[0][0] = matrix[0][0];
		mat[1][1] = matrix[1][1];
		mat[2][2] = matrix[2][2];
		mat[3][0] = matrix[3][0] + vector.x;
		mat[3][1] = matrix[3][1] + vector.y;
		mat[3][2] = matrix[3][2] + vector.z;
		return mat;
	}

	inline vec3 normalize(vec3& vector)
	{
		return vector / sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}
}
