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

#if ! defined(__LIBXCB_GLIB_H_INSIDE__) && ! defined(LIBXCB_GLIB_COMPILATION)
#error "Only <libxcb-glib.h> can be included directly."
#endif

#ifndef __LIBXCB_GLIB_WINDOW_H__
#define __LIBXCB_GLIB_WINDOW_H__

G_BEGIN_DECLS

GXcbWindow *g_xcb_window_new(GXcbSource *source, guint8 depth, xcb_window_t parent, gint16 x, gint16 y, guint16 width, guint16 height, guint16 border_width, guint16 _class, xcb_visualid_t visual, guint32 value_mask, const guint32 *value_list, gpointer user_data);
void g_xcb_window_free(GXcbWindow *window);

xcb_window_t g_xcb_window_get_window(GXcbWindow *window);

typedef void (*GXcbWindowExposeEventCallback)(GXcbWindow *window, xcb_expose_event_t *event, gpointer user_data);
typedef void (*GXcbWindowButtonPressEventCallback)(GXcbWindow *window, xcb_button_press_event_t *event, gpointer user_data);
typedef void (*GXcbWindowButtonReleaseEventCallback)(GXcbWindow *window, xcb_button_release_event_t *event, gpointer user_data);

void g_xcb_window_set_expose_event_callback(GXcbWindow *window, GXcbWindowExposeEventCallback callback);
void g_xcb_window_set_button_press_event_callback(GXcbWindow *window, GXcbWindowButtonPressEventCallback callback);
void g_xcb_window_set_button_release_event_callback(GXcbWindow *window, GXcbWindowButtonReleaseEventCallback callback);

G_END_DECLS

#endif /* __LIBXCB_GLIB_WINDOW_H__ */
