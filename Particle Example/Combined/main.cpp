#include <raylib.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

struct FrameLogEntry {
    float time;
    float frameTime;
    bool isMultithreaded;
};

const int screenWidth = 800;
const int screenHeight = 600;
const int particleCount = 10000;
const float loggingDuration = 30.0f;

std::vector<Particle> particles;
std::vector<FrameLogEntry> frameTimeLog;

std::mutex logMutex;
std::atomic<bool> stopThreads(false);
std::atomic<float> deltaTime(0.016f);
std::vector<std::thread> threads;
int numThreads;
bool isMultithreaded = false;

void InitParticles() {
    particles.clear();
    particles.reserve(particleCount);

    for (int i = 0; i < particleCount; i++) {
        particles.push_back({
            {(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)},
            {(float)GetRandomValue(-200, 200) / 100.0f, (float)GetRandomValue(-200, 200) / 100.0f},
            (float)GetRandomValue(2, 5),
            {(unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), 255}
        });
    }
}

void UpdateParticlesSingle(float delta) {
    for (auto& p : particles) {
        p.position.x += p.velocity.x * delta;
        p.position.y += p.velocity.y * delta;

        if (p.position.x <= p.radius || p.position.x >= screenWidth - p.radius) {
            p.velocity.x *= -1;
        }
        if (p.position.y <= p.radius || p.position.y >= screenHeight - p.radius) {
            p.velocity.y *= -1;
        }
    }
}

void UpdateParticlesChunk(int start, int end, float delta) {
    for (int i = start; i < end; i++) {
        Particle& p = particles[i];
        p.position.x += p.velocity.x * delta;
        p.position.y += p.velocity.y * delta;

        if (p.position.x <= p.radius || p.position.x >= screenWidth - p.radius) {
            p.velocity.x *= -1;
        }
        if (p.position.y <= p.radius || p.position.y >= screenHeight - p.radius) {
            p.velocity.y *= -1;
        }
    }
}

void WorkerThread(int start, int end) {
    while (!stopThreads) {
        UpdateParticlesChunk(start, end, deltaTime.load());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void StartThreads() {
    numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    int chunkSize = particleCount / numThreads;
    threads.clear();

    for (int t = 0; t < numThreads; t++) {
        int start = t * chunkSize;
        int end = (t == numThreads - 1) ? particleCount : start + chunkSize;
        threads.emplace_back(WorkerThread, start, end);
    }
}

void StopThreads() {
    stopThreads = true;
    for (auto& thread : threads) {
        if (thread.joinable()) thread.join();
    }
    stopThreads = false;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Toggle Single/Multi-threaded Simulation");
    InitParticles();
    SetTargetFPS(0);

    auto startLoggingTime = std::chrono::high_resolution_clock::now();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            isMultithreaded = !isMultithreaded;

            if (isMultithreaded) {
                StartThreads();
            } else {
                StopThreads();
            }
        }

        auto frameStartTime = std::chrono::high_resolution_clock::now();
        float dt = GetFrameTime();
        deltaTime.store(dt);

        if (!isMultithreaded) {
            UpdateParticlesSingle(dt);
        }

        auto frameEndTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startLoggingTime).count();

        {
            std::lock_guard<std::mutex> lock(logMutex);
            frameTimeLog.push_back({elapsedTime, frameTime, isMultithreaded});
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (const auto& p : particles) {
            DrawCircleV(p.position, p.radius, p.color);
        }

        DrawText(TextFormat("Mode: %s", isMultithreaded ? "Multithreaded" : "Single-threaded"), 10, 10, 20, WHITE);
        DrawText(TextFormat("Particles: %d", particleCount), 10, 40, 20, WHITE);
        DrawText(TextFormat("Frame Time: %.2f ms", frameTime), 10, 70, 20, WHITE);
        DrawText("Press SPACE to toggle threading mode", 10, 100, 20, YELLOW);

        EndDrawing();

        if (elapsedTime >= loggingDuration) break;
    }

    if (isMultithreaded) StopThreads();

    // Save combined log
    std::ofstream outFile("particle_frametime_both.csv");
    outFile << "Time (s),Frame Time (ms),Mode\n";
    for (const auto& entry : frameTimeLog) {
        outFile << entry.time << "," << entry.frameTime << "," 
                << (entry.isMultithreaded ? "Multi" : "Single") << "\n";
    }
    outFile.close();

    CloseWindow();
    return 0;
}
