#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

int windowWidth = 1280;
int windowHeight = 720;

constexpr Rectangle NULLREC = { 0.0f,0.0f,0.0f,0.0f };

inline Vector2 operator + (const Vector2& left, const Vector2& right) {
	return { left.x + right.x, left.y + right.y };
}

inline Vector2& operator += (Vector2& left, const Vector2& right) {
	left.x += right.x;
	left.y += right.y;
	return left;
}
Color COLLISIONBLUE = { 0, 128, 255, 66 };