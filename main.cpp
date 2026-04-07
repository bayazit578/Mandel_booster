#include <stdio.h>
#include <CSFML/Window.h>
#include <CSFML/Graphics.h>
#include <stdint.h>

#include "calc.h"
#include "draw.h"

int main() {
    sfRenderWindow* window = sfRenderWindow_create({800, 600, 32},
                                        "Mandelbrot", sfDefaultStyle, 
                                        sfWindowed, NULL);
    
    sfImage* image = sfImage_create({800, 600});

    color_t color_offset = {};
        
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            switch (event.type) {
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                    break;

                case sfEvtKeyPressed:
                    // printf("key\n");
                    if (event.key.control) {
                        change_color(event, &color_offset, 1); 
                    }

                    else if (event.key.alt) {
                        // printf("alt was pressed\n");
                        change_color(event, &color_offset, -1); 
                    }

                default:
                    break;
            }
        }

        sfRenderWindow_clear(window, sfBlack);

        calc_mandelbrot(image, 200.f, 300.f, 400.f, color_offset); 

        sfTexture* texture = sfTexture_create({800, 600});
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
