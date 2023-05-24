#pragma once

namespace Constant {
const float kPI = 3.1415926f;
const float kPI2 = kPI * 2.0f;

const float kPerFrame = 1.0f / 60.0f;

const int kScreenWidth = 1280;
const int kScreenHeight = 960;

const int kGameTopX = 32;
const int kGameTopY = 32;
const int kGameBottomX = 768;
const int kGameBottomY = 928;
const int kGameWidth = kGameBottomX - kGameTopX;
const int kGameHeight = kGameBottomY - kGameTopY;

static const float kZeroBorder = 0.01f;

namespace Priority {
const float kMarker = 0.0f;
const float kEffect = 0.3f;
const float kBullet = 0.4f;
const float kPlayer = 0.5f;
}  // namespace Priorityf
}  // namespace Constant