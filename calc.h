#pragma once

#include <CSFML/Graphics.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <immintrin.h>

#include "draw.h"

const float R2MAX       = 4.f;
const uint16_t NMAX     = 255;

const int IMG_WDTH      = 800;
const int IMG_HGHT      = 600;

const __m256 R2MAX_VECT = _mm256_set1_ps(4.f);
const __m256 NMAX_VECT  = _mm256_set1_ps(255.f);
const __m256 TWO_VECT   = _mm256_set1_ps(2.f);

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

void calc_mandelbrot_intrin(sfImage* image, trans_t transform,
                            color_t color_offset);
