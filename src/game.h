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

/*! \file game.h
    \brief Game class header file.

    This header provides a class responsible for managing nearly every aspect of gameplay.
     This includes, start, stop, draw, load, and clean up of the game's resources.
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

/*! \class MonopolyGame
    \brief Game class
    This class is used manage the various other classes as well as the Allegro library.
*/

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

    /*! \struct alCamera
        \brief Structure representing the viewable area of the screen.
     */
    struct alCamera {
    	float cameraPosition[2]; /*!< X/Y axis position of the camera. */
    	ALLEGRO_TRANSFORM alCameraTransform; /*!< Allegro5 transform flag. */
    }alCamera;

    /*! \fn bool mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr)
        \brief Routine for handling the mortgaging of a property.

        This function will handle the mortgaging of a property. First we check ownership.
         If this passes then the property's mortgage value is added to the player's cash supply
         and the mortgage and ownership flags are changed.
    */
    bool mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr);

    /*! \fn bool purchaseProperty(MonopolyProperty &prop, MonopolyPlayer &plyr, int priceMod)
        \brief Routine for handling the sale of a property.

        This function will first check if the property is already owned, if not, it will then
         figure the purchase price minus the passed modifier. This final cost if subtracted from
         the player's money and the owned flag is set to true. True is returned on success, false on failure.
    */
    bool purchaseProperty(MonopolyProperty &prop, MonopolyPlayer &plyr, int priceMod);

    void draw();

    void handleTurn();

    void handleMove();

    void reset();

    void drawText(ALLEGRO_COLOR col, int x, int y, const char *msg, ...);
    void drawTextCen(ALLEGRO_COLOR col, int x, int y, const char *msg, ...);

    void cameraUpdate(float *cameraPosition, float x, float y, int width, int height);

    Database sqlConn; /*!< SQLite3 connection. */

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
