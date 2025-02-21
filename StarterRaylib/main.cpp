#include <raylib.h>
#include <vector>
#include <fstream>
#include <iostream>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRAVITY 0.5f
#define JUMP_STRENGTH -10.0f
#define SPEED 5.0f

using namespace std;

typedef struct Player {
    Rectangle rect;
    Vector2 velocity;
    bool isJumping;
} Player;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    SetTargetFPS(60);

    // Player
    Player player = {{100, SCREEN_HEIGHT - 150, 50, 50}, {0, 0}, false};

    // Floor
    Rectangle floor = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};

    // Stores frametime and FPS
    vector<pair<float,int>> frameData;

    double startTime = GetTime(); // initialize time
    double duration = 10.0; // duration to record FPS and frametime

    while (!WindowShouldClose()) {
        double currentTime = GetTime();

        // exits game loop when time duration for recording is passed
        if (currentTime - startTime >= duration) {
            break;
        }

        // Move left and right
        if (IsKeyDown(KEY_A)) player.rect.x -= SPEED;
        if (IsKeyDown(KEY_D)) player.rect.x += SPEED;

        float frameTime = GetFrameTime();
        int fps = GetFPS();

        // Doesnt log data if current time is under 1 second to allow FPS to stablize
        // else, time is logged
         if (currentTime - startTime > 1.0) {
            frameData.push_back({frameTime, fps});
        }

        // Jump
        if (IsKeyPressed(KEY_W) && !player.isJumping) {
            player.velocity.y = JUMP_STRENGTH;
            player.isJumping = true;
        }

        // Gravity
        player.velocity.y += GRAVITY;
        player.rect.y += player.velocity.y;

        // Floor collision
        if (player.rect.y + player.rect.height >= floor.y) {
            player.rect.y = floor.y - player.rect.height;
            player.velocity.y = 0;
            player.isJumping = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(floor, DARKGRAY);
        DrawRectangleRec(player.rect, BLUE);
        EndDrawing();
    }


    // open CSV file and save data to it
    ofstream file("fps_data.csv");
    if (file.is_open()){
        file << "FrameTime,FPS\n";
        for (const auto& entry : frameData) {
            file << entry.first << "," << entry.second << "\n";
        }
        file.close();
        cout << "FPS data saved to file\n";
    }
    else {
        cerr << "Failed to open file\n";
    }

    CloseWindow();
    return 0;
}
