#ifndef OPENPYROJET_DISPLAY_PAGE_H
#define OPENPYROJET_DISPLAY_PAGE_H

typedef enum {
    CONFIG_MODE = 0,
    NORMAL_MODE
} DisplayPage;

/**
 * Render a display page.
 */
void display_page_show(DisplayPage state);

/**
 * Notify that the content on the display page has been changed.
 * When the page is currently active, it will trigger rendering.
 */
void display_page_notify_changed(DisplayPage state);

#endif //OPENPYROJET_DISPLAY_PAGE_H
