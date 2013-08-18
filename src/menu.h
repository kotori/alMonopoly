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

/*! \file menu.h
    \brief Menu class header file.

    This header provides a wrapper around creating quick text-based menus.
*/

#ifndef MENU_H
#define MENU_H

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "database.h"

class MonopolyMenu {

public:
    MonopolyMenu();
    ~MonopolyMenu();

    void draw();

    void set_id(int id);
    int get_id();

    void set_selection(int sel);
    int get_selection();

    int build(int menu_id);

    void cleanup();

private:

    Database sqlConn;
    int _id;
    int _selection;
    int _entryCount;
    ALLEGRO_FONT *_fontFile;
    std::vector <MonopolyMenuEntry> _entries;
};

#endif // MENU_H

