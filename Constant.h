#pragma once

namespace Constant {
constexpr float kPI = 3.1415926f;
constexpr float kPI2 = kPI * 2.0f;

constexpr float kPerFrame = 1.0f / 60.0f;

constexpr int kScreenWidth = 1280;
constexpr int kScreenHeight = 960;

constexpr int kGameTopX = 32;
constexpr int kGameTopY = 32;
constexpr int kGameBottomX = 768;
constexpr int kGameBottomY = 928;
constexpr int kGameWidth = kGameBottomX - kGameTopX;
constexpr int kGameHeight = kGameBottomY - kGameTopY;

static constexpr float kZeroBorder = 0.01f;

namespace Priority {
constexpr float kMarker = 0.0f;
constexpr float kEffect = 0.3f;
constexpr float kBullet = 0.4f;
constexpr float kPlayer = 0.5f;
}  // namespace Priorityf
}  // namespace Constant