#include "calc.h"
#include <stdint.h>

void calc_mandelbrot_(sfImage* image, float scale, 
                     float vert_offset, float hor_offset,
                     color_t color_offset) {
    for (unsigned int iy = 0; iy < 600; iy++) {
        for (unsigned int ix = 0; ix < 800; ix++) {
            float x0 = (ix - hor_offset) / scale,
                  y0 = (iy - vert_offset) / scale;

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

            uint8_t red =   N - ((uint8_t)log2(log2(N)) % 255);
            uint8_t green = N - ((uint8_t)log2(log2(N)) % 255);
            uint8_t blue =  N - ((uint8_t)log2(log2(N)) % 255);
            uint8_t brightness = 255;
            sfColor color = {(uint8_t)(red - color_offset.red), 
                             (uint8_t)(green - color_offset.green),
                             (uint8_t)(blue - color_offset.blue), 
                             brightness};
            sfImage_setPixel(image, (sfVector2u){ix, iy}, color);
        }
    }
}
