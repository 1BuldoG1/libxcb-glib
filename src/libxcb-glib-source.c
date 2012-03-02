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
#include <libxcb-glib-window.h>
#include "libxcb-glib-window-internal.h"
#include <libxcb-glib-source.h>
#include "libxcb-glib-source-internal.h"

struct _GXcbSource {
    GSource source;
    gboolean connection_owned;
    xcb_connection_t *connection;
    GPollFD fd;
    GQueue *queue;
    GHashTable *windows;
};

static void
_g_xcb_source_event_free(gpointer data, gpointer user_data)
{
    free(data);
}

static gboolean
_g_xcb_source_prepare(GSource *source, gint *timeout)
{
    GXcbSource *self = (GXcbSource *)source;
    *timeout = -1;
    return ! g_queue_is_empty(self->queue);
}

static gboolean
_g_xcb_source_check(GSource *source)
{
    GXcbSource *self = (GXcbSource *)source;

    if ( self->fd.revents & G_IO_IN )
    {
        xcb_generic_event_t *event;

        while ( ( event = xcb_poll_for_event(self->connection) ) != NULL )
            g_queue_push_tail(self->queue, event);
    }

    return ! g_queue_is_empty(self->queue);
}

static gboolean
_g_xcb_source_dispatch(GSource *source, GSourceFunc callback, gpointer user_data)
{
    GXcbSource *self = (GXcbSource *)source;
    xcb_generic_event_t *event;

    event = g_queue_pop_head(self->queue);
    switch ( event->response_type & ~0x80 )
    {
    case XCB_EXPOSE:
    {
        xcb_expose_event_t *e = (xcb_expose_event_t *)event;
        GXcbWindow *window;

        window = g_hash_table_lookup(self->windows, GUINT_TO_POINTER(e->window));
        if ( window != NULL )
            g_xcb_window_expose_event(window, e);
    }
    break;
    case XCB_BUTTON_PRESS:
    {
        xcb_button_press_event_t *e = (xcb_button_press_event_t *)event;
        GXcbWindow *window;

        window = g_hash_table_lookup(self->windows, GUINT_TO_POINTER(e->event));
        if ( window != NULL )
            g_xcb_window_button_press_event(window, e);
    }
    break;
    case XCB_BUTTON_RELEASE:
    {
        xcb_button_release_event_t *e = (xcb_button_release_event_t *)event;
        GXcbWindow *window;

        window = g_hash_table_lookup(self->windows, GUINT_TO_POINTER(e->event));
        if ( window != NULL )
            g_xcb_window_button_release_event(window, e);
    }
    break;
    default:
    break;
    }
    _g_xcb_source_event_free(event, NULL);

    return TRUE;
}

static void
_g_xcb_source_finalize(GSource *source)
{
    GXcbSource *self = (GXcbSource *)source;

    g_hash_table_unref(self->windows);
    g_queue_foreach(self->queue, _g_xcb_source_event_free, NULL);
    g_queue_free(self->queue);

    if ( self->connection_owned )
        xcb_disconnect(self->connection);
}

static GSourceFuncs _g_xcb_source_funcs = {
    _g_xcb_source_prepare,
    _g_xcb_source_check,
    _g_xcb_source_dispatch,
    _g_xcb_source_finalize
};

GXcbSource *
g_xcb_source_new(GMainContext *context, const gchar *display, gint *screen)
{
    xcb_connection_t *connection;
    GXcbSource *source;

    connection = xcb_connect(display, screen);
    if ( xcb_connection_has_error(connection) )
    {
        xcb_disconnect(connection);
        return NULL;
    }

    source = g_xcb_source_new_for_connection(context, connection);
    source->connection_owned = TRUE;
    return source;
}

GXcbSource *
g_xcb_source_new_for_connection(GMainContext *context, xcb_connection_t *connection)
{
    GXcbSource *source;

    source = (GXcbSource *)g_source_new(&_g_xcb_source_funcs, sizeof(GXcbSource));

    source->connection = connection;
    source->queue = g_queue_new();
    source->windows = g_hash_table_new(NULL, NULL);

    source->fd.fd = xcb_get_file_descriptor(connection);
    source->fd.events = G_IO_IN;

    g_source_add_poll((GSource *)source, &source->fd);
    g_source_attach((GSource *)source, context);

    return source;
}

void
g_xcb_source_ref(GXcbSource *self)
{
    g_source_ref((GSource *)self);
}

void
g_xcb_source_unref(GXcbSource *self)
{
    g_source_unref((GSource *)self);
}

xcb_connection_t *
g_xcb_source_get_connection(GXcbSource *self)
{
    return self->connection;
}

void
g_xcb_source_attach_window(GXcbSource *self, GXcbWindow *window)
{
    g_hash_table_insert(self->windows, GUINT_TO_POINTER(g_xcb_window_get_window(window)), window);
}

void
g_xcb_source_detach_window(GXcbSource *self, GXcbWindow *window)
{
    g_hash_table_remove(self->windows, GUINT_TO_POINTER(g_xcb_window_get_window(window)));
}
