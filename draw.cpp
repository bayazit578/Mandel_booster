#include "draw.h"
#include <CSFML/System/Types.h>
#include <stdint.h>

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

void draw_fps(sfClock* fps_clock) {
    static uint32_t frame_count = 0;
    frame_count++;

    if (sfTime_asSeconds(sfClock_getElapsedTime(fps_clock)) 
        >= 1.0f) {
        sfClock_restart(fps_clock);
        printf("FPS: %u\n", frame_count);
        frame_count = 0;
    }
}

void change_color(sfEvent event, color_t* color_offset, 
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


