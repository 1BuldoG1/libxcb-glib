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

#ifndef __LIBXCB_GLIB_SOURCE_H__
#define __LIBXCB_GLIB_SOURCE_H__

G_BEGIN_DECLS

GXcbSource *g_xcb_source_new(GMainContext *context, const gchar *display, gint *screen);
GXcbSource *g_xcb_source_new_for_connection(GMainContext *context, xcb_connection_t *connection);
void g_xcb_source_ref(GXcbSource *self);
void g_xcb_source_unref(GXcbSource *self);

xcb_connection_t *g_xcb_source_get_connection(GXcbSource *source);

G_END_DECLS

#endif /* __LIBXCB_GLIB_SOURCE_H__ */
