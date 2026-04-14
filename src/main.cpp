#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <CSFML/Graphics/Font.h>
#include <CSFML/Window.h>
#include <CSFML/Graphics.h>
#include <x86intrin.h>

#include "calc.h"
#include "draw.h"

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

int main(int argc, char* argv[]) {
    IF_BENCHMARK(
        char* bench_filename = NULL;
        if (argc == 2) {
            bench_filename = argv[1];
        } else {
            fprintf(stderr, "No output file for benchmark\n");
            return EXIT_FAILURE;
        }

        FILE* bench_out_file = fopen(bench_filename, "w");
        if (!bench_out_file) {
            perror("fopen error");
        }
    )

    sfRenderWindow* window = 
                        sfRenderWindow_create({IMG_WDTH, IMG_HGHT, 32},
                                              "Mandelbrot", sfDefaultStyle,
                                              sfWindowed, NULL);

    sfImage* image = sfImage_create({IMG_WDTH, IMG_HGHT});
   
    sfClock* fps_clock = sfClock_create();

    sfFont* font = sfFont_createFromFile("fonts/Cascadia.ttf");

    sfText* title_text = sfText_create(font);
    sfText_setCharacterSize(title_text, 16);
    sfText_setFillColor(title_text, {255, 255, 255, 255});

    sfText_setOutlineColor(title_text, sfBlack);
    sfText_setOutlineThickness(title_text, 2.0f);    
        
    sfEvent event = {}; 

    color_t color_offset = {};
    trans_t transform = {0.007f, 300.f, 400.f};

    IF_BENCHMARK(uint32_t cycle_counter = 0;)

    while (sfRenderWindow_isOpen(window)) {
        DRAWING(
            while (sfRenderWindow_pollEvent(window, &event)) {
                handle_events(event, window, color_offset);
            }

            sfRenderWindow_clear(window, sfBlack);
        )

        IF_BENCHMARK(
            cycle_counter++;
            if (cycle_counter == AVERAGE_CYCLE_COUNT) {
                break;
            }

            _mm_lfence(); 
            int64_t clock1 = __rdtsc();
        )
       
        CALC_MANDELBROT(image, transform, color_offset)

        IF_BENCHMARK(
                _mm_lfence(); 
                int64_t clock2 = __rdtsc();
                fprintf(bench_out_file, "%ld\n", clock2 - clock1);
        )
        
        DRAWING(
            draw_fps(fps_clock, title_text);

        sfTexture* texture = sfTexture_create({IMG_WDTH, IMG_HGHT});
        sfTexture_updateFromImage(texture, image, {0, 0});
        sfSprite* sprite = sfSprite_create(texture);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_drawText(window, title_text, NULL);

        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        )
    }

    DRAWING(
        sfRenderWindow_destroy(window);
    )

    sfImage_destroy(image);
    sfRenderWindow_display(window);
    
    return EXIT_SUCCESS;
}
