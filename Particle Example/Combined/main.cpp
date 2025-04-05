#include <raylib.h>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <atomic>

// Single particle position, velocity, radius and color
struct Particle {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

const int screenWidth = 800;
const int screenHeight = 600;
const int particleCount = 10000;

std::vector<Particle> particles;

// Mutex for logging
std::mutex logMutex;

// Stop flag for threads
std::atomic<bool> stopThreads(false);

std::atomic<float> deltaTime(0.016f);
std::vector<std::thread> threads;
int numThreads;
bool isMultithreaded = false;

// Initialize particles
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

// Update particles position single-threaded
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

// Update subset of particles
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

// Worker thread function
void WorkerThread(int start, int end) {
    while (!stopThreads) {
        UpdateParticlesChunk(start, end, deltaTime.load());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// Start and stopping of threads to handle simulation
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

    while (!WindowShouldClose()) {
        // Toggle single and mulit-threading with space
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

        BeginDrawing();
        ClearBackground(BLACK);

        for (const auto& p : particles) {
            DrawCircleV(p.position, p.radius, p.color);
        }

        DrawText(TextFormat("Mode: %s", isMultithreaded ? "Multi-threaded" : "Single-threaded"), 10, 10, 20, WHITE);
        DrawText(TextFormat("Particles: %d", particleCount), 10, 40, 20, WHITE);
        DrawText(TextFormat("Frame Time: %.2f ms", frameTime), 10, 70, 20, WHITE);
        DrawText("Press SPACE to toggle threading mode", 10, 100, 20, YELLOW);

        EndDrawing();
    }

    if (isMultithreaded) StopThreads();

    CloseWindow();
    return 0;
}