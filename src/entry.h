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

/*! \file entry.h
    \brief Menu entry class header file.

    This header provides the entry object which when grouped together can be drawn as a menu.
*/

#ifndef ENTRY_H
#define ENTRY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class MonopolyMenuEntry {

public:
    MonopolyMenuEntry();
    ~MonopolyMenuEntry();

    void set_text(std::string text);
    std::string get_text();

    void draw(ALLEGRO_FONT *f);

private:

    std::string _text;
    int _id;
    float _x;
    float _y;
    bool _isSelected;
};

#endif // ENTRY_H

