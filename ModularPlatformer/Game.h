#pragma once
#include "Scene.h"

class Player : public Body {
public:
    string dir = "Sprites/Characters/Pink Man/Idle (32x32).png";
    AnimatedSprite PlayerSprite = AnimatedSprite(dir, { 32.0f,32.0f },11,2);
    float speed = 300.0f;
    Player() {
        mode = KINEMATIC;
        PlayerSprite.size = { 3.0f,3.0f };

        addChild(&PlayerSprite);
        Hitbox.setSize(96.0f, 96.0f);
    }
    void process(double deltatime) {
        velocity.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
        velocity.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
        velocity.x *= speed * deltatime;
        velocity.y *= speed * deltatime;
    }
};
class Box :public Body {
public:
    string dir = "Sprites/Items/Boxes/Box2/Idle.png";
    Sprite BoxSprite = Sprite(dir, { 28.0f,24.0f });
    Box() {
        BoxSprite.setSize(3.0f, 3.0f);
        Hitbox.setSize(96.0f, 96.0f);
        addChild(&BoxSprite);
    }
};
class MainScene:public Scene {
public:
	CameraNode MyCamera;
    Player MyPlayer;
    Box Box1;
    Box Box2;

    MainScene() {//Setup
        MyCamera.setTarget(&MyPlayer);
        addChild(&MyCamera);
        camera = &MyCamera;
        MyPlayer.position = { 640.0f,360.0f };
        Box1.mode = KINEMATIC;
        Box2.mode = STATIC;
        Box1.position = { (float)windowWidth - 200.0f,(float)windowHeight / 2.0f };
        Box2.position = { 200.0f,400.0f};
        addChild(&MyPlayer);//Player is child of Scene
        addChild(&Box1);
        addChild(&Box2);
	}
    void process(double deltatime) {
        Box2.position.x++;
        if (Box2.position.x > windowWidth) {
            Box2.position.x = 0.0f;
        }
    }
};