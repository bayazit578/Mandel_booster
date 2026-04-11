#include "draw.h"


static void change_color(sfEvent event, color_t* color_offset, 
                         uint8_t increment);

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

sfText* prepare_text() {
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


