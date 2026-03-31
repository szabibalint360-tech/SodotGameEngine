# C++ Raylib Node Engine

A lightweight, 2D game framework built on top of **Raylib**. This project implements a hierarchical "Node" system designed to manage game objects, physics, and rendering through inheritance and tree-based propagation.

> **Note on Architecture:** The mental model for this project is heavily inspired by the **Godot Game Engine**. It utilizes a Scene Tree where transformations and updates propagate from parent nodes to their children.



## 🚀 Features

* **Hierarchical Node System:** Manage complex object relationships with a parent-child tree.
* **Automated Lifecycle:** `update()` and `draw()` calls propagate automatically through the tree.
* **Component-Based Physics:** * `STATIC` and `KINEMATIC` physics bodies.
    * Custom AABB collision detection with shallowest-penetration resolution.
    * `moveAndSlide` implementation for smooth character movement.
* **Sprite Management:** Built-in support for static and animated sprites with frame-based logic.
* **Smart Camera:** A dedicated `CameraNode` that can easily track targets with built-in offsets and zoom.

## 🛠 Project Structure

The engine is built with modularity in mind, separating core logic from game-specific implementation:

| File | Description |
| :--- | :--- |
| `Node.h` | The base class for all objects; handles the tree structure and basic transforms. |
| `Bodies.h` | Contains `CollisionShape` and `Body` classes for physics and movement. |
| `Scene.h` | Manages the game world, cameras, and physics-aware children. |
| `Game.h` | Where you define your specific game logic (e.g., `Player`, `MainScene`). |
| `Setup.h` | Utility functions, operator overloads, and Raylib configurations. |

## 💻 Quick Start

### Creating a Custom Node
Just like in Godot, you can extend the `Node` or `Sprite` class to create custom behavior:

```cpp
class Player : public Body {
public:
    AnimatedSprite playerSprite;
    
    Player() : playerSprite("path/to/sprite.png", {32, 32}, 11, 2) {
        mode = KINEMATIC;
        addChild(&playerSprite); // Sprite follows Player transform
    }

    void process(double deltaTime) override {
        // Input handling
        velocity.x = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * speed * deltaTime;
        velocity.y = (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * speed * deltaTime;
    }
};
```

### Running the Scene
The `MainScene` acts as the root of your tree. Add your nodes to the scene, and the engine handles the rest.
<img width="1482" height="834" alt="Screenshot (83)" src="https://github.com/user-attachments/assets/f925e78f-c1a5-42d5-80f0-c9388e33fa4d" />

```cpp
int main() {
    InitWindow(1280, 720, "My Raylib Game");
    MainScene scene;

    while (!WindowShouldClose()) {
        scene.update(GetFrameTime());
        BeginDrawing();
            ClearBackground(RAYWHITE);
            scene.draw();
        EndDrawing();
    }
    CloseWindow();
}
```

## 🧠 Technical Highlights

### Collision Resolution
The engine uses a custom SAT-lite (Separating Axis Theorem) approach to determine collision normals. By calculating the **shallowest penetration** on the X or Y axis, the `moveAndSlide` function can resolve collisions without "stuttering" or clipping through corners.

$$overlap = (halfSize_A + halfSize_B) - |dist_{centers}|$$

### Memory Management
The `Node` destructor is designed to recursively delete all children, preventing memory leaks when a Scene or a complex object is removed from the heap.

## ⚙️ Requirements
* **C++17** or higher
* **Raylib 4.5+**
* A C++ Compiler (GCC/Clang or MSVC)
