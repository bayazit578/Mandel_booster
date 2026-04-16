#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <CSFML/Graphics/Font.h>
#include <CSFML/Window.h>
#include <CSFML/Graphics.h>
#include <x86intrin.h>

#include "calc.h"
#include "draw.h"

int main(int argc, char* argv[]) {
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

    sfRenderWindow* window = 
                        sfRenderWindow_create({IMG_WDTH, IMG_HGHT, 32},
                                              "Mandelbrot", sfDefaultStyle,
                                              sfWindowed, NULL);

    sfImage* image = sfImage_create({IMG_WDTH, IMG_HGHT});
    sfClock* fps_clock = sfClock_create();
    sfText* title_text = text_create(); 
    sfEvent event = {}; 

    color_t color_offset = {};
    trans_t transform = {0.007f, 300.f, 400.f};

    render_window(window, image, title_text, 
                  fps_clock, color_offset, transform,
                  event, bench_out_file);

    return EXIT_SUCCESS;
}
