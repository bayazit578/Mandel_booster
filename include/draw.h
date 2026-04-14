#pragma once

#include <CSFML/Graphics.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#ifdef BENCHMARK
    #define IF_BENCHMARK(...) __VA_ARGS__
    #define DRAWING(...)
#else
    #define IF_BENCHMARK(...)
    #define DRAWING(...) __VA_ARGS__
#endif


#if defined SCALAR_MODE
    #define CALC_MANDELBROT(image, transform, color_offset) \
        calc_mandelbrot_scalar(image, transform, color_offset);
#elif defined ARRAY_MODE
    #define CALC_MANDELBROT(image, transform, color_offset) \
        calc_mandelbrot_array(image, transform, color_offset);
#elif defined INTRIN_MODE
    #define CALC_MANDELBROT(image, transform, color_offset) \
        calc_mandelbrot_intrin(image, transform, color_offset);
#else 
    #define CALC_MANDELBROT(image, transform, color_offset) \
        perror("No selected mode for Mandelbrot rendering"); \
        return EXIT_FAILURE;
#endif

const uint32_t AVERAGE_CYCLE_COUNT = 1000;

const int IMG_WDTH = 800;
const int IMG_HGHT = 600;

typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
} color_t;

typedef struct {
        float scale;
        float vert_offset;
        float hor_offset;
} trans_t;

void render_window(sfRenderWindow* window, sfImage* image, 
                   sfText* title_text, sfClock* fps_clock,
                   color_t color_offset, trans_t transform,
                   sfEvent event, FILE* bench_out_file);

void draw_image_n_text_on_window(sfImage* image, sfRenderWindow* window, 
                                 sfText* title_text);

sfText* text_create();

void handle_events(sfEvent event, sfRenderWindow* window, 
                   color_t color_offset);

void draw_fps(sfClock* fps_clock, sfText* title_text); 
