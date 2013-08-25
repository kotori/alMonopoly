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

#include "game.h"

int main(int argc, char **argv) {

    // Create new instance of the MonopolyGame object.
    MonopolyGame *monopoly = new MonopolyGame();

    // Initialize the Allegro routines
    if(monopoly->init()) {
    	delete monopoly;
        return -1;
    }

    // Load Resources
    if(monopoly->loadResources()) {
    	fprintf( stderr, "Failed loading the necessary resources!\nExiting Now!\n" );
        monopoly->halt();
        delete monopoly;
        return -1;
    }

    // Run the game loop until the exit condition is met.
    if(monopoly->run()) {
    	fprintf( stderr, "A fatal gameplay error has occurred!\nExiting Now!\n" );
        monopoly->halt();
        delete monopoly;
        return -1;
    }

    // Cleanup when we are done.
    monopoly->halt();

    delete monopoly;

    return 0;
}

