#pragma once
#include "Architecture/Scene.h"

//user is encoureg to define layer names
enum Layers {
    PLAYER = 0,
    ENEMEY = 1,
    CHESTS = 2//...
};


class Player : public Body {
public:

    string dir = "Sprites/Characters/Pink Man/Idle (32x32).png";
    AnimatedSprite PlayerSprite = AnimatedSprite(dir, { 32.0f,32.0f },11,2);
    Area InteractBox = Area(128, 128);
    float speed = 300.0f;
    Player() {
        mode = KINEMATIC;
        PlayerSprite.size = { 3.0f,3.0f };
        addChild(&InteractBox);
        addChild(&PlayerSprite);
        Hitbox.setSize(96.0f, 96.0f);
        position = { randf_range(-windowWidth,windowWidth),randf_range(-windowHeight,windowHeight) };
    }
    void process(double deltatime) {

        /*
        velocity.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
        velocity.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
        velocity.x *= speed * deltatime;
        velocity.y *= speed * deltatime;
        */
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

	CameraNode* MyCamera = instantiate<CameraNode>();
    Player* MyPlayer = instantiate<Player>();

    Box* Box1 = instantiate<Box>();
    Box* Box2 = instantiate<Box>();
    //stress test

    MainScene() {//Setup

        MyCamera->setTarget(MyPlayer);
        camera = MyCamera;
        addChild(MyCamera);
        
        Debugger.setTarget(MyPlayer);
        MyPlayer->position = { 640.0f,360.0f };
        addChild(MyPlayer);//Player is child of Scene

        Box1->mode = KINEMATIC;
        Box2->mode = STATIC;
        Box1->position = { (float)windowWidth - 200.0f,(float)windowHeight / 2.0f };
        Box2->position = { 200.0f,400.0f};
        
        addChild(Box1);
        addChild(Box2);
        

        for (int i = 0; i < 1000;i++) {
            addChild(instantiate<Player>());
        }
	}
    void process(double deltatime) {
        Box2->position.x++;
        if (Box2->position.x > windowWidth) {
            Box2->position.x = 0.0f;
        }
    }
    ~MainScene(){
         for (auto c : children_) delete c;
    }
};