#include "calc.h"

static sfColor set_color_by_N (float N, color_t color_offset); 

void calc_mandelbrot_scalar(sfImage* image, trans_t transform,
                            color_t color_offset) {
    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        float y0 = (iy - transform.vert_offset) * transform.scale;

        for (uint32_t ix = 0; ix < IMG_WDTH; ix++) {
            float x0 = (ix - transform.hor_offset) * transform.scale;

            float x = 0,
                  y = 0;

            uint8_t N = 0;
            
            for (; N < NMAX; N++) {
                float x2 = x*x,
                      y2 = y*y,
                      xy = x*y;
                float r2 = x2 + y2;
                if (r2 >= R2MAX) {
                    break;
                }

                x = x2 - y2 + x0;
                y = 2*xy + y0;
            }

            sfImage_setPixel(image, (sfVector2u){ix, iy}, 
                             set_color_by_N(N, color_offset));
        }
    }
}

void calc_mandelbrot_array(sfImage* image, trans_t transform,
                            color_t color_offset) {
    float x0[8] = {};
    float y0 = 0;
    float indices[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        y0 = (iy - transform.vert_offset) * transform.scale;

        for (uint32_t ix = 0; ix < IMG_WDTH; ix += 8) {
            for (uint32_t i = 0; i < 8; i++) {
                x0[i] = ix + indices[i];
            }

            for (uint32_t i = 0; i < 8; i++) {
                x0[i] = x0[i] - transform.hor_offset;
            }

            for (uint32_t i = 0; i < 8; i++) {
                x0[i] = x0[i] * transform.scale;
            }

            float x[8] = {};
            float y[8] = {};
            uint32_t N[8] = {};
            uint32_t any_active = 0;
            
            for (uint32_t iter = 0; iter < NMAX; iter++) {
                float x2[8] = {},
                      y2[8] = {}, 
                      xy[8] = {}, 
                      r2[8] = {};
                uint32_t cmp_mask[8] = {};
                any_active = 0;
                
                for (uint32_t i = 0; i < 8; i++) {
                    x2[i] = x[i] * x[i];
                }

                for (uint32_t i = 0; i < 8; i++) {
                    y2[i] = y[i] * y[i];
                }

                for (uint32_t i = 0; i < 8; i++) {
                    xy[i] = x[i] * y[i];
                }

                for (uint32_t i = 0; i < 8; i++) {
                    r2[i] = x2[i] + y2[i];
                }

                for (uint32_t i = 0; i < 8; i++) {
                    x[i] = x2[i] - y2[i] + x0[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    y[i] = 2.0f * xy[i] + y0;
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    cmp_mask[i] = (r2[i] < R2MAX) ? ~0u : 0;
                }

                for (uint32_t i = 0; i < 8; i++) {
                    N[i] = (cmp_mask[i]) ? N[i] + 1 : N[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    any_active |= cmp_mask[i];
                }

                if (!any_active) break;
            }

            for (uint32_t i = 0; i < 8; i++) {
                sfImage_setPixel(image, (sfVector2u){ix + i, iy}, 
                                 set_color_by_N(N[i], color_offset));
            }
        }
    }
}

void calc_mandelbrot_intrin(sfImage* image, trans_t transform,
                               color_t color_offset) {
    __m256 indices = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 
                                   3.f, 2.f, 1.f, 0.f);
    __m256 hor_offset_vec = _mm256_set1_ps(transform.hor_offset);
    __m256 scale_vec = _mm256_set1_ps(transform.scale);

    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        float y0 = (iy - transform.vert_offset) * transform.scale;
        __m256 y0_vec = _mm256_set1_ps(y0);

        for (uint32_t ix = 0; ix < IMG_WDTH; ix += 8) {
            __m256 ix_vec = _mm256_set1_ps(float(ix));

            __m256 x0_vec = _mm256_add_ps(ix_vec, indices);
            x0_vec = _mm256_sub_ps(x0_vec, hor_offset_vec);
            x0_vec = _mm256_mul_ps(x0_vec, scale_vec);   

            __m256 x_vec  = _mm256_set1_ps(0);
            __m256 y_vec  = _mm256_set1_ps(0);
            __m256 N_vec  = _mm256_cvtepi32_ps(_mm256_set1_epi32(0));

            for (uint32_t iter = 0; iter < NMAX; iter++) {
                __m256 x2_vec = _mm256_mul_ps(x_vec, x_vec);
                __m256 y2_vec = _mm256_mul_ps(y_vec, y_vec);
                __m256 xy_vec = _mm256_mul_ps(x_vec, y_vec);
                __m256 r2_vec = _mm256_add_ps(x2_vec, y2_vec);
                __m256 cmp_mask_vec = _mm256_set1_ps(0);

                x_vec = _mm256_add_ps(_mm256_sub_ps(x2_vec, y2_vec), 
                                                           x0_vec);
                y_vec = _mm256_add_ps(_mm256_mul_ps(TWO_VECT, xy_vec), 
                                                           y0_vec);
                __m256 new_N_vec = _mm256_add_ps(N_vec, _mm256_set1_ps(1));

                cmp_mask_vec = _mm256_cmp_ps(r2_vec, R2MAX_VECT, _CMP_LT_OQ);
                N_vec = _mm256_blendv_ps(N_vec, new_N_vec, cmp_mask_vec); 

                int mask = _mm256_movemask_ps(cmp_mask_vec);

                if (mask == 0) {
                    break;
                }
            }

            float* N = (float*)calloc(8, sizeof(float));
            _mm256_storeu_ps(N, N_vec);

            for (uint32_t i = 0; i < 8; i++) {
                sfImage_setPixel(image, (sfVector2u){ix + i, iy}, 
                                 set_color_by_N(N[i], color_offset));
            }  
        }
    }
}

static sfColor set_color_by_N (float N, color_t color_offset) {
    float t = N / NMAX;
    uint8_t red   = (uint8_t)(9 * (1 - t) * 
                              t * t * t * 255);
    uint8_t green = (uint8_t)(15 * (1 - t) * 
                              (1 - t) * t * t * 255);
    uint8_t blue  = (uint8_t)(8.5f * (1 - t) 
                              * (1 - t) * (1 - t) * t * 255);

    sfColor color = {(uint8_t)(red - color_offset.red), 
                     (uint8_t)(green - color_offset.green),
                     (uint8_t)(blue - color_offset.blue), 
                     255};
    return color;
}
