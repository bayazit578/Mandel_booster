#pragma once

#include <CSFML/Graphics.h>
#include <stdint.h>
#include <math.h>

#include "draw.h"

const float R2MAX = 100.f;
const uint16_t NMAX = 255;

void calc_mandelbrot(sfImage* image, float scale, 
                     float vert_offset, float hor_offset,
                     color_t color_offset); 
