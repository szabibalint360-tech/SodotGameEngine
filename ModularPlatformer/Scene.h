#pragma once
#include "Bodies.h"

class Scene :public Node {
public:
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
    virtual void draw() {
        for (Node* child : children) {
            child->draw();
        }
    }
    void addPhysicsChild(Body* child) {
        if (child == nullptr) return;
        if (child->mode == KINEMATIC) {
            physics_children.push_back(child);
        }
        physics_children_colshapes.push_back(&(child->Hitbox));//for static bodies we only want their collision shapes
    }
};