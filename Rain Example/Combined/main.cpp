// This is a combined example of both single and multi rain programs

#include <raylib.h>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>

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
bool multiThreaded = false;

// Smoothing counters variables
float smoothedFps = 0.0f;
float smoothedFrameTime = 0.0f;
const float alpha = 0.1f;

// Initialize all raindrops
void InitRain() {
    rain.clear();
    for (int i = 0; i < RAIN_COUNT; i++) {
        rain.push_back({{(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)}, 300.0f + (rand() % 200)});
    }
}

// Update raindrops single-threaded
void UpdateRainSingle(float dt) {
    for (auto &drop : rain) {
        drop.position.y += drop.speed * dt;
        if (drop.position.y > SCREEN_HEIGHT) {
            drop.position = {(float)(rand() % SCREEN_WIDTH), -10.0f};
        }
    }
}

// Update raindrops multi-threaded
void UpdateRainMulti() {
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
    if (multiThreaded) {
        std::lock_guard<std::mutex> lock(rainMutex);
        rainRender = rain;
    } else {
        rainRender = rain;
    }
    
    for (const auto &drop : rainRender) {
        DrawLineV(drop.position, {drop.position.x, drop.position.y + 10}, BLUE);
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Heavy Rain Simulation");
    SetTargetFPS(0);
    InitRain();
    std::thread physicsThread;

    // Main simulation loop
    while (!WindowShouldClose()) {
        // Toggle between single and multi-threaded mode using spacebar
        if (IsKeyPressed(KEY_SPACE)) {
            multiThreaded = !multiThreaded;
            InitRain();
            if (multiThreaded) {
                running = true;
                physicsThread = std::thread(UpdateRainMulti);
            } else {
                running = false;
                if (physicsThread.joinable()) physicsThread.join();
            }
        }

        float dt = GetFrameTime();
        int fps = GetFPS();

        // Smooth the counters so they are easier to read
        smoothedFps = alpha * fps + (1.0f - alpha) * smoothedFps;
        smoothedFrameTime = alpha * (dt * 1000.0f) + (1.0f - alpha) * smoothedFrameTime;

        if (!multiThreaded) {
            UpdateRainSingle(dt);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRain();
        DrawText(multiThreaded ? "Heavy Rain Simulation (Multi Thread)" : "Heavy Rain Simulation (Single Thread)", 10, 10, 20, WHITE);
        DrawText(TextFormat("Rain Particles: %d", RAIN_COUNT), 10, 40, 20, YELLOW);
        DrawText("Press SPACE to switch modes", 10, 70, 20, RED);
        
        DrawText(TextFormat("CURRENT FPS: %.1f", smoothedFps), GetScreenWidth() - 220, 40, 20, WHITE);

        EndDrawing();
    }
    
    // Ensure the thread is safely stopped on exit
    running = false;
    if (physicsThread.joinable()) physicsThread.join();
    CloseWindow();
    return 0;
}
