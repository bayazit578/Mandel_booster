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

        for (uint32_t ix = 0; ix < VECTOR_WDTH; ix++) {
                float x0[8] = {0, 1, 2, 3, 4, 5, 6, 7};

                for (uint32_t i = 0; i < 8; i++) {
                    x0[i] = (x0[i] + ix - transform.hor_offset) * 
                            transform.scale;
                } 

                float x[8] = {},
                      y    = 0;

                uint8_t N[8] = {};
                bool processing[8] = {1, 1, 1, 1, 1, 1, 1, 1};
                bool next = true;

                while (next) {
                    float y2   = y * y;
                          
                    for (uint32_t i = 0; i < 8; i++) {
                        float x2 = x[i]*x[i],
                              xy = x[i]*y,
                              r2 = x2 + y2;

                        if (r2 >= R2MAX || N[i] >= NMAX) {
                            processing[i] = 0;
                            continue;
                        }

                        x[i] = x2 - y2 + x0[i];
                        y    = 2*xy + y0;

                        N[i] += processing[i];
                    }

                    bool next = false;

                    for (uint32_t i = 0; i < 8; i++) {
                        next |= processing[i];
                    }
                }

            for (uint32_t i = 0; i < 8; i++) {
                float t = (float)N[i] / NMAX;
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
}

void calc_mandelbrot_intrinsic(sfImage* image, trans_t transform,
                               color_t color_offset) {
    
}
