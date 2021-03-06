// window_progress.c
#include "window_progress.hpp"
#include "gui.hpp"
#include <algorithm>

#define WINDOW_PROGRESS_MAX_TEXT 16

void window_progress_init(window_progress_t *window) {
    window->color_back = gui_defaults.color_back;
    window->color_text = gui_defaults.color_text;
    window->color_progress = COLOR_LIME;
    window->font = gui_defaults.font;
    window->padding = gui_defaults.padding;
    window->alignment = ALIGN_CENTER;
    window->height_progress = 8;
    window->format = "%.0f%%";
    window->value = 0;
    window->min = 0;
    window->max = 100;
}

void window_progress_draw(window_progress_t *window) {
    if (((window->flg & (WINDOW_FLG_INVALID | WINDOW_FLG_VISIBLE)) == (WINDOW_FLG_INVALID | WINDOW_FLG_VISIBLE))) {
        rect_ui16_t rc = window->rect;
        char text[WINDOW_PROGRESS_MAX_TEXT];
        snprintf(text, WINDOW_PROGRESS_MAX_TEXT, window->format, (double)window->value);
        int progress_w = (int)(rc.w * (window->value - window->min) / (window->max - window->min));
        rc.h = window->height_progress;

        if (1) { // TODO: border attribute, default is no border (Prusa GUI)
            rc.x += progress_w;
            rc.w -= progress_w;
            display::FillRect(rc, window->color_back);
            rc.x = window->rect.x;
            rc.w = progress_w;
            display::FillRect(rc, window->color_progress);
        } else {
            display::FillRect(rc, window->color_progress);
            rc.x += progress_w;
            rc.w = window->rect.w - progress_w;
            display::DrawRect(rc, window->color_progress);
            rc = rect_ui16_sub_padding_ui8(rc, padding_ui8(1, 1, 1, 1));
            display::FillRect(rc, window->color_back);
        }

        rc = window->rect;
        if (rc.h > window->height_progress) {
            rc.y += window->height_progress;
            rc.h -= window->height_progress;
            render_text_align(rc, // @@TODO translate this string here?
                text,
                window->font,
                window->color_back,
                window->color_text,
                window->padding,
                window->alignment);
        }
        window->flg &= ~WINDOW_FLG_INVALID;
    }
}

const window_class_progress_t window_class_progress = {
    {
        WINDOW_CLS_PROGRESS,
        sizeof(window_progress_t),
        (window_init_t *)window_progress_init,
        0,
        (window_draw_t *)window_progress_draw,
        0,
    },
};
//todo use this virtual methods does not work yet - stupid memcpy
/*
void window_progress_t::setValue(float val) {
    value = std::max(min, std::min(val, max));
}
*/
//todo erase me, virtual methods does not work yet - stupid memcpy
void window_progress_t::SetValue(float val) {
    value = std::max(min, std::min(val, max));
    Invalidate();
}
