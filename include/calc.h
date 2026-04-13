#pragma once

#include <CSFML/Graphics.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <immintrin.h>

#include "draw.h"

typedef struct {
        float scale;
        float vert_offset;
        float hor_offset;
} trans_t;

typedef struct {
    float data[8];
} m256;

typedef struct {
    uint32_t data[8];
} m256i;

const float R2MAX   = 4.f;
const uint16_t NMAX = 255;

const int IMG_WDTH = 800;
const int IMG_HGHT = 600;

const __m256 R2MAX_VECT = _mm256_set1_ps(R2MAX);
const __m256 TWO_VECT   = _mm256_set1_ps(2.f);

void calc_mandelbrot_scalar(sfImage* image, trans_t transform,
                            color_t color_offset);

void calc_mandelbrot_array(sfImage* image, trans_t transform,
                            color_t color_offset);

inline __attribute__((always_inline)) m256 mm256_set1_ps_array(float val);
inline __attribute__((always_inline)) m256 mm256_set_ps_array(float e7, float e6, float e5, float e4,
                                                              float e3, float e2, float e1, float e0);
inline __attribute__((always_inline)) m256 mm256_add_ps_array(m256 a, m256 b);
inline __attribute__((always_inline)) m256 mm256_sub_ps_array(m256 a, m256 b);
inline __attribute__((always_inline)) m256 mm256_mul_ps_array(m256 a, m256 b);
inline __attribute__((always_inline)) m256i mm256_cmp_ps_array(m256 a, m256 b, int cmp_type);
inline __attribute__((always_inline)) int mm256_movemask_ps_array(m256i mask);
inline __attribute__((always_inline)) m256 mm256_blendv_ps_array(m256 a, m256 b, m256i mask);
inline __attribute__((always_inline)) void mm256_storeu_ps_array(float* ptr, m256 a);
inline __attribute__((always_inline)) m256 mm256_cvtepi32_ps_array(m256i a); 

void calc_mandelbrot_intrin(sfImage* image, trans_t transform,
                            color_t color_offset);
