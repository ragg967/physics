#include "raylib.h"
#define CIRCLES_IMPLEMENTATION
#include "../includes/circles.h"
#include "../includes/utilities.h"

#include <stdlib.h>

typedef struct {
  int height, width;
} st_Window;

st_Window window = {
    .height = 450,
    .width = 800,
};

typedef struct {
  float x, y, height, width;
  Color color;
  Vector2 speed;
} st_Rectangle;

static st_Rectangle *initRectangle(float x, float y, float height, float width,
                                   Color color, Vector2 speed) {
  st_Rectangle *newRectangle = malloc(sizeof(st_Rectangle));
  if (!newRectangle)
    return NULL;
  *newRectangle = (st_Rectangle){
      .x = x,
      .y = y,
      .height = height,
      .width = width,
      .color = color,
      .speed = speed,
  };
  return newRectangle;
}

static void UpdateRectangle(st_Rectangle *rectangle);
static void RectangleWallCollision(st_Rectangle *rectangle);

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(window.width, window.height, "raylib [Physics]");
  SetTargetFPS(60);

  st_Circle *circle1 =
      initCircle(200.0f, 300.0f, 50.0f, ORANGE, (Vector2){5.0f, 4.0f});
  st_Circle *circle2 =
      initCircle(600.0f, 100.0f, 50.0f, BLUE, (Vector2){5.0f, 4.0f});

  st_Rectangle *rectangle1 =
      initRectangle(100.0F, 200.0F, 50.0F, 50.0F, RED, (Vector2){7.0F, 14.0F});
  st_Rectangle *rectangle2 =
      initRectangle(500.0F, 300.0F, 50.0F, 50.0F, WHITE, (Vector2){1.0F, 2.0F});

  while (!WindowShouldClose()) {
    // Update

    // circles
    UpdateCircle(circle1);
    UpdateCircle(circle2);
    CircleCircleCollision(circle1, circle2);

    // rectangles
    UpdateRectangle(rectangle1);
    UpdateRectangle(rectangle2);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    // cricle1's body, center, volocity dir, and radius
    DrawCircle(circle1->x, circle1->y, circle1->radius, circle1->color);
    DrawCircleDebug(circle1);

    // cricle2's body, center, volocity dir, and radius
    DrawCircle(circle2->x, circle2->y, circle2->radius, circle2->color);
    DrawCircleDebug(circle2);

    // rectangle1's body, center, and velocity dir
    DrawRectangle(rectangle1->x, rectangle1->y, rectangle1->width,
                  rectangle1->height, rectangle1->color);

    // rectangle2's body, center, and velocity dir
    DrawRectangle(rectangle2->x, rectangle2->y, rectangle2->width,
                  rectangle2->height, rectangle2->color);

    // debug text
    DrawCircleDebugText(0.0f, circle1);
    DrawCircleDebugText(14.0f, circle2);
    EndDrawing();
  }
  free(circle1);
  free(circle2);
  CloseWindow();
}

static void UpdateRectangle(st_Rectangle *rectangle) {
  rectangle->x += rectangle->speed.x;
  rectangle->y += rectangle->speed.y;

  rectangle->speed.y += GRAVITY * GetFrameTime();
  rectangle->speed.y -= AIR_RESISTANCE * GetFrameTime();
  rectangle->speed.x -= AIR_RESISTANCE * GetFrameTime();

  RectangleWallCollision(rectangle);

  if (fabsf(rectangle->speed.y) < 0.15f &&
      rectangle->y >= GetScreenHeight() - rectangle->height - 1.0f) {
    rectangle->speed.y = 0.0f; // Stop vertical movement when nearly at rest
  }
  if (fabsf(rectangle->speed.x) < 0.15f) {
    rectangle->speed.x = 0.0f; // Stop horizontal movement when nearly at rest
  }
}

static void RectangleWallCollision(st_Rectangle *rectangle) {
  // Right wall
  if (rectangle->x >= (GetScreenWidth() - rectangle->width)) {
    rectangle->x = GetScreenWidth() - rectangle->width; // Clamp position
    if (rectangle->speed.x > 0) // Only reverse if moving into wall
      rectangle->speed.x *= -1.0f * RESTITUTION;
  }
  // Left wall
  if (rectangle->x <= 0) {
    rectangle->x = 0; // Clamp position
    if (rectangle->speed.x < 0)    // Only reverse if moving into wall
      rectangle->speed.x *= -1.0f * RESTITUTION;
  }
  // Bottom wall
  if (rectangle->y >= (GetScreenHeight() - rectangle->height)) {
    rectangle->y = GetScreenHeight() - rectangle->height; // Clamp position
    if (rectangle->speed.y > 0) // Only reverse if moving into wall
      rectangle->speed.y *= -1.0f * RESTITUTION;
  }
  // Top wall
  if (rectangle->y <= 0) {
    rectangle->y = 0; // Clamp position
    if (rectangle->speed.y < 0)    // Only reverse if moving into wall
      rectangle->speed.y *= -1.0f * RESTITUTION;
  }
}

static void RectangleRectangleCollision(st_Rectangle *r1, st_Rectangle *r2) {}
