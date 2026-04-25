#pragma once
#include "Area.h"
#include "Bodies.h"
#include "Sprite.h"
#include "UserInterface.h"

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
    vector <Body*> physics_children_;
    vector <CollisionShape*> physics_children_colshapes;
    vector <Control*> ui_children;// Elements that stay fixed on screen
    DebugOverlay Debugger;

    virtual void process(double deltaTime) {}//should be defined in a seperate class

    void addChild(Node* child) {
        if (child == nullptr) return;
        child->parent_ = this;
        if (auto control = dynamic_cast<Control*>(child)) {
            ui_children.push_back(control); return;
        }

        children_.push_back(child);//no Control Nodes

        if (auto body = dynamic_cast<Body*>(child)) {
            addPhysicsChild(body);
        }
    }
    virtual void update(double deltaTime) {
        process(deltaTime);
        for (auto child : physics_children_) {
            child->moveAndSlide(physics_children_colshapes);
        }
        for (Node* child : children_) {
            child->update(deltaTime);
        }
        updateUI(deltaTime);
    }
    virtual void updateUI(double deltaTime) {
        for (Control* child : ui_children) {
            child->update(deltaTime);
        }
    }
    virtual void drawUI() {
        Debugger.draw();
        for (Control* child : ui_children) {
            child->draw();
        }
    } // override this for HUD/labels

    virtual void draw() {

        if (camera != nullptr) camera->beginDraw();

        for (Node* child : children_) {
            child->draw();
        }
		if (camera != nullptr) camera->endDraw();
        drawUI();
    }
    void addPhysicsChild(Body* child) {
        if (child == nullptr) return;
        if (child->mode == KINEMATIC) {
            physics_children_.push_back(child);
        }
        physics_children_colshapes.push_back(&(child->Hitbox));//for static bodies we only want their collision shapes
    }
};