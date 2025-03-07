#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <algorithm>
#include <vector>
#include <cstdlib>

using namespace std;

class Player
{
  public:
  float x, y;
  float width, height;
  int speed;

  void Draw()
  {
    DrawRectangle(x, y, width, height, WHITE);
  }

  void Update()
  {
    if(IsKeyDown(KEY_LEFT) && x >= 50)
    {
      x -= speed;
    }
    if(IsKeyDown(KEY_RIGHT) && x + width <= GetScreenWidth() - 50)
    {
      x += speed;
    }
  }

  
};

class Particle
{
  public:
  float x, y;
  float speed_x, speed_y;
  int radius;

  void Draw()
  {
    DrawCircle(x, y, radius, BLUE);
  }

  void Update (Player player) 
  {
    x += speed_x;
    y += speed_y;

    if(y + radius >= GetScreenHeight() || y - radius <= 0)
    {
      speed_y *= -1;
    }
    if(x + radius >= GetScreenWidth() || x - radius <= 0)
    {
      speed_x *= -1;
    }

    //Gravity Constant
    speed_y += 0.5;

    //Inertial Constant
    speed_x *= 0.992;

    //Speed Cap
    speed_x = Clamp(speed_x, -10, 10);
    speed_y = Clamp(speed_y, -10, 10);

    //Check Collision
    CheckPlayerCollide(player);

    KeepWithinBounds();
  }

  //Keeps circles bounded within window in a way that mimics being at rest
  void KeepWithinBounds()
  {
    if(x - radius < 0)
    {
      x = 0 + radius;
    } else if (x + radius > GetScreenWidth())
    {
      x = GetScreenWidth() - radius;
    }

    if(y - radius < 0)
    {
      y = 0 + radius;
    } else if(y + radius > GetScreenHeight())
    {
      y = GetScreenHeight() - radius;
    }
  }

  //check Collision with the player
  void CheckPlayerCollide(Player player)
  {
    if(CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{player.x, player.y, player.width, player.height}))
    {
      //X-Value collision based on current player input. Needs work
      if(IsKeyDown(KEY_LEFT))
      {
        speed_x -= player.speed;
      } else if(IsKeyDown(KEY_RIGHT))
      { 
        speed_x += player.speed;
      }

      //Normal Bounce
      speed_y *= -1;
    }
  }
};

Player player;

int main()
{
  const int screen_width = 1280;
  const int screen_height = 800;

  //Physics object array
  vector<Particle> objects;

  //Spawn particles, allowing for some variation in size, velocity, and starting position
  for(int i = 0; i < 50; i++) 
  {
    Particle tempBall;
    tempBall.x = rand() % screen_width;
    tempBall.y = 200 + rand() % screen_height;
    tempBall.speed_x = rand() % 5;
    tempBall.speed_y = rand() % 5;
    tempBall.radius = 5 + rand() % 15;

    objects.push_back(tempBall);
  }

  //Player setup
  player.height = 50;
  player.width = 50;
  player.x = screen_width / 2;
  player.y = screen_height - 50;
  player.speed = 5;

  InitWindow(screen_width, screen_height, "2D Physics");
  SetTargetFPS(60);

  //Game Loop
  while(WindowShouldClose() == false)
  {
    BeginDrawing();
    
    //Update Player first so that objects affected by it can have the latest values
    player.Update();

    //Iterate through objects to update position (CONCURRENCY TARGET)
    for(int i = 0; i < 50; i++)
    {
      objects.at(i).Update(player);
    }

    //Drawing
    ClearBackground(BLACK);
    player.Draw();

    //Iterate through objects for drawing (POSSIBLE CONCURRENCY TARGET)
    for(int i = 0; i < 50; i++)
    {
      objects.at(i).Draw();
    }

    EndDrawing();
    
  }

  CloseWindow();
  return 0;
}
