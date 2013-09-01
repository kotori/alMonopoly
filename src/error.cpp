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

#include "error.h"

void error(const char *func, int line, ALLEGRO_DISPLAY *display, const char *message)
{
   char text[256];

   sprintf(text, "%s(%d): %s", func, line, message);
   al_show_native_message_box(display, "Error", NULL, text,
                              NULL, ALLEGRO_MESSAGEBOX_ERROR);
}
