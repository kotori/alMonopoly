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
#include <vector>

#include "database.h"
#include "entry.h"
#include "menu.h"

MonopolyMenu::MonopolyMenu() {
    m_entryCount = 0;
    m_id = 0;
    m_selection = 0;
    m_fontFile = NULL;
}

MonopolyMenu::~MonopolyMenu() {
    m_entries.clear();
}

int MonopolyMenu::build(int menu_id) {
    // Assign the ID to our new menu.
    m_id = menu_id;
    // Set some default values.
    m_entryCount = 0;
    std::string tempQuery = "";

    tempQuery = m_sqlConn.Format( "SELECT COUNT(*) FROM %s WHERE %s = %i", "menu", "menu_id", menu_id );
    if( m_sqlConn.SelectInt( m_entryCount, tempQuery.c_str() ) ) {
        fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
        return -1;
    }

    // Now ensure that before we proceed we actually have a number of entries to parse.
    if(m_entryCount <= 0) {
        fprintf( stderr, "No Entries exist for this menu!\n" );
        return -1;
    }

    // Menu entry object instance.
    MonopolyMenuEntry *newEntry;
    for( int eCounter = 0; eCounter < m_entryCount; eCounter++ ) {

        // Create a new object pointer that we will build and then insert into the vector.
        newEntry = new MonopolyMenuEntry();

        // SELECT `title` FROM `menu` WHERE `menu_id` == `menu_id` AND `id` == `eCounter`;
        tempQuery = m_sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s = %i",
                                    "title", "menu", "menu_id", menu_id, "id", eCounter );
        // Assign the value just pulled from the database to our object.
        std::string entryName;
        if( m_sqlConn.SelectStr( entryName, tempQuery.c_str() ) ) {
            fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
            return -1;
        }
        else {
            newEntry->set_text( entryName );
        }

        // Now that the entry has been built, insert it into our menu.
        m_entries.push_back( *newEntry );

        // DEBUG: print value just added.
        fprintf( stderr, "Added new Menu Entry: [%i] %s\n", eCounter, newEntry->get_text().c_str() );
        fprintf( stderr, "Added new Menu entry: [%i] %s\n", eCounter, m_entries[eCounter].get_text().c_str() );
    }

    return 0;
}

void MonopolyMenu::cleanup() {
    if(m_fontFile) {
        m_fontFile = NULL;
        al_destroy_font(m_fontFile);
    }
}

void MonopolyMenu::draw() {
    // Iterate through our entry vector and draw its associated entries.
    std::vector <MonopolyMenuEntry>::iterator it;

    for( it = m_entries.begin(); it != m_entries.end(); ++it ) {
        it->draw( m_fontFile );
    }
}

void MonopolyMenu::set_selection(int sel) {
    m_selection = sel;
}

int MonopolyMenu::get_selection() {
    return m_selection;
}

void MonopolyMenu::set_id(int id) {
    m_id = id;
}

int MonopolyMenu::get_id() {
    return m_id;
}

