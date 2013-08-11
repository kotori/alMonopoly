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

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "common.h"
#include "database.h"
#include "property.h"
#include "player.h"

class MonopolyGame {

public:
    MonopolyGame();
    ~MonopolyGame();

    int init();
    int loadResources(); /*!< Ensure's all gfx & sfx resources are loaded. */
    int run();
    void halt();

    int randomNum(int max);

private:
    void draw();
    void handleTurn(int playerId);
    void handleMove();
    void reset();
    
    void drawText(int x, int y, const char *msg, ...);

    void cameraUpdate(float *cameraPosition, float x, float y, int width, int height);
    
    Database sqlConn;

    int buildPropertyList(); /*!< Populates the propertyList. */
    MonopolyProperty propertyList[MAX_PROPERTIES]; /*!< Maximum number of properties. */

    MonopolyPlayer playerList[NUM_PLAYERS];

    // Allegro5 objects.
    ALLEGRO_DISPLAY			*alDisplayDevice;
    ALLEGRO_EVENT_QUEUE		*alEventQueue;
    ALLEGRO_TIMER			*alTimer;
    ALLEGRO_TIMER			*alFrameTimer;
    ALLEGRO_DISPLAY_MODE	alDisplayData;
    ALLEGRO_KEYBOARD_STATE 	alKeyState;
    ALLEGRO_TRANSFORM		alCameraTransform;
    ALLEGRO_FONT			*fontCollection[3];

    ALLEGRO_BITMAP 			*alBoardImage;

    // Actual game objects.
    int playersTurn; 		/*!< Current player's turn. */
    int numPlayers;		/*!< Number of players. */

    GameMode activeGameMode;
    TurnState turnState;

    int doublesRollCounter; /*!< Keeps track the number of times a double was rolled. */

    bool exitGame;	/*!< Boolean flag for exiting the main loop. */
    bool redrawScreen;	/*!< Boolean flag for handling screen redraws. */
    bool isActive;	/*!< Boolean flag for handling animation renders. */
};

#endif // GAME_H
