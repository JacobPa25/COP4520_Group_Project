#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRAVITY 0.5f
#define JUMP_STRENGTH -10.0f
#define SPEED 5.0f

typedef struct Player {
    Rectangle rect;
    Vector2 velocity;
    bool isJumping;
} Player;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    SetTargetFPS(60);

    // Player
    Player player = {{100, SCREEN_HEIGHT - 150, 50, 50}, {0, 0}, false};

    // Floor
    Rectangle floor = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};

    while (!WindowShouldClose()) {
        // Move left and right
        if (IsKeyDown(KEY_A)) player.rect.x -= SPEED;
        if (IsKeyDown(KEY_D)) player.rect.x += SPEED;

        // Jump
        if (IsKeyPressed(KEY_W) && !player.isJumping) {
            player.velocity.y = JUMP_STRENGTH;
            player.isJumping = true;
        }

        // Gravity
        player.velocity.y += GRAVITY;
        player.rect.y += player.velocity.y;

        // Floor collision
        if (player.rect.y + player.rect.height >= floor.y) {
            player.rect.y = floor.y - player.rect.height;
            player.velocity.y = 0;
            player.isJumping = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(floor, DARKGRAY);
        DrawRectangleRec(player.rect, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
