#ifndef OPENPYROJET_DISPLAY_PAGE_H
#define OPENPYROJET_DISPLAY_PAGE_H

typedef enum DisplayPage {
    CONFIG_MODE = 0,
    NORMAL_MODE
} DisplayState;

/**
 * Render a display page.
 */
void display_page_show(enum DisplayPage state);

/**
 * Notify that the content on the display page has been changed.
 * When the page is currently active, it will trigger rendering.
 */
void display_page_notify_changed(enum DisplayPage state);

#endif //OPENPYROJET_DISPLAY_PAGE_H
