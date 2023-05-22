#pragma once

namespace Constant {
const float kPI = 3.1415926f;
const float kPI2 = kPI * 2;

const int kScreenWidth = 1280;
const int kScreenHeight = 960;

const int kGameTopX = 32;
const int kGameTopY = 32;
const int kGameBottomX = kScreenWidth * 0.6 - kGameTopX;
const int kGameBottomY = kScreenHeight - kGameTopY;

static const float kZeroBorder = 0.01f;

namespace Priority {
const float kEffect = 0.5f;
const float kBullet = 0.4f;
}  // namespace Priorityf
}  // namespace Constant