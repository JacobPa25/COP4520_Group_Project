#include <raylib.h>

struct Player {
    Rectangle rect;
    Vector2 speed;
    bool isGrounded;
};

struct Sign {
    Rectangle rect;
    bool isPlayerNearby;
};

int main() {
    // Initialize the screen
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Platformer with Interactive Sign");

    // Player setup
    Player player = {{100, 350, 50, 50}, {0, 0}, false};

    // Ground setup
    Rectangle ground = {0, 400, screenWidth, 50};

    // Sign setup
    Sign sign = {{300, 350, 30, 50}, false};

    // Physics constants
    const float gravity = 500.0f; // Pixels per second^2
    const float jumpForce = -300.0f; // Pixels per second

    SetTargetFPS(60);

    bool showControls = false;

    while (!WindowShouldClose()) {
        // Delta time
        float dt = GetFrameTime();

        // Handle player input only if not paused
        if (!showControls) {
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += 200 * dt;
            if (IsKeyDown(KEY_LEFT)) player.rect.x -= 200 * dt;

            if (IsKeyPressed(KEY_UP) && player.isGrounded) {
                player.speed.y = jumpForce;
                player.isGrounded = false;
            }

            // Apply gravity
            player.speed.y += gravity * dt;

            // Update player position
            player.rect.y += player.speed.y * dt;

            // Collision with the ground
            if (player.rect.y + player.rect.height >= ground.y) {
                player.rect.y = ground.y - player.rect.height;
                player.speed.y = 0;
                player.isGrounded = true;
            }

            // Check if player is near the sign
            sign.isPlayerNearby = CheckCollisionRecs(player.rect, sign.rect);
        }

        // Toggle controls display if near the sign and F is pressed
        if (sign.isPlayerNearby && IsKeyPressed(KEY_F)) {
            showControls = !showControls;
        }

        // Exit controls display with ESC
        if (showControls && IsKeyPressed(KEY_ESCAPE)) {
            showControls = false;
        }

        // Begin drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw ground
        DrawRectangleRec(ground, DARKGRAY);

        // Draw player
        DrawRectangleRec(player.rect, BLUE);

        // Draw sign
        DrawRectangle(screenWidth/4*1.6 - 60, 300, 120, 60, BROWN);
        DrawRectangleRec(sign.rect, BROWN);
        DrawText("Sign", sign.rect.x - 5, sign.rect.y - 30, 20, BLACK);

        // Display interaction hint
        if (sign.isPlayerNearby && !showControls) {
            DrawText("Press F to interact", player.rect.x - 50, player.rect.y - 30, 20, GRAY);
        }

        // Display controls if F was pressed
        if (showControls) {
            DrawRectangle(100, 100, 600, 200, LIGHTGRAY);
            DrawRectangleLines(100, 100, 600, 200, BLACK);
            DrawText("CONTROLS:", 120, 120, 20, BLACK);
            DrawText("- Arrow Keys to Move", 120, 150, 20, BLACK);
            DrawText("- Up arrow to Jump", 120, 180, 20, BLACK);
            DrawText("- F to Interact with Signs", 120, 210, 20, BLACK);
            DrawText("Press F to stop reading", 120, 240, 20, BLACK);
        }

        // End drawing
        EndDrawing();
    }

    CloseWindow();

    return 0;
}