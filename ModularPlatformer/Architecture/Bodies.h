#pragma once
#include "SpacialHashGrid.h"

enum PhysicsMode
{
	STATIC,KINEMATIC
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
	void moveAndSlide(SpatialGrid& grid);
	CollisionInfo checkCollision(vector<CollisionShape*>& candidades);
	bool isOnFloor() {
		return grounded;
	}
};
CollisionInfo Body::checkCollision(vector<CollisionShape*>& candidades) {
	Rectangle myRect = Hitbox.getRectangle(); // Assuming Body has a way to get its bounds

	for (auto* otherShape : candidades) {
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
void Body::moveAndSlide(SpatialGrid& grid) {

    grounded = false;

    // --- X AXIS ---
    position.x += velocity.x;
    // DELETE: Hitbox.position = position;
	
	// Query the grid for only nearby shapes based on NEW position
	auto& candidatesX = grid.getCandidates(&Hitbox);

    CollisionInfo hitX = checkCollision(candidatesX);
    if (hitX.happened) {
        Rectangle other = hitX.shape->getRectangle();
        Rectangle mine = Hitbox.getRectangle();
        float overlapX = (mine.width / 2 + other.width / 2) 
                       - fabsf((mine.x + mine.width/2) - (other.x + other.width/2));
        position.x += overlapX * hitX.normal.x;
        velocity.x = 0.0f;
    }

    // --- Y AXIS ---
    position.y += velocity.y;
    // DELETE: Hitbox.position = position;

	// Query again (position might have changed significantly)
	auto& candidatesY = grid.getCandidates(&Hitbox);

    CollisionInfo hitY = checkCollision(candidatesY);
    if (hitY.happened) {
        Rectangle other = hitY.shape->getRectangle();
        Rectangle mine = Hitbox.getRectangle();
        float overlapY = (mine.height / 2 + other.height / 2) 
                       - fabsf((mine.y + mine.height/2) - (other.y + other.height/2));
        position.y += overlapY * hitY.normal.y;
        if (hitY.normal.y < 0) grounded = true;
        velocity.y = 0.0f;
    }
}