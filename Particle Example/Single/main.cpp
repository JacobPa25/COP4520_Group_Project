// Single threaded particle example
#include <raylib.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>

// Single particle position, velocity, radius and color
struct Particle {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

// Update particles position single-threaded
void UpdateParticles(std::vector<Particle>& particles, float screenWidth, float screenHeight, float deltaTime) {
    for (auto& p : particles) {
        // Position
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

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Single-threaded Physics Simulation");

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

    // Frame time logging
    std::vector<std::pair<float, float>> frameTimeLog;
    auto startLoggingTime = std::chrono::high_resolution_clock::now();
    const float loggingDuration = 30.0f; // 30 seconds

    while (!WindowShouldClose()) {
        // Measure frame time
        auto frameStartTime = std::chrono::high_resolution_clock::now();

        // Update physics
        float deltaTime = GetFrameTime();
        UpdateParticles(particles, screenWidth, screenHeight, deltaTime);

        // Measure update time
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        // Log frame time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startLoggingTime).count();
        frameTimeLog.push_back({elapsedTime, frameTime});

        // Draw particles
        BeginDrawing();
        ClearBackground(BLACK);

        for (const auto& p : particles) {
            DrawCircleV(p.position, p.radius, p.color);
        }

        DrawText(TextFormat("Particles: %d", particleCount), 10, 10, 20, WHITE);
        DrawText(TextFormat("Frame Time: %.2f ms", frameTime), 10, 40, 20, WHITE);

        EndDrawing();

        // Stop logging after 30 seconds
        if (elapsedTime >= loggingDuration) {
            break;
        }
    }

    // Save frame time log to CSV file
    std::ofstream outFile("particle_frametime_single.csv");
    outFile << "Time (s),Frame Time (ms)\n";
    for (const auto& entry : frameTimeLog) {
        outFile << entry.first << "," << entry.second << "\n";
    }
    outFile.close();

    CloseWindow();
    return 0;
}
