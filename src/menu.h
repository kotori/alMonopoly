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

/*! \class MonopolyMenu
    \brief Menu class for providing a simple GUI

    This header provides a means for rapidly building a simple gui out of database entries.
    \see MonopolyMenuEntry
*/
class MonopolyMenu {

public:

    /*! \fn MonopolyMenu()
	\brief Menu class constructor.
    */
    MonopolyMenu();

    /*! \fn ~MonopolyMenu()
	\brief Menu class destructor.
    */
    ~MonopolyMenu();

    /*! \fn void draw()
	\brief Routine to print this menu to the screen. This function will handle
          printing a colored font depending on the selected menu entry.
    */
    void draw();

    /*! \fn void set_id(int id)
	\brief Set this menu's unique ID.
	\param id integer representing this menu's ID code.
    */
    void set_id(int id);

    /*! \fn int get_id()
	\brief Set this menu's unique ID.
	\return Integer representing this menu's ID code.
    */
    int get_id();

    /*! \fn void set_selection(int sel)
	\brief Set this menu's currently selected entry.
	\param sel integer representing this menu's currently selected entry.
    */
    void set_selection(int sel);

    /*! \fn int get_selection()
	\brief Get this menu's currently selected entry.
	\return Integer representing this menu's currently selected entry.
    */
    int get_selection();

    /*! \fn int build(int menu_id)
	\brief The build() routine will take a list of database entries and build a complete menu
	\param menu_id This is a menu's unique ID code. No other menu should share this code.
	\return 0 on success, anything else on failure.
    */
    int build(int menu_id);

    /*! \fn void cleanup()
	\brief Release any stored resources.
    */
    void cleanup();

private:

    Database sqlConn; /*!< SQLite3 database connection handle. */
    int _id; /*!< Menu's unique ID. */				
    int _selection; /*!< Menu entry that is currently selected. */
    int _entryCount; /*!< Number of entries in this menu. */
    ALLEGRO_FONT *_fontFile; /*!< Font to use when drawing this menu. */
    std::vector <MonopolyMenuEntry> _entries; /*!< Group of entries that make us this menu. */
};

#endif // MENU_H

