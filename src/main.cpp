#include <algorithm>
#include <format>
#include <iostream>
#include <random>
#include <raylib.h>
#include <raymath.h>

constexpr int world_size_x = 1600;
constexpr int world_size_y = 900;

constexpr Color NIGHTBLUE{0, 0, 20, 255};
constexpr float GRAVITY = 9.8;
float WIND = 0.0;
constexpr size_t MAX_SNOW = 1024 * 8;

float normal_rand() { return float(std::rand()) / RAND_MAX; }

struct SnowParticle {
  Vector2 position{};
  Vector2 velocity{};
};

bool out_of_bound(const SnowParticle &part) {
  const auto &pos = part.position;
  return pos.x > world_size_x || pos.x < 0.0 || pos.y > world_size_y ||
         pos.y < 0.0;
};
int main(int argc, char **argv) {
  std::vector<SnowParticle> snow_slots{};
  snow_slots.reserve(MAX_SNOW);
  InitWindow(world_size_x, world_size_y, "It's snowing!");
  float time = 0.0;
  while (!WindowShouldClose()) {
    float delta = GetFrameTime();
    time += delta;
    int8_t input_dir = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    WIND = Clamp(WIND + input_dir * delta, -10, 10);
    if (snow_slots.size() < snow_slots.capacity()) {
      snow_slots.emplace_back(
          Vector2(1600.0 * (static_cast<double>(std::rand()) / RAND_MAX), 5.0),
          Vector2((normal_rand() - 0.5) * 16.0, GRAVITY));
    }
    for (auto &part : snow_slots) {
      part.velocity.x += WIND * delta;
      part.position += Vector2Scale(part.velocity, 10.0 * delta);
    }
    std::erase_if(snow_slots, out_of_bound);
    BeginDrawing();
    ClearBackground(NIGHTBLUE);
    std::ranges::for_each(snow_slots, [](const auto &snow) {
      DrawCircle(snow.position.x, snow.position.y, 10.0f, RAYWHITE);
    });
    DrawRectangle(15, 15, 200, 60 + 10, GRAY);
    DrawText(std::format("Time: {}", time).c_str(), 20, 20, 20, RAYWHITE);
    DrawText(std::format("FPS: {}", GetFPS()).c_str(), 20, 40, 20, RAYWHITE);
    DrawText(std::format("Wind: {}", WIND).c_str(), 20, 60, 20, RAYWHITE);
    EndDrawing();
  }
  return 0;
}
