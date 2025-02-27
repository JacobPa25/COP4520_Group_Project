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

typedef struct 
{
    Vector2 position;
    float radius;
    float alpha;
    float speed;
    Color color;
    
} Particle;


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    SetTargetFPS(60);

    // Player
    Player player = {{100, SCREEN_HEIGHT - 150, 50, 50}, {0, 0}, false};

    // Floor
    Rectangle floor = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};


    // Test Particle
    Color colors[14] = { ORANGE, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK,
        YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE };

    Particle circles[10] = { 0 };

    for (int i = 10 - 1; i >= 0; i--)
    {
        circles[i].alpha = 0.0f;
        circles[i].radius = 10;
        circles[i].position.x = (float)GetRandomValue((int)circles[i].radius, (int)(SCREEN_WIDTH - circles[i].radius));
        circles[i].position.y = (float)GetRandomValue((int)circles[i].radius, (int)((SCREEN_HEIGHT) - circles[i].radius));
        circles[i].speed = (float)GetRandomValue(1, 100)/2000.0f;
        circles[i].color = colors[GetRandomValue(0, 13)];
    }


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
        if (IsKeyPressed(KEY_SPACE) && !player.isJumping) {
            player.velocity.y = JUMP_STRENGTH;
            player.isJumping = true;
        }
        // Spawning Particles
        if (IsKeyPressed(KEY_P)) {
            for (int i = 10 - 1; i >= 0; i--)
            {
                circles[i].alpha = 0.0f;
                circles[i].radius = 10;
                circles[i].position.x = (float)GetRandomValue((int)circles[i].radius, (int)(SCREEN_WIDTH - circles[i].radius));
                circles[i].position.y = (float)GetRandomValue((int)circles[i].radius, (int)((SCREEN_HEIGHT) - circles[i].radius));
                circles[i].speed = (float)GetRandomValue(1, 100)/2000.0f;
                circles[i].color = colors[GetRandomValue(0, 13)];
            }
        }

        // Gravity
        player.velocity.y += GRAVITY;
        player.rect.y += player.velocity.y;
        for (int i = 10 - 1; i >= 0; i--)
        {
            circles[i].position.y += GRAVITY;

        }

        // Floor collision
        if (player.rect.y + player.rect.height >= floor.y) {
            player.rect.y = floor.y - player.rect.height;
            player.velocity.y = 0;
            player.isJumping = false;
        }
        for (int i = 10 - 1; i >= 0; i--){

        if (circles[i].position.y + (circles[i].radius) >= floor.y) {
            circles[i].position.y = floor.y - circles->radius;
        }
    }


        // for (int i = 10 - 1; (i >= 0); i--)
        // {
        //     circles[i].alpha += circles[i].speed;
        //     circles[i].radius += circles[i].speed*10.0f;

        //     if (circles[i].alpha > 1.0f) circles[i].speed *= -1;

        //     if (circles[i].alpha <= 0.0f)
        //     {
        //         circles[i].alpha = 0.0f;
        //         circles[i].radius = 20;
        //         circles[i].position.x = (float)GetRandomValue((int)circles[i].radius, (int)(SCREEN_WIDTH - circles[i].radius));
        //         circles[i].position.y = (float)GetRandomValue((int)circles[i].radius, (int)((SCREEN_HEIGHT - 100) - circles[i].radius));
        //         circles[i].color = colors[GetRandomValue(0, 13)];
        //         circles[i].speed = (float)GetRandomValue(1, 100)/2000.0f;
        //     }
        // }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = 10 - 1; i >= 0; i--)
        {
            DrawCircleV(circles[i].position, circles[i].radius, (circles[i].color));
        }

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
