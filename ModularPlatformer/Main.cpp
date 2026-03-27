#include "Game.h"

int main()
{

    InitWindow(windowWidth,windowHeight,"Platformer");
    SetTargetFPS(60);
    
    MainScene Main;

    while (!WindowShouldClose()) {
        Main.update(GetFrameTime());
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Main.draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

