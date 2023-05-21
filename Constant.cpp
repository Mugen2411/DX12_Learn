#include "Constant.h"

float Constant::PI = 3.1415926f;
float Constant::PI2 = PI * 2;

int Constant::screen_width = 1280;
int Constant::screen_height = 960;

int Constant::game_top_x = 32;
int Constant::game_top_y = 32;

int Constant::game_bottom_x =
    Constant::screen_width * 0.6 - Constant::game_top_x;
int Constant::game_bottom_y = Constant::screen_height - Constant::game_top_y;