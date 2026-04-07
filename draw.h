#pragma once

#include <CSFML/Graphics.h>
#include <stdint.h>
#include <math.h>

typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
} color_t;

void change_color(sfEvent event, color_t* color_offset, 
                  uint8_t increment); 
