#include "calc.h"

DRAWING(
    static sfColor set_color_by_N (float N, color_t color_offset);
)

void calc_mandelbrot_scalar(sfImage* image, trans_t transform,
                            color_t color_offset) {
    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        float y0 = (iy - transform.vert_offset) * transform.scale;

        for (uint32_t ix = 0; ix < IMG_WDTH; ix++) {
            float x0 = (ix - transform.hor_offset) * transform.scale;

            float x = 0,
                  y = 0;

            volatile uint8_t N = 0;
            
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
            
            DRAWING(
                sfImage_setPixel(image, (sfVector2u){ix, iy}, 
                                 set_color_by_N(N, color_offset));
            )
        }
    }
}

m256 mm256_set1_ps_array(float val) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = val;
    }
    return result;
}

m256 mm256_set_ps_array(float e7, float e6, float e5, float e4,
                        float e3, float e2, float e1, float e0) {
    m256 result = {{e0, e1, e2, e3, e4, e5, e6, e7}};
    return result;
}

m256 mm256_add_ps_array(m256 a, m256 b) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = a.data[i] + b.data[i];
    }
    return result;
}

m256 mm256_sub_ps_array(m256 a, m256 b) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = a.data[i] - b.data[i];
    }
    return result;
}

m256 mm256_mul_ps_array(m256 a, m256 b) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = a.data[i] * b.data[i];
    }
    return result;
}

m256i mm256_cmp_ps_array(m256 a, m256 b, int cmp_type) {
    m256i result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = (a.data[i] < b.data[i]) ? ~0u : 0;
    }
    return result;
}

int mm256_movemask_ps_array(m256i mask) {
    int result = 0;
    for (int i = 0; i < 8; i++) {
        if (mask.data[i]) result |= (1 << i);
    }
    return result;
}

m256 mm256_blendv_ps_array(m256 a, m256 b, m256i mask) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = mask.data[i] ? b.data[i] : a.data[i];
    }
    return result;
}

void mm256_storeu_ps_array(float* ptr, m256 a) {
    for (int i = 0; i < 8; i++) {
        ptr[i] = a.data[i];
    }
}

m256 mm256_cvtepi32_ps_array(m256i a) {
    m256 result;
    for (int i = 0; i < 8; i++) {
        result.data[i] = (float)a.data[i];
    }
    return result;
}

void calc_mandelbrot_array(sfImage* image, trans_t transform,
                                       color_t color_offset) {
    m256 indices = mm256_set_ps_array(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
    m256 hor_offset_vec = mm256_set1_ps_array(transform.hor_offset);
    m256 scale_vec = mm256_set1_ps_array(transform.scale);
    
    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        float y0 = (iy - transform.vert_offset) * transform.scale;
        m256 y0_vec = mm256_set1_ps_array(y0);
        
        for (uint32_t ix = 0; ix < IMG_WDTH; ix += 8) {
            m256 ix_vec = mm256_set1_ps_array((float)ix);
            
            m256 x0_vec = mm256_add_ps_array(ix_vec, indices);
            x0_vec = mm256_sub_ps_array(x0_vec, hor_offset_vec);
            x0_vec = mm256_mul_ps_array(x0_vec, scale_vec);
            
            m256 x_vec = mm256_set1_ps_array(0);
            m256 y_vec = mm256_set1_ps_array(0);
            m256 N_vec = mm256_set1_ps_array(0);
            m256 TWO_VECT = mm256_set1_ps_array(2.0f);
            m256 R2MAX_VECT = mm256_set1_ps_array(R2MAX);
            
            for (volatile uint32_t iter = 0; iter < NMAX; iter++) {
                m256 x2_vec = mm256_mul_ps_array(x_vec, x_vec);
                m256 y2_vec = mm256_mul_ps_array(y_vec, y_vec);
                m256 xy_vec = mm256_mul_ps_array(x_vec, y_vec);
                m256 r2_vec = mm256_add_ps_array(x2_vec, y2_vec);
                
                x_vec = mm256_add_ps_array(mm256_sub_ps_array(x2_vec, y2_vec), x0_vec);
                y_vec = mm256_add_ps_array(mm256_mul_ps_array(TWO_VECT, xy_vec), y0_vec);
                
                m256 new_N_vec = mm256_add_ps_array(N_vec, mm256_set1_ps_array(1.0f));
                m256i cmp_mask_vec = mm256_cmp_ps_array(r2_vec, R2MAX_VECT, 0);
                
                N_vec = mm256_blendv_ps_array(N_vec, new_N_vec, cmp_mask_vec);
                
                int mask = mm256_movemask_ps_array(cmp_mask_vec);
                if (mask == 0) break;
            }
            
            float N[8];
            mm256_storeu_ps_array(N, N_vec);
            
            DRAWING(
                for (uint32_t i = 0; i < 8; i++) {
                    sfImage_setPixel(image, (sfVector2u){ix + i, iy}, 
                                     set_color_by_N((uint32_t)N[i], color_offset));
                }
            )
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

        for (volatile uint32_t ix = 0; ix < IMG_WDTH; ix += 8) {
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
            
            DRAWING(
                for (uint32_t i = 0; i < 8; i++) {
                    sfImage_setPixel(image, (sfVector2u){ix + i, iy}, 
                                     set_color_by_N(N[i], color_offset));
                }
            )
        }
    }
}

DRAWING(
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
)
