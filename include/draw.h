#pragma once

#include <CSFML/Graphics.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
} color_t;

void handle_events(sfEvent event, sfRenderWindow* window, 
                   color_t color_offset);

void draw_fps(sfClock* fps_clock, sfText* title_text); 
