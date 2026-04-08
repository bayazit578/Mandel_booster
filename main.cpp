#include <stdio.h>
#include <CSFML/Window.h>
#include <CSFML/Graphics.h>
#include <stdint.h>

#include "calc.h"
#include "draw.h"

int main() {
    sfRenderWindow* window = 
                    sfRenderWindow_create({IMG_WDTH, IMG_HGHT, 32},
                                          "Mandelbrot", sfDefaultStyle, 
                                          sfWindowed, NULL);
    
    sfImage* image = sfImage_create({IMG_WDTH, IMG_HGHT});

    sfClock* fps_clock = sfClock_create();
    uint32_t frame_count = 0;
    uint32_t fps = 0;
    
    color_t color_offset = {};
    trans_t transform = {0.003f, 388.f, 680.f};
        
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
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

        sfRenderWindow_clear(window, sfBlack);

        calc_mandelbrot_vector(image, transform, 
                               color_offset);

        frame_count++;
        if (sfTime_asSeconds(sfClock_getElapsedTime(fps_clock)) 
            >= 1.0f) {
            fps = frame_count;
            frame_count = 0;
            sfClock_restart(fps_clock);
            printf("FPS: %u\n", fps);
        }

        sfTexture* texture = sfTexture_create({IMG_WDTH, IMG_HGHT});
        sfTexture_updateFromImage(texture, image, {0, 0});
        sfSprite* sprite = sfSprite_create(texture);
        sfRenderWindow_drawSprite(window, sprite, NULL);

        sfRenderWindow_display(window);
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
    }

    sfImage_destroy(image);
    sfRenderWindow_destroy(window);

    return 0;
}
