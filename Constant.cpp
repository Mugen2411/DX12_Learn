#include "Constant.h"

float Constant::kPI = 3.1415926f;
float Constant::kPI2 = kPI * 2;

int Constant::kScreenWidth = 1280;
int Constant::kScreenHeight = 960;

int Constant::kGameTopX = 32;
int Constant::kGameTopY = 32;

int Constant::kGameBottomX =
    Constant::kScreenWidth * 0.6 - Constant::kGameTopX;
int Constant::kGameBottomY = Constant::kScreenHeight - Constant::kGameTopY;

float Constant::kZeroBorder = 0.01f;