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
    /*! \fn MonopolyMenuEntry()
	\brief Class constructor.
    */
    MonopolyMenuEntry();

    /*! \fn ~MonopolyMenuEntry()
	\brief Class destructor.
    */
    ~MonopolyMenuEntry();

    /*! \fn void set_text(std::string text)
	\brief Set the text value of this menu entry.
        \param text std::string representing this entry's text.
    */
    void set_text(std::string text);

    /*! \fn  std::string get_text()
	\brief Get the text value of this menu entry.
        \return std::string value representing this entry's text.
    */
    std::string get_text();

    /*! \fn void draw(ALLEGRO_FONT *f)
	\brief Draw this entry to the screen using the passed font file.
        \param f ALLEGRO_FONT pointer to use to print the text to the screen.
    */
    void draw(ALLEGRO_FONT *f);

private:

    std::string _text;	/*!< std::string value representing this entries text. */
    int _id;		/*!< This entry's unique ID code. */
    float _x;		/*!< X position of this entry. */
    float _y;		/*!< Y position of this entry. */
    bool _isSelected;	/*!< Selection flag. */
};

#endif // ENTRY_H

