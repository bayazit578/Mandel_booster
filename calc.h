#pragma once

#include <CSFML/Graphics.h>
#include <stdint.h>
#include <math.h>

#include "draw.h"

const float R2MAX     = 4.f;
const uint16_t NMAX   = 255;

const int IMG_WDTH    = 800;
const int IMG_HGHT    = 600;
const int VECTOR_WDTH = 100;
const int VECTOR_HGHT = 75;

typedef struct {
        float scale;
        float vert_offset;
        float hor_offset;
} trans_t;

typedef struct {
        float data[8];  
} m256;

void calc_mandelbrot_scalar(sfImage* image, trans_t transform,
                            color_t color_offset);

void calc_mandelbrot_vector(sfImage* image, trans_t transform,
                            color_t color_offset);

void calc_mandelbrot_intrinc(sfImage* image, trans_t transform,
                            color_t color_offset);
