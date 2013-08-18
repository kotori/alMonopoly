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

	/*! \fn MonopolyGame()
        \brief Class constructor.
	*/
    MonopolyGame();

    /*! \fn ~MonopolyGame()
		\brief Class destructor.
	*/
    ~MonopolyGame();

    /*! \fn int init()
		\brief Initialize the Allegro requirements.
		\return 0 on success, anything else on failure.
	*/
    int init();

    /*! \fn int loadResources()
		\brief Load gfx/sfx resources.
		\return 0 on success, anything else on failure.
	*/
    int loadResources();

    /*! \fn int run()
		\brief Start the game loop.
		\return 0 on success, anything else on failure.

		This loop will continue to run until the exit condition is met.
		 The exit condition is MonopolyGame::exitGame.
	*/
    int run();

    /*! \fn void halt()
		\brief Cleanup the game's resources.
	*/
    void halt();

    /*! \fn int randomNum(int max)
		\brief Generate a random number.
		\param max Integer representing the ceiling for generation.
		\return A random integer between 1 and \b max.
	*/
    int randomNum(int max);

private:

    /*! \struct alCamera
        \brief Structure representing the viewable area of the screen.
     */
    struct alCamera {
    	float cameraPosition[2]; /*!< X/Y axis position of the camera. */
    	ALLEGRO_TRANSFORM alCameraTransform; /*!< Allegro5 transform flag. */
    }alCamera; /*!< Camera instance. */

    /*! \fn bool mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr)
        \brief Routine for handling the mortgaging of a property.
        \param prop Property to be purchased.
        \param plyr Player wishing to purchase the property.
        \return true on success, false on failure.

        This function will handle the mortgaging of a property. First we check ownership.
         If this passes then the property's mortgage value is added to the player's cash supply
         and the mortgage and ownership flags are changed.
    */
    bool mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr);

    /*! \fn bool purchaseProperty(MonopolyProperty &prop, MonopolyPlayer &plyr, int priceMod)
        \brief Routine for handling the sale of a property.
        \param prop Property to be purchased.
        \param plyr Player wishing to purchase the property.
        \param priceMod Any price bonsues this player might get when purchasing.
        \return true on success, false on failure.

        This function will first check if the property is already owned, if not, it will then
         figure the purchase price minus the passed modifier. This final cost if subtracted from
         the player's money and the owned flag is set to true. True is returned on success, false on failure.
    */
    bool purchaseProperty(MonopolyProperty &prop, MonopolyPlayer &plyr, int priceMod);

    /*! \fn void draw()
        \brief Redrawn the screen.

        Called once per frame to refresh the screen with updates. \see MAX_FPS
     */
    void draw();

    /*! \fn void handleTurn()
        \brief Perform the necessary routines to get a player through one complete turn.
     */
    void handleTurn();

    /*! \fn void void handleMove()
        \brief Handle moving a player's icon around the board.

        This function will either be called in response to a dice roll or a card draw.
     */
    void handleMove();

    /*! \fn void reset()
        \brief Completely reset the game back to its initial values.
     */
    void reset();

    /*! \fn void drawText(ALLEGRO_COLOR col, int x, int y, const char *msg, ...)
        \brief Print a formatted message on the screen.
        \param col ALLEGRO_COLOR representing the text's foreground color.
        \param x Begin writing the text at this X-axis location.
        \param y Begin writing the text at this Y-axis location.
        \param msg Formatted string representing the message to be displayed.
     */
    void drawText(ALLEGRO_COLOR col, int x, int y, const char *msg, ...);

    /*! \fn void drawTextCen(ALLEGRO_COLOR col, int x, int y, const char *msg, ...)
        \brief Print a formatted message centered on the screen.
        \param col ALLEGRO_COLOR representing the text's foreground color.
        \param x Begin writing the text at this X-axis location.
        \param y Begin writing the text at this Y-axis location.
        \param msg Formatted string representing the message to be displayed.
     */
    void drawTextCen(ALLEGRO_COLOR col, int x, int y, const char *msg, ...);

    /*! \fn void cameraUpdate(float *cameraPosition, float x, float y, int width, int height)
        \brief Update the camera's position on the screen.
        \param cameraPosition Float integer array representing the location of the camera.
        \param x X-axis location of the active player to center the camera on.
        \param y Y-axis location of the active player to center the camera on.
        \param width Active player's bitmap image width.
        \param height Active player's bitmap image height.
     */
    void cameraUpdate(float *cameraPosition, float x, float y, int width, int height);

    Database sqlConn; /*!< SQLite3 connection. */

    /*! \fn int buildPropertyList()
        \brief Build the property list.
        \return 0 on success, anything else on failure.

        This function is called to populate the property list from database entries.
     */
    int buildPropertyList();

    MonopolyProperty propertyList[MAX_PROPERTIES]; /*!< Maximum number of properties. */

    MonopolyPlayer playerList[NUM_PLAYERS];		/*!< Group of player objects. */

    ALLEGRO_DISPLAY			*alDisplayDevice;	/*!< Allegro display device. */
    ALLEGRO_EVENT_QUEUE		*alEventQueue;		/*!< Allegro event queue. */
    ALLEGRO_TIMER			*alTimer;			/*!< Allegro FPS timer. */
    ALLEGRO_TIMER			*alFrameTimer;		/*!< Allegro Animation FPS timer. */
    ALLEGRO_DISPLAY_MODE	alDisplayData;		/*!< Allegro display data. */
    ALLEGRO_KEYBOARD_STATE 	alKeyState;			/*!< Current state of the keyboard. */
    ALLEGRO_FONT			*fontCollection[3];	/*!< Collection of Allegro FONTs. */

    ALLEGRO_BITMAP 			*alBoardImage;	/*!< Allegro BITMAP representing the game board. */

    int playersTurn; 		/*!< Current player's turn. */
    int numPlayers;		/*!< Number of players. */

    GameMode activeGameMode;	/*!< Tracks the active game mode. */
    TurnState turnState;		/*!< Tracks the current turn state. */

    int doublesRollCounter; /*!< Keeps track the number of times a double was rolled. */

    bool exitGame;	/*!< Boolean flag for exiting the main loop. */
    bool redrawScreen;	/*!< Boolean flag for handling screen redraws. */
    bool isActive;	/*!< Boolean flag for handling animation renders. */
};

#endif // GAME_H
