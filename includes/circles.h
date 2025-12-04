#ifndef CIRCLES_H
#define CIRCLES_H

#include "raylib.h"
#include "utilities.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
  float x, y, radius;
  Color color;
  Vector2 speed;
} st_Circle;

static void DrawCircleDebug(st_Circle *circle);
static void DrawDebugText(float offset, st_Circle *circle);
static void UpdateCircle(st_Circle *circle);
static void WallCollision(st_Circle *circle);
static void CircleCircleCollision(st_Circle *c1, st_Circle *c2);

#endif // CIRCLES_H

#define CIRCLES_IMPLEMENTATION
#if defined(CIRCLES_IMPLEMENTATION)

#if defined(__cplusplus)
extern "C" {
#endif
// implamentation

static void UpdateCircle(st_Circle *circle) {
  circle->x += circle->speed.x;
  circle->y += circle->speed.y;

  circle->speed.y += GRAVITY * GetFrameTime();
  circle->speed.y -= AIR_RESISTANCE * GetFrameTime();
  circle->speed.x -= AIR_RESISTANCE * GetFrameTime();

  WallCollision(circle);

  if (fabsf(circle->speed.y) < 0.15f &&
      circle->y >= GetScreenHeight() - circle->radius - 1.0f) {
    circle->speed.y = 0.0f; // Stop vertical movement when nearly at rest
  }
  if (fabsf(circle->speed.x) < 0.15f) {
    circle->speed.x = 0.0f; // Stop horizontal movement when nearly at rest
  }
}

static st_Circle *initCircle(float x, float y, float radius, Color color,
                             Vector2 speed) {
  st_Circle *newCircle = malloc(sizeof(st_Circle));
  if (!newCircle)
    return NULL;
  *newCircle = (st_Circle){
      .x = x,
      .y = y,
      .radius = radius,
      .color = color,
      .speed = speed,
  };
  return newCircle;
}

static void DrawCircleDebug(st_Circle *circle) {
  DrawCircle(circle->x, circle->y, 5.0f, GREEN);
  DrawLine(circle->x, circle->y, circle->x + circle->radius, circle->y, GREEN);
  DrawLine(circle->x, circle->y, circle->x + circle->speed.x * 10.0f,
           circle->y + circle->speed.y * 10.0f, WHITE);
}

static void DrawDebugText(float offset, st_Circle *circle) {
  int fontSize = 14;
  int padding = 5;

  DrawText(TextFormat("Position: x=%.1f y=%.1f", circle->x, circle->y),
           0 + padding, 0 + padding + offset, fontSize, GREEN);
}

static void WallCollision(st_Circle *circle) {
  // Right wall
  if (circle->x >= (GetScreenWidth() - circle->radius)) {
    circle->x = GetScreenWidth() - circle->radius; // Clamp position
    if (circle->speed.x > 0) // Only reverse if moving into wall
      circle->speed.x *= -1.0f * RESTITUTION;
  }
  // Left wall
  if (circle->x <= circle->radius) {
    circle->x = circle->radius; // Clamp position
    if (circle->speed.x < 0)    // Only reverse if moving into wall
      circle->speed.x *= -1.0f * RESTITUTION;
  }
  // Bottom wall
  if (circle->y >= (GetScreenHeight() - circle->radius)) {
    circle->y = GetScreenHeight() - circle->radius; // Clamp position
    if (circle->speed.y > 0) // Only reverse if moving into wall
      circle->speed.y *= -1.0f * RESTITUTION;
  }
  // Top wall
  if (circle->y <= circle->radius) {
    circle->y = circle->radius; // Clamp position
    if (circle->speed.y < 0)    // Only reverse if moving into wall
      circle->speed.y *= -1.0f * RESTITUTION;
  }
}

static void CircleCircleCollision(st_Circle *c1, st_Circle *c2) {
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
#ifdef __cplusplus
}
#endif

#endif // CIRCLES_IMPLEMENTATION
