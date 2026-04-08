#include <CSFML/Graphics/Font.h>
#include <stdint.h>
#include <CSFML/Window.h>
#include <CSFML/Graphics.h>

#include "calc.h"
#include "draw.h"

int main() {
    sfRenderWindow* window = 
                    sfRenderWindow_create({IMG_WDTH, IMG_HGHT, 32},
                                          "Mandelbrot", sfDefaultStyle, 
                                          sfWindowed, NULL);
    
    sfImage* image = sfImage_create({IMG_WDTH, IMG_HGHT});

    sfClock* fps_clock = sfClock_create();

    sfFont* font = sfFont_createFromFile("fonts/MTF Epic.ttf"); 

    sfText* title_text = sfText_create(font);
    sfText_setString(title_text, "FPS:GAY");
    sfText_setCharacterSize(title_text, 10);
    sfText_setFillColor(title_text, {255, 255, 255, 255});

    sfText_setOutlineColor(title_text, sfBlack);
    sfText_setOutlineThickness(title_text, 2.0f);
    
    color_t color_offset = {};
    trans_t transform = {0.007f, 300.f, 400.f};
        
    sfEvent event = {};

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            handle_events(event, window, color_offset);
        }

        sfRenderWindow_clear(window, sfBlack);

        calc_mandelbrot_vector(image, transform, color_offset);

        draw_fps(fps_clock);

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
