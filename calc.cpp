#include "calc.h"
#include <cstdint>
#include <stdint.h>

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

            float t = (float)N / NMAX;
            uint8_t red   = (uint8_t)(9 * (1 - t) * t * t * t * 255);
            uint8_t green = (uint8_t)(15 * (1 - t) * (1 - t) * t 
                                    * t * 255);
            uint8_t blue  = (uint8_t)(8.5 * (1 - t) * (1 - t) 
                                    * (1 - t) * t * 255); 
            
            uint8_t brightness = 255;

            sfColor color = {(uint8_t)(red - color_offset.red), 
                             (uint8_t)(green - color_offset.green),
                             (uint8_t)(blue - color_offset.blue), 
                             brightness};
            sfImage_setPixel(image, (sfVector2u){ix, iy}, color);
        }
    }
}

void calc_mandelbrot_vector(sfImage* image, trans_t transform,
                                  color_t color_offset) {
    for (uint32_t iy = 0; iy < IMG_HGHT; iy++) {
        float y0 = (iy - transform.vert_offset) * transform.scale;

        for (uint32_t ix = 0; ix < IMG_WDTH; ix += 8) {
            float x0[8];

            for (uint32_t i = 0; i < 8; i++) {
                x0[i] = (ix + i - transform.hor_offset) * transform.scale;
            }

            float x[8] = {};
            float y[8] = {};
            uint32_t N[8] = {};
            uint32_t active[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
            
            if (ix + 8 > IMG_WDTH) {
                for (uint32_t i = IMG_WDTH - ix; i < 8; i++) {
                    active[i] = 0;
                }
            }

            for (uint32_t iter = 0; iter < NMAX; iter++) {
                float x2[8], y2[8], xy[8], r2[8], new_x[8], new_y[8];
                uint32_t cmp_mask[8];
                
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
                    new_x[i] = x2[i] - y2[i] + x0[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    new_y[i] = 2.0f * xy[i] + y0;
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    cmp_mask[i] = (r2[i] < R2MAX) ? ~0u : 0;
                }

                for (uint32_t i = 0; i < 8; i++) {
                    cmp_mask[i] &= active[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    x[i] = (cmp_mask[i]) ? new_x[i] : x[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    y[i] = (cmp_mask[i]) ? new_y[i] : y[i];
                }

                for (uint32_t i = 0; i < 8; i++) {
                    N[i] = (cmp_mask[i]) ? N[i] + 1 : N[i];
                }
                
                for (uint32_t i = 0; i < 8; i++) {
                    active[i] = cmp_mask[i];
                }
                
                uint32_t any_active = 0;
                for (uint32_t i = 0; i < 8; i++) {
                    any_active |= active[i];
                }

                if (!any_active) break;
            }

            for (uint32_t i = 0; i < 8; i++) {
                float t = (float)N[i] / NMAX;
                uint8_t red   = (uint8_t)(9 * (1 - t) * 
                                          t * t * t * 255);
                uint8_t green = (uint8_t)(15 * (1 - t) * 
                                          (1 - t) * t * t * 255);
                uint8_t blue  = (uint8_t)(8.5f * (1 - t) 
                                          * (1 - t) * (1 - t) * t * 255);
                
                sfColor color = {(uint8_t)(red - color_offset.red), 
                                 (uint8_t)(green - color_offset.green),
                                 (uint8_t)(blue - color_offset.blue), 255};
                sfImage_setPixel(image, (sfVector2u){ix + i, iy}, color);
            }
        }
    }
}

void calc_mandelbrot_intrinsic(sfImage* image, trans_t transform,
                               color_t color_offset) {
    
}
