#include "draw.h"
#include "calc.h"

static void change_color(sfEvent event, color_t* color_offset, 
                         uint8_t increment);

void render_window(sfRenderWindow* window, sfImage* image, 
                   sfText* title_text, sfClock* fps_clock,
                   color_t color_offset, trans_t transform,
                   sfEvent event, FILE* bench_out_file) {
    IF_BENCHMARK(
        uint32_t cycle_counter = 0;
    )
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
            draw_image_n_text_on_window(image, window, title_text);
        )
    }

    sfImage_destroy(image);
    sfRenderWindow_destroy(window);
}

void draw_image_n_text_on_window(sfImage* image, sfRenderWindow* window, 
                                 sfText* title_text) {
    sfTexture* texture = sfTexture_create({IMG_WDTH, IMG_HGHT});
    sfTexture_updateFromImage(texture, image, {0, 0});
    sfSprite* sprite = sfSprite_create(texture);
    sfRenderWindow_drawSprite(window, sprite, NULL);
    sfRenderWindow_drawText(window, title_text, NULL);

    sfRenderWindow_display(window);
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
}

sfText* text_create() {
    sfFont* font = sfFont_createFromFile("fonts/Cascadia.ttf");

    sfText* title_text = sfText_create(font);
    sfText_setCharacterSize(title_text, 16);
    sfText_setFillColor(title_text, {255, 255, 255, 255});

    sfText_setOutlineColor(title_text, sfBlack);
    sfText_setOutlineThickness(title_text, 2.0f);

    return title_text;
}

void handle_events(sfEvent event, sfRenderWindow* window, 
                   color_t color_offset) {
    switch (event.type) {
        case sfEvtClosed:
            sfRenderWindow_close(window);
            break;

        case sfEvtKeyPressed:
            if (event.key.control) {
                change_color(event, &color_offset, 1); 
            }

            else if (event.key.alt) {
                change_color(event, &color_offset, -1); 
            }

            else if (event.key.shift 
                  && event.key.code == sfKeyEqual) {
            }

            break;

        default:
            break;
    }
}

sfText* prepare_create() {
    sfFont* font = sfFont_createFromFile("fonts/Cascadia.ttf"); 

    sfText* title_text = sfText_create(font);
    sfText_setCharacterSize(title_text, 18);
    sfText_setFillColor(title_text, sfWhite);
    sfText_setOutlineColor(title_text, sfBlack);
    sfText_setOutlineThickness(title_text, 2.0f); 

    sfText_setPosition(title_text, {5, 0});

    return title_text;
}

void draw_fps(sfClock* fps_clock, sfText* title_text) {
    static uint32_t frame_count = 0;
    frame_count++;

    if (sfTime_asSeconds(sfClock_getElapsedTime(fps_clock)) 
        >= 1.0f) {
        sfClock_restart(fps_clock);
        char* buffer = (char*)calloc(8, sizeof(char));
        sprintf(buffer, "%d", frame_count);
        sfText_setString(title_text, buffer);
        frame_count = 0;
    }
}

static void change_color(sfEvent event, color_t* color_offset, 
                         uint8_t increment) {
    
    switch (event.key.code) {
        case sfKeyR:
            color_offset->red += increment;
            break;

        case sfKeyG:
            color_offset->green += increment;
            break;

        case sfKeyB:
            color_offset->blue += increment;
            break;

        default:
            break;
    }
}


