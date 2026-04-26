#pragma once
#include "Bodies.h"

class Control : public Node {
public:
    Control() { name = "Control"; }

    // Controls usually don't use the camera, so we use their local position
    virtual void draw() override {
        if (!visible) return;
        // Logic for buttons/labels goes here
        Node::draw();
    }

    bool isMouseOver() const {
        return CheckCollisionPointRec(GetMousePosition(), { position.x, position.y, size.x, size.y });
    }
};

class DebugOverlay : public Control {
public:
    // just add pointers to whatever you want to watch
    Body* watchedBody = nullptr;
    bool showFPS = true;

    void setTarget(Body* target) {
        watchedBody = target;
    }
    void draw() override {
        Control::draw();
        if (!visible) return;
        int y = 10;
        int fontSize = 18;
        int lineHeight = 22;
        Color col = LIME;

        if (showFPS) {
            DrawText(TextFormat("FPS: %d", GetFPS()), 10, y, fontSize, col);
            y += lineHeight;
        }

        if (watchedBody) {
            DrawText(TextFormat("pos:      %.1f  %.1f",
                watchedBody->position.x,
                watchedBody->position.y), 10, y, fontSize, col); y += lineHeight;

            DrawText(TextFormat("velocity: %.1f  %.1f",
                watchedBody->velocity.x,
                watchedBody->velocity.y), 10, y, fontSize, col); y += lineHeight;

            DrawText(TextFormat("grounded: %s",
                watchedBody->grounded ? "YES" : "NO"), 10, y, fontSize, col); y += lineHeight;

            DrawText(TextFormat("hitbox:   %.1f  %.1f",
                watchedBody->Hitbox.getRectangle().x,
                watchedBody->Hitbox.getRectangle().y), 10, y, fontSize, col); y += lineHeight;
        }
    }
};