#pragma once
#include "Bodies.h"

class CameraNode : public Node {
public:
    Camera2D camera = { 0 };
    Node* target = nullptr;
    Vector2 offset = { windowWidth / 2.0f, windowHeight / 2.0f };
    float zoom = 1.0f;
    float rotation = 0.0f;

    CameraNode() {
        camera.offset = offset;
        camera.zoom = zoom;
    }

    void setTarget(Node* node) {
        target = node;
    }

    void update(double deltaTime) override {
        if (target != nullptr) {
            camera.target = target->position;
        }
        camera.offset = offset;
        camera.zoom = zoom;
        camera.rotation = rotation;
        Node::update(deltaTime);
    }

    void beginDraw() { BeginMode2D(camera); }
    void endDraw() { EndMode2D(); }
};



class Scene :public Node {
public:
    CameraNode* camera = nullptr;
    vector <Node*> children;
    vector <Body*> physics_children;
    vector <CollisionShape*> physics_children_colshapes;

    virtual void process(double deltatime) {}//should be defined in a seperate class

    void addChild(Node* child) {
        if (child == nullptr) return;
        child->parent = this;
        children.push_back(child);

        Body* body = dynamic_cast<Body*>(child);
        if (body != nullptr) {
            addPhysicsChild(body);
        }
    }
    virtual void update(double deltatime) {
        process(deltatime);
        for (auto child : physics_children) {
            child->moveAndSlide(physics_children_colshapes);
        }
        for (Node* child : children) {
            child->update(deltatime);
        }
    }
    virtual void drawUI() {} // override this for HUD/labels

    virtual void draw() {

        if (camera != nullptr) camera->beginDraw();

        for (Node* child : children) {
            child->draw();
        }
		if (camera != nullptr) camera->endDraw();
        drawUI();
    }
    void addPhysicsChild(Body* child) {
        if (child == nullptr) return;
        if (child->mode == KINEMATIC) {
            physics_children.push_back(child);
        }
        physics_children_colshapes.push_back(&(child->Hitbox));//for static bodies we only want their collision shapes
    }
};