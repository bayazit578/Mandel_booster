#include "draw.h"
#include <stdint.h>

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
