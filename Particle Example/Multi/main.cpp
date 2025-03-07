// Multi threaded rain example
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

// Mutex for logging
std::mutex logMutex;

// Stop flag for threads
std::atomic<bool> stopThreads(false);

// Update subset of particles
void UpdateParticles(std::vector<Particle>& particles, int start, int end, float screenWidth, float screenHeight, float deltaTime) {
    for (int i = start; i < end; i++) {
        Particle& p = particles[i];

        // Update position
        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;

        // Bounce off walls
        if (p.position.x <= p.radius || p.position.x >= screenWidth - p.radius) {
            p.velocity.x *= -1;
        }
        if (p.position.y <= p.radius || p.position.y >= screenHeight - p.radius) {
            p.velocity.y *= -1;
        }
    }
}

// Worker thread function
void WorkerThread(std::vector<Particle>& particles, int start, int end, float screenWidth, float screenHeight, std::atomic<float>& deltaTime) {
    while (!stopThreads) {
        UpdateParticles(particles, start, end, screenWidth, screenHeight, deltaTime.load());
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Prevent CPU overuse
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Multi-threaded Physics Simulation");

    // Create particles
    const int particleCount = 10000;
    std::vector<Particle> particles;
    particles.reserve(particleCount);

    for (int i = 0; i < particleCount; i++) {
        particles.push_back({
            {(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)},
            {(float)GetRandomValue(-200, 200) / 100.0f, (float)GetRandomValue(-200, 200) / 100.0f},
            (float)GetRandomValue(2, 5),
            {(unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), 255}
        });
    }

    SetTargetFPS(0);

    // Get number of threads available
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // Default = 4

    std::vector<std::thread> threads;
    std::atomic<float> deltaTime(0.016f);

    int chunkSize = particleCount / numThreads;

    // Start worker threads
    for (int t = 0; t < numThreads; t++) {
        int start = t * chunkSize;
        int end = (t == numThreads - 1) ? particleCount : start + chunkSize;
        threads.emplace_back(WorkerThread, std::ref(particles), start, end, screenWidth, screenHeight, std::ref(deltaTime));
    }

    // Frame time logging
    std::vector<std::pair<float, float>> frameTimeLog;
    auto startLoggingTime = std::chrono::high_resolution_clock::now();
    const float loggingDuration = 20.0f;

    while (!WindowShouldClose()) {
        // Measure fame time
        auto frameStartTime = std::chrono::high_resolution_clock::now();
        deltaTime.store(GetFrameTime());

        // Measure update time
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        // Log frame time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startLoggingTime).count();

        {
            std::lock_guard<std::mutex> lock(logMutex);
            frameTimeLog.push_back({elapsedTime, frameTime});
        }

        // Draw particles
        BeginDrawing();
        ClearBackground(BLACK);

        for (const auto& p : particles) {
            DrawCircleV(p.position, p.radius, p.color);
        }

        DrawText(TextFormat("Particles: %d", particleCount), 10, 10, 20, WHITE);
        DrawText(TextFormat("Threads: %d", numThreads), 10, 40, 20, WHITE);
        DrawText(TextFormat("Frame Time: %.2f ms", frameTime), 10, 70, 20, WHITE);

        EndDrawing();

        // Stop logging after 20 seconds
        if (elapsedTime >= loggingDuration) {
            break;
        }
    }

    // Stop worker threads
    stopThreads = true;
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Save frame time log to CSV file
    std::ofstream outFile("frametime_log_multi.csv");
    outFile << "Time (s),Frame Time (ms)\n";
    for (const auto& entry : frameTimeLog) {
        outFile << entry.first << "," << entry.second << "\n";
    }
    outFile.close();

    CloseWindow();
    return 0;
}
