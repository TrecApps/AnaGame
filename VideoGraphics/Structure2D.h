#pragma once
#include <d2d1_1.h>
#include "VideoGraphics.h" 


// Windows Definitions
#define RECT_2D D2D1_RECT_F

#define ROUNDED_RECT_2D D2D1_ROUNDED_RECT

#define ELLIPSE_2D D2D1_ELLIPSE

#define GRADIENT_STOP_2D D2D1_GRADIENT_STOP

#define POINT_2D D2D1_POINT_2F

#define TRANSFORM_2D D2D1_MATRIX_3X2_F

#define SAFE_RELEASE(x)  if(x) x->Release(); x = nullptr;