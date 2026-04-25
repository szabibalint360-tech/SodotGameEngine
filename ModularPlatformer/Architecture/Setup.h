#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>


namespace Engine {
    constexpr bool BUILD_DEBUG_TOOLS = true;
    constexpr bool CENTER_AREAS = true;
    constexpr bool CENTER_SPRITES = true;
}

using namespace std;

int windowWidth = 1280;
int windowHeight = 720;

constexpr Rectangle NULLREC = { 0.0f,0.0f,0.0f,0.0f };

Color COLLISIONBLUE = { 0, 128, 255, 66 };

//Helpers.h
#include <random>

namespace {
    static std::mt19937_64 _gen{ std::random_device{}() };
}

//Global Functions

// Returns 0.0f to 1.0f
inline float randf() {
    return std::uniform_real_distribution<float>(0.0f, 1.0f)(_gen);
}

// Returns a value between min and max (inclusive)
inline float randf_range(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(_gen);
}

// Returns an integer between min and max
inline int randi_range(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(_gen);
}

// Returns -1.0f or 1.0f
inline float randsign() {
    return (randf() < 0.5f) ? -1.0f : 1.0f;
}