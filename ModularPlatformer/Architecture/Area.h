#pragma once
#include "Node.h"

class Area : public Node {
public:
    // Using a bitmask (1 byte = 8 bits/layers)
    Vector2 dimention = { 0.0f,0.0f };
    unsigned char collision_mask = 0b00000001;

    static constexpr bool is_centered = true;

    explicit Area(float dim_x,float dim_y) {
        name = "Area";
        setSize(dim_x, dim_y);
    }
    inline void setSize(float width, float height) {
        dimention = { width, height };
    }

    Rectangle getRect() const {
        Vector2 global_pos = getGlobalPositon();
        
        if constexpr (Engine::CENTER_AREAS) {
            global_pos.x -= dimention.x / 2.0f;
            global_pos.y -= dimention.y / 2.0f;
        }

        return { global_pos.x, global_pos.y, dimention.x, dimention.y };
    }

    // Checks if a world-space point is inside this area
    bool containsPoint(Vector2 point) const {
        return CheckCollisionPointRec(point, getRect());
    }
    // Checks if the mouse is currently hovering this area
    //rework later 
    /*
    bool isMouseOver(Camera2D worldCamera) const {
        // Convert screen mouse position to world mouse position
        Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), worldCamera);
        return CheckCollisionPointRec(worldMouse, getRect());
    }*/
    // Checks if this area overlaps another Area node
    bool overlapsArea(const Area* other) const {
        if (!other) return false;

        // Bitwise AND: Only returns true if they share at least one collision layer
        bool layersMatch = ((this->collision_mask & other->collision_mask) != 0);

        if (layersMatch) {
            return CheckCollisionRecs(this->getRect(), other->getRect());
        }
        return false;
    }

    void draw() override {
        if (!visible) return;

        // If BUILD_DEBUG_TOOLS is false, the line below is literally deleted by the compiler...nice
        if constexpr (Engine::BUILD_DEBUG_TOOLS) {
            DrawRectangleRec(getRect(), Fade(GREEN,0.4f));
        }

        Node::draw(); // Always call base to draw children
    }
    //LITERARELY BIT BANGING 
    inline void addLayer(int index) {
        collision_mask |= (1 << index);
    }

    // Removes a layer from the mask (Bitwise AND with NOT)
    inline void removeLayer(int index) {
        collision_mask &= ~(1 << index);
    }

    // Toggles a layer (Bitwise XOR)
    inline void toggleLayer(int index) {
        collision_mask ^= (1 << index);
    }

    // Checks if a specific layer is active
    inline bool isLayerActive(int index) const {
        return (collision_mask & (1 << index)) != 0;
    }

    // Overwrites the entire mask (e.g., 0b10101010)
    inline void setFullMask(unsigned char new_mask) {
        collision_mask = new_mask;
    }

    // The logic: Does this area share ANY layer with another?
    inline bool canCollideWith(const Area* other) const {
        return (this->collision_mask & other->collision_mask) != 0;
    }
};