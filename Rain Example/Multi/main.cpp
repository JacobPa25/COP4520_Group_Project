// Multi threaded rain example

#include <raylib.h>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <fstream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RAIN_COUNT 25000

// Single raindrop position and speed
struct Raindrop {
    Vector2 position;
    float speed;
};

std::vector<Raindrop> rain;
std::vector<Raindrop> rainRender;
std::mutex rainMutex;
bool running = true;

// Initialize all raindrops
void InitRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        rain.push_back({{(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)}, 300.0f + (rand() % 200)});
    }
}

// Multi-threading for updating raindrop positions
void UpdateRainPhysics() {
    while (running) {
        float dt = GetFrameTime();
        {
            std::lock_guard<std::mutex> lock(rainMutex);
            for (auto &drop : rain) {
                drop.position.y += drop.speed * dt;
                if (drop.position.y > SCREEN_HEIGHT) {
                    drop.position = {(float)(rand() % SCREEN_WIDTH), -10.0f};
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// Draw the raindrops thread-safe
void DrawRain() {
    std::lock_guard<std::mutex> lock(rainMutex);
    rainRender = rain;
    for (const auto &drop : rainRender) {
        DrawLineV(drop.position, {drop.position.x, drop.position.y + 10}, BLUE);
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Heavy Rain Simulation - Multi Thread");
    SetTargetFPS(0);
    InitRain();
    std::thread physicsThread(UpdateRainPhysics);

    std::ofstream fpsFile("rain_fps_multi.csv");
    fpsFile << "Time, FPS\n";

    double startTime = GetTime();

    // Main simulation loop
    while (!WindowShouldClose()) {
        double elapsedTime = GetTime() - startTime;
        if (elapsedTime >= 30.0) break;

        int currentFPS = GetFPS();
        fpsFile << elapsedTime << ", " << currentFPS << "\n";

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRain();
        DrawText("Heavy Rain Simulation (Multi Thread)", 10, 10, 20, WHITE);
        DrawText(TextFormat("Rain Particles: %d", RAIN_COUNT), 10, 40, 20, YELLOW);
        EndDrawing();
    }

    // Ensure the thread is safely stopped on exit
    running = false;
    physicsThread.join();
    fpsFile.close();
    CloseWindow();
    return 0;
}
