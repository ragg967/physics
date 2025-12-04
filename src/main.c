#include "raylib.h"
#define CIRCLES_IMPLEMENTATION
#include "../includes/circles.h"

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


int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(window.width, window.height, "raylib [Physics]");
  SetTargetFPS(60);

  st_Circle *circle1 =
      initCircle(200.0f, 300.0f, 50.0f, ORANGE, (Vector2){5.0f, 4.0f});
  st_Circle *circle2 =
      initCircle(600.0f, 100.0f, 50.0f, BLUE, (Vector2){5.0f, 4.0f});

  while (!WindowShouldClose()) {
    // Update
    UpdateCircle(circle1);
    UpdateCircle(circle2);

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
  free(circle1);
  free(circle2);
  CloseWindow();
}

