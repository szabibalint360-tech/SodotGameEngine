#pragma once
#include "Node.h"

enum PhysicsMode
{
	STATIC,KINEMATIC
};

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
	Vector2 offset = { 0.0f, 0.0f };
	Vector2 size = { 32.0f, 32.0f }; // Absolute pixels, not multiplier
	bool centered = true;

	void setSize(float width, float height) {
		size = { width, height };
	}

	Rectangle getRectangle() {
		// 1. Start with the parent's global position
		Vector2 pos = parent->position;

		// 2. Add our local offset
		pos.x += offset.x;
		pos.y += offset.y;

		// 3. If the parent is centered, we need to shift the hitbox 
		// back so it aligns with the sprite's visuals
		if (centered) {
			pos.x -= size.x / 2.0f;
			pos.y -= size.y / 2.0f;
		}

		return { pos.x, pos.y, size.x, size.y };
	}
	void draw() {
		DrawRectangleRec(getRectangle(), Fade(SKYBLUE, 0.4f));
	}
};
struct CollisionInfo {
	bool happened = false;
	Vector2 normal = { 0, 0 };
	CollisionShape* shape = nullptr;
};

class Body :public Node {
public:
	PhysicsMode mode = STATIC;
	CollisionShape Hitbox;
	Vector2 velocity = { 0.0f,0.0f };
	bool grounded = false;
	Body() {
		collision = true;
		addChild(&Hitbox);
	}
	void moveAndSlide(vector<CollisionShape*>& worldShapes);
	CollisionInfo checkCollision(vector<CollisionShape*>& worldShapes);
	bool isOnFloor() {
		return grounded;
	}
};
CollisionInfo Body::checkCollision(vector<CollisionShape*>& worldShapes) {
	Rectangle myRect = Hitbox.getRectangle(); // Assuming Body has a way to get its bounds

	for (auto* otherShape : worldShapes) {
		if (otherShape == &Hitbox) continue; // Self collision
		Rectangle otherRect = otherShape->getRectangle();

		// Use your optimized logic here
		Vector2 normal = getCollisionNormal(myRect, otherRect);

		if (normal.x != 0 || normal.y != 0) {
			return { true, normal, otherShape };
		}
	}

	return { false, {0, 0}, nullptr }; // No collision found
}
void Body::moveAndSlide(vector<CollisionShape*>& worldShapes) {
	grounded = false; // Reset grounded status at start of move

	 // Ensure hitbox follows the body
	// --- X AXIS ---
	position.x += velocity.x;
	Hitbox.position = position;

	CollisionInfo hitX = checkCollision(worldShapes);
	if (hitX.happened) {
		Rectangle other = hitX.shape->getRectangle();
		Rectangle mine = Hitbox.getRectangle();

		// Calculate how much we overlapped on X
		float overlapX = (mine.width / 2 + other.width / 2) - fabsf((mine.x + mine.width / 2) - (other.x + other.width / 2));

		// Push back by exactly the overlap amount in the direction of the normal
		position.x += overlapX * hitX.normal.x;
		Hitbox.position.x = position.x;
		velocity.x = 0.0f;
	}

	// --- Y AXIS ---
	position.y += velocity.y;
	Hitbox.position = position;

	CollisionInfo hitY = checkCollision(worldShapes);
	if (hitY.happened) {
		Rectangle other = hitY.shape->getRectangle();
		Rectangle mine = Hitbox.getRectangle();

		float overlapY = (mine.height / 2 + other.height / 2) - fabsf((mine.y + mine.height / 2) - (other.y + other.height / 2));

		position.y += overlapY * hitY.normal.y;
		Hitbox.position.y = position.y;
		if (hitY.normal.y < 0) grounded = true; // Normal -1 means floor pushed us UP
		velocity.y = 0.0f;
	}
}