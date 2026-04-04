#pragma once
#include "Node.h"

class Sprite : public Node {
public:
	float rotation = 0.0f;
	Texture2D texture;
	Color color = WHITE;
	Vector2 resolution;
	bool centered = true;

	Sprite(string& texture_relative_path, Vector2 resolution) {
		this->resolution = resolution;
		texture = LoadTexture(texture_relative_path.c_str());
	}
	~Sprite() {
		UnloadTexture(texture);
	}
	Vector2 getOrigin() {
		if (centered) {
			return Vector2{ (resolution.x * size.x) / 2.0f, (resolution.y * size.y) / 2.0f };
		}
		return Vector2{ 0.0f, 0.0f };
	}
	virtual Rectangle getRecSource() {
		return { 0.0f,0.0f,resolution.x,resolution.y };
	}
	Rectangle getRecDestination() const {
		Vector2 worldPos = getGlobalPositon();
		return { worldPos.x, worldPos.y, resolution.x * size.x, resolution.y * size.y };
	}
	void draw() override {
		DrawTexturePro(texture, getRecSource(), getRecDestination(), getOrigin(), rotation, color);
	}
};

class AnimatedSprite : public Sprite {
public:
	int frame_current = 0;
	int frames;
	int animation_speed;//frames/second
	int clock = 0;
	AnimatedSprite(string& texture_absolute_path, Vector2 resolution, int frames, int animation_speed)
		:Sprite(texture_absolute_path, resolution)
	{
		this->frames = frames;
		this->animation_speed = animation_speed;
	}
	Rectangle getRecSource() {
		clock++;
		if (clock > animation_speed) {
			frame_current++;
			clock = 0;
		}
		if (frame_current > frames) frame_current = 0;

		return { resolution.x * frame_current,0.0f,resolution.x,resolution.y };
	}
};