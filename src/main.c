#include "raylib.h"
#include <math.h>

#define GRAVITY 9.81f

typedef struct {
  int height, width;
} st_Window;

st_Window window = {
    .height = 450,
    .width = 800,
};

typedef struct {
  float x, y, radius;
  Color color;
  Vector2 speed;
} phys_Circle;

static void DrawCircleDebug(phys_Circle *circle);
static void DrawDebugText(float offset, phys_Circle *circle);
static void UpdateCircle(phys_Circle *circle);
static void WallCollision(phys_Circle *circle);
static void CircleCircleCollision(phys_Circle *c1, phys_Circle *c2);

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(window.width, window.height, "raylib [Physics]");
  SetTargetFPS(60);

  phys_Circle *circle1 = &(phys_Circle){
      .x = 200,
      .y = 300,
      .radius = 50.0f,
      .color = ORANGE,
      .speed = {.x = 5.0f, .y = 4.0f},
  };

  phys_Circle *circle2 = &(phys_Circle){
      .x = 600,
      .y = 100,
      .radius = 50.0f,
      .color = BLUE,
      .speed = {.x = 5.0f, .y = 4.0f},
  };

  while (!WindowShouldClose()) {
    // Update
    UpdateCircle(circle1);
    UpdateCircle(circle2);

    WallCollision(circle1);
    WallCollision(circle2);

    CircleCircleCollision(circle1, circle2);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    // cricle1's body, center, volocity dir, and radius
    DrawCircle(circle1->x, circle1->y, circle1->radius, circle1->color);
    DrawCircleDebug(circle1);

    // cricle2's body, center, volocity dir, and radius
    DrawCircle(circle2->x, circle2->y, circle2->radius, circle2->color);
    DrawCircleDebug(circle2);

    DrawDebugText(0.0f, circle1);
    DrawDebugText(14.0f, circle2);
    EndDrawing();
  }
  CloseWindow();
}

static void DrawCircleDebug(phys_Circle *circle) {
  DrawCircle(circle->x, circle->y, 5.0f, GREEN);
  DrawLine(circle->x, circle->y, circle->x + circle->radius, circle->y, GREEN);
  DrawLine(circle->x, circle->y, circle->x + circle->speed.x * 10.0f,
           circle->y + circle->speed.y * 10.0f, WHITE);
}

static void DrawDebugText(float offset, phys_Circle *circle) {
  int fontSize = 14;
  int padding = 5;

  DrawText(TextFormat("Position: x=%.1f y=%.1f", circle->x, circle->y),
           0 + padding, 0 + padding + offset, fontSize, GREEN);
}

static void UpdateCircle(phys_Circle *circle) {
  circle->x += circle->speed.x;
  circle->y += circle->speed.y;

  circle->speed.y += GRAVITY * GetFrameTime();
}

static void WallCollision(phys_Circle *circle) {
  if ((circle->x >= (GetScreenWidth() - circle->radius)) ||
      (circle->x <= circle->radius))
    circle->speed.x *= -1.0f;
  if ((circle->y >= (GetScreenHeight() - circle->radius)) ||
      (circle->y <= circle->radius))
    circle->speed.y *= -0.95f;
}

static void CircleCircleCollision(phys_Circle *c1, phys_Circle *c2) {
  float distX = c1->x - c2->x;
  float distY = c1->y - c2->y;
  float distance = sqrt((distX * distX) + (distY * distY));

  if (distance <= c1->radius + c2->radius && distance > 0) {
    float normalX = distX / distance;
    float normalY = distY / distance;

    // Separate the circles so they're no longer overlapping
    float overlap = (c1->radius + c2->radius) - distance;
    float separationX = normalX * overlap * 0.5f;
    float separationY = normalY * overlap * 0.5f;

    c1->x += separationX;
    c1->y += separationY;
    c2->x -= separationX;
    c2->y -= separationY;

    // Calculate relative velocity
    float relVelX = c1->speed.x - c2->speed.x;
    float relVelY = c1->speed.y - c2->speed.y;

    // Calculate velocity along the normal
    float velAlongNormal = relVelX * normalX + relVelY * normalY;

    // Don't resolve if circles are moving apart
    if (velAlongNormal > 0)
      return;

    // Assume equal masses - just reflect velocities along normal
    float restitution = 1.0f; // 1.0 = perfectly elastic, 0.8 = some energy loss

    // Calculate impulse
    float impulse = -(1.0f + restitution) * velAlongNormal / 2.0f;

    // Apply impulse along the normal
    float impulseX = impulse * normalX;
    float impulseY = impulse * normalY;

    c1->speed.x += impulseX;
    c1->speed.y += impulseY;
    c2->speed.x -= impulseX;
    c2->speed.y -= impulseY;
  }
}
