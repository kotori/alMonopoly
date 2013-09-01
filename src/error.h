/*
* Copyright (C) 2013 Kotori <https://github.com/kotori>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file error.h
    \brief Error handling header file.

    This header provides a single error handling function.
*/

#ifndef _al_error_h_
#define _al_error_h_

#include <stdio.h>
#include <allegro5/allegro_native_dialog.h>

// usage: error(AT, display, "your message here");
#define AT __func__,__LINE__
void error(const char *func, int line, ALLEGRO_DISPLAY *display, const char *message);

#endif
