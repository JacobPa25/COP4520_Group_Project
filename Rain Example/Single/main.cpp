// Single threaded rain example

#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <fstream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RAIN_COUNT 25000

struct Raindrop {
    Vector2 position;
    float speed;
};

std::vector<Raindrop> rain;

void InitRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        rain.push_back({{(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)}, 300.0f + (rand() % 200)});
    }
}

void UpdateRain(float dt) {
    for (auto &drop : rain) {
        drop.position.y += drop.speed * dt;
        if (drop.position.y > SCREEN_HEIGHT) {
            drop.position = {(float)(rand() % SCREEN_WIDTH), -10.0f};
        }
    }
}

void DrawRain() {
    for (const auto &drop : rain) {
        DrawLineV(drop.position, {drop.position.x, drop.position.y + 10}, BLUE);
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Heavy Rain Simulation - Single Thread");
    SetTargetFPS(0);
    InitRain();

    std::ofstream fpsFile("rain_fps_single.csv");
    fpsFile << "Time, FPS\n";

    double startTime = GetTime();
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        int currentFPS = GetFPS();
        double elapsedTime = GetTime() - startTime;

        fpsFile << elapsedTime << ", " << currentFPS << "\n";

        UpdateRain(dt);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRain();
        DrawText("Heavy Rain Simulation (Single Thread)", 10, 10, 20, WHITE);
        DrawText(TextFormat("Rain Particles: %d", RAIN_COUNT), 10, 40, 20, YELLOW);
        EndDrawing();

        if (elapsedTime >= 30.0) break;
    }

    fpsFile.close();
    CloseWindow();
    return 0;
}
