#pragma once
#include "Node.h"

Vector2 getCollisionNormal(Rectangle first, Rectangle second) {
	// 1. Calculate distances between centers
	float dx = (first.x + first.width / 2) - (second.x + second.width / 2);
	float dy = (first.y + first.height / 2) - (second.y + second.height / 2);

	// 2. Calculate combined half-dimensions
	float combinedHalfW = (first.width + second.width) / 2.0f;
	float combinedHalfH = (first.height + second.height) / 2.0f;

	// 3. Check for overlap (The "Early Exit")
	float overlapX = combinedHalfW - fabsf(dx);
	float overlapY = combinedHalfH - fabsf(dy);

	// If either overlap is negative, they aren't touching at all
	if (overlapX <= 0 || overlapY <= 0) {
		return { 0.0f, 0.0f };
	}

	// 4. Determine the normal based on the SHALLOWEST penetration
	if (overlapX < overlapY) {
		// Collision is primarily horizontal
		return { (dx > 0) ? 1.0f : -1.0f, 0.0f };
	}
	else {
		// Collision is primarily vertical
		return { 0.0f, (dy > 0) ? 1.0f : -1.0f };
	}
}

class CollisionShape : public Node {
public:
	Vector2 dimention = { 32.0f, 32.0f }; // Absolute pixels, not multiplier
	bool centered = true;

	void setSize(float width, float height) {
		dimention = { width, height };
	}

	Rectangle getRectangle() {
		// Start with the parent's global position
		Vector2 global_pos = getGlobalPositon();
		;

		// If is centered, we need to shift the hitbox 
		// back so it aligns with the sprite's visuals
		if constexpr (Engine::CENTER_AREAS) {
			global_pos.x -= dimention.x / 2.0f;
			global_pos.y -= dimention.y / 2.0f;
		}

		return { global_pos.x, global_pos.y, dimention.x, dimention.y };
	}
	void draw() override {
		// If BUILD_DEBUG_TOOLS is false, the line below is literally deleted by the compiler.
		if constexpr (Engine::BUILD_DEBUG_TOOLS) {
			DrawRectangleRec(getRectangle(), Fade(SKYBLUE, 0.4f));
		}
	}
};
struct CollisionInfo {
	bool happened = false;
	Vector2 normal = { 0, 0 };
	CollisionShape* shape = nullptr;
};