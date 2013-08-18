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

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "database.h"
#include "entry.h"

MonopolyMenuEntry::MonopolyMenuEntry() {
	_isSelected = false;
    _id = 0;
    _text = "";
    _x = 0.0;
    _y = 0.0;
}

MonopolyMenuEntry::~MonopolyMenuEntry() {
    //
}

void MonopolyMenuEntry::set_text(std::string text) {
    _text = text;
}

std::string MonopolyMenuEntry::get_text() {
    return _text;
}

void MonopolyMenuEntry::draw(ALLEGRO_FONT *f) {
    if(_isSelected) {
        // If this entry is selected, print the highlighted text.
        al_draw_text( f, al_map_rgb(0, 0, 0), _x + 2, _y+2, 0, _text.c_str() );
        al_draw_text( f, al_map_rgb(250, 0, 0), _x, _y, 0, _text.c_str() );
    }
    else {
        // If the entry is not selected, print standard text.
        al_draw_text( f, al_map_rgb(0, 0, 0), _x + 2, _y+2, 0, _text.c_str() );
        al_draw_text( f, al_map_rgb(255, 255, 255), _x, _y, 0, _text.c_str() );
    }
}

