#include <raylib.h>
#include <vector>
#include <fstream>
#include <iostream>
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif



#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
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

    // Player
    Player player = {{100, SCREEN_HEIGHT - 150, 50, 50}, {0, 0}, false};

    // Floor
    Rectangle floor = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};

    Color colors[14] = { RAYWHITE, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK,
        YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE };

    Particle circles[20000] = { 0 };


    // for (int i = 10 - 1; i >= 0; i--)
    // {
    //     circles[i].alpha = 0.0f;
    //     circles[i].radius = 10;
    //     circles[i].position.x = (float)GetRandomValue((int)circles[i].radius, (int)(SCREEN_WIDTH - circles[i].radius));
    //     circles[i].position.y = (float)GetRandomValue((int)circles[i].radius, (int)((SCREEN_HEIGHT) - circles[i].radius));
    //     circles[i].speed = (float)GetRandomValue(1, 100)/2000.0f;
    //     circles[i].color = colors[GetRandomValue(0, 13)];
    // }

    // Stores frametime and FPS
    vector<pair<float,int>> frameData;

    double startTime = GetTime(); // initialize time
    double duration = 10.0; // duration to record FPS and frametime

    while (!WindowShouldClose()) {
        for (int i = 20000 - 1; i >= 0; i--)
        {
            circles[i].alpha = 0.0f;
            circles[i].radius = 10;
            circles[i].position.x = (float)GetRandomValue((int)circles[i].radius, (int)(SCREEN_WIDTH - circles[i].radius));
            circles[i].position.y = (float)GetRandomValue((int)circles[i].radius, (int)((SCREEN_HEIGHT) - circles[i].radius));
            circles[i].speed = (float)GetRandomValue(1, 100)/2000.0f;
            circles[i].color = colors[0];
        }
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
        for (int i = 10 - 1; i >= 0; i--)
        {
            circles[i].position.y += GRAVITY;

        }

        // Jump
        if (IsKeyPressed(KEY_W) && !player.isJumping) {
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
                        circles[i].color = colors[0];
                    }
                }

        // Gravity
        player.velocity.y += GRAVITY;
        player.rect.y += player.velocity.y;

        // Floor collision w
        if (player.rect.y + player.rect.height >= floor.y) {
            player.rect.y = floor.y - player.rect.height;
            player.velocity.y = 0;
            player.isJumping = false;
        }

        

        BeginDrawing();
        DrawText(TextFormat("CURRENT FPS: %i", fps), GetScreenWidth() - 220, 40, 20, GREEN);
        ClearBackground(RAYWHITE);
        for (int i = 10 - 1; i >= 0; i--)
        {
            DrawCircleV(circles[i].position, circles[i].radius, (circles[i].color));
        }
        

        ClearBackground(BLACK);
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
