#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define SIN(x) sin(x * 3.141592653589/180) 
#define COS(x) cos(x * 3.141592653589/180)   

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

struct vector2 {


	float x, y;

	void Zero() {
		x = y = 0;
	}

	float operator*(const vector2& b)
	{
		return x* b.x + y * b.y;
	}

	vector2 operator+ (const vector2& b)
	{
		return { x + b.x, y + b.y };
	}

	vector2 operator- (const vector2& b)
	{
		return { x - b.x, y - b.y };
	}

	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	float LengthSqr() const
	{
		return x * x + y * y;
	}

	void Normalize()
	{
		const float Len = Length();
		x /= Len;
		y /= Len;
	}

	vector2 operator*(float b) const
	{
		return {
			x * b,
			y * b
		};
	}

};

struct Color
{
	int r;
	int g;
	int b;
	int a;

	void SetRed(int s_a) {
		r = 255;
		g = 0;
		b = 0;
		a = s_a;
	}

	void SetGreen(int s_a) {
		r = 0;
		g = 255;
		b = 0;
		a = s_a;
	}

	void SetGrey() {
		r = 255;
		g = 255;
		b = 255;
		a = 2;
	}

};


// Configuration -----------
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "2D Aimbot Verlet"