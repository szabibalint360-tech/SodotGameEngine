#pragma once
#include "Collison.h"

// world bounds
const int GRID_WIDTH = 200;  // 200 cells wide
const int GRID_HEIGHT = 150; // 150 cells high
const float CELL_SIZE = 128.0f; // Each cell is 128x128 pixels

class SpatialGrid {
public:
    // A 2D array of vectors
    // This is basically a "SRAM" for collisions.
    vector<CollisionShape*> cells[GRID_WIDTH][GRID_HEIGHT];

    void clear() {
        for (int x = 0; x < GRID_WIDTH; x++)
            for (int y = 0; y < GRID_HEIGHT; y++)
                cells[x][y].clear();
    }

    void insert(CollisionShape* shape) {
        Rectangle r = shape->getRectangle();

        // Convert world coordinates to grid indices
        int startX = (int)(r.x / CELL_SIZE);
        int startY = (int)(r.y / CELL_SIZE);
        int endX = (int)((r.x + r.width) / CELL_SIZE);
        int endY = (int)((r.y + r.height) / CELL_SIZE);

        // Clamp to grid bounds to prevent Access Violations
        startX = max(0, min(startX, GRID_WIDTH - 1));
        startY = max(0, min(startY, GRID_HEIGHT - 1));
        endX = max(0, min(endX, GRID_WIDTH - 1));
        endY = max(0, min(endY, GRID_HEIGHT - 1));

        for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                cells[x][y].push_back(shape);
            }
        }
    }

    // This is where the magic happens. 
    // We only return the list of guys in the SAME cell as our shape.
    vector<CollisionShape*>& getCandidates(CollisionShape* shape) {
        Rectangle r = shape->getRectangle();
        int x = (int)(r.x / CELL_SIZE);
        int y = (int)(r.y / CELL_SIZE);

        // Clamp
        x = max(0, min(x, GRID_WIDTH - 1));
        y = max(0, min(y, GRID_HEIGHT - 1));

        return cells[x][y];
    }
};