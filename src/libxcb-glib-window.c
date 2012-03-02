/*
 * libxcb-glib - Library to integrate XCB nicely with GLib
 *
 * Copyright © 2012 Quentin "Sardem FF7" Glidic
 *
 * This file is part of libxcb-glib.
 *
 * libxcb-glib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libxcb-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libxcb-glib. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <glib.h>
#include <xcb/xcb.h>

#include <libxcb-glib-types.h>
#include <libxcb-glib-source.h>
#include "libxcb-glib-source-internal.h"
#include <libxcb-glib-window.h>
#include "libxcb-glib-window-internal.h"

struct _GXcbWindow {
    GXcbSource *source;
    xcb_window_t window;
    gpointer user_data;
    GXcbWindowExposeEventCallback expose_event_callback;
    GXcbWindowButtonPressEventCallback button_press_event_callback;
    GXcbWindowButtonReleaseEventCallback button_release_event_callback;
};

GXcbWindow *
g_xcb_window_new(GXcbSource *source, guint8 depth, xcb_window_t parent, gint16 x, gint16 y, guint16 width, guint16 height, guint16 border_width, guint16 _class, xcb_visualid_t visual, guint32 value_mask, const guint32 *value_list, gpointer user_data)
{
    GXcbWindow *window;

    window = g_new0(GXcbWindow, 1);

    window->source = source;
    window->user_data = user_data;

    window->window = xcb_generate_id(g_xcb_source_get_connection(source));
    xcb_create_window(g_xcb_source_get_connection(source), depth, window->window, parent, x, y, width, height, border_width, _class, visual, value_mask, value_list);

    g_xcb_source_attach_window(source, window);

    return window;
}

void
g_xcb_window_free(GXcbWindow *self)
{
    g_xcb_source_detach_window(self->source, self);

    xcb_destroy_window(g_xcb_source_get_connection(self->source), self->window);

    g_free(self);
}

xcb_window_t
g_xcb_window_get_window(GXcbWindow *self)
{
    return self->window;
}

void
g_xcb_window_set_expose_event_callback(GXcbWindow *window, GXcbWindowExposeEventCallback callback)
{
    window->expose_event_callback = callback;
}

void
g_xcb_window_set_button_press_event_callback(GXcbWindow *window, GXcbWindowButtonPressEventCallback callback)
{
    window->button_press_event_callback = callback;
}

void
g_xcb_window_set_button_release_event_callback(GXcbWindow *window, GXcbWindowButtonReleaseEventCallback callback)
{
    window->button_release_event_callback = callback;
}

void
g_xcb_window_expose_event(GXcbWindow *window, xcb_expose_event_t *event)
{
    if ( window->expose_event_callback != NULL )
        window->expose_event_callback(window, event, window->user_data);
}

void
g_xcb_window_button_press_event(GXcbWindow *window, xcb_button_press_event_t *event)
{
    if ( window->button_press_event_callback != NULL )
        window->button_press_event_callback(window, event, window->user_data);
}

void
g_xcb_window_button_release_event(GXcbWindow *window, xcb_button_release_event_t *event)
{
    if ( window->button_release_event_callback != NULL )
        window->button_release_event_callback(window, event, window->user_data);
}
