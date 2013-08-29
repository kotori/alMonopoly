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

/*! \file common.h
    \brief Common header file

    This header provides several global variables/enumerations. This includes
     The screen resolution, number of players, max FPS, etc.
*/

#ifndef COMMON_H
#define COMMON_H

/*! \def WINDOW_WIDTH
    \brief Global containing the maximum size of the window's width.
*/
#define WINDOW_WIDTH	800

/*! \def WINDOW_HEIGHT
    \brief Global containing the maximum size of the window's height.
*/
#define WINDOW_HEIGHT	600

/*! \def MAX_FPS
    \brief Global containing the maximum number of frames per second.
*/
#define MAX_FPS		60.0

/*! \def MAX_FRAME_FPS
    \brief Global containing the maximum number of frames per second for animations.
*/
#define MAX_FRAME_FPS	15.0

/*! \def MAX_PROPERTIES
    \brief Global containing the maximum number of properties to load from our database.
*/
#define MAX_PROPERTIES	40

/*! \def NUM_PLAYERS
    \brief Global containing the maximum number of players to initialize.
*/
#define NUM_PLAYERS	2

/*! \def MAX_FONTS
    \brief Global containing the maximum number of font copies to make.
*/
#define MAX_FONTS 3

/*! \def DB_PROPERTY_TABLE
    \brief Global containing the name of the property table.
*/
#define DB_PROPERTY_TABLE	"landing"

/*! \def DB_GROUP_TABLE
    \brief Global containing the name of the property group sets table.
*/
#define DB_GROUP_TABLE		"sets"

/*! \def DB_PIECES_TABLE
    \brief Global containing the name of the player pieces table.
*/
#define DB_PIECES_TABLE		"pieces"

/*! \def PLAYER_PIECES_COUNT
    \brief Number of piece to load into memory.
*/
# define PLAYER_PIECES_COUNT 6

/*! \def NUM_DICE
    \brief Number of dice to throw at a time.
*/
#define NUM_DICE 2

/*! \def NUM_SIDES_PER_DIE
    \brief Number of sides per die.
*/
#define NUM_SIDES_PER_DIE 6

/*! \enum PlayerPiece
    \brief List of different game pieces a player can choose from.
*/
enum PlayerPiece {
	PIECE_BOAT = 0,		/*!< The Battleship. */
	PIECE_HAT = 1,		/*!< The Tophat. */
	PIECE_DOG = 2,		/*!< The Dog. */
	PIECE_THIMBLE = 3,	/*!< The Thimble. */
	PIECE_IRON = 4,		/*!< The Iron. */
	PIECE_CAR = 5		/*!< The Car. */
};


/*! \enum GameMode
    \brief List of possible game modes.
*/
enum GameMode {
    EASY = 0,		/*!< Easy difficulty game. */
    NORMAL = 1,		/*!< Normal difficulty game. */
    DIFFICULT = 2	/*!< Hard difficulty game. */
};

/*! \enum PlayerType
    \brief List of possible player types.
*/
enum PlayerType {
    COMPUTER = 0,	/*!< Computer controlled player. */
    PLAYER = 1		/*!< Human player. */
};

/*! \enum Direction
    \brief List of possible directions a player can face.
*/
enum Direction {
    DOWN = 0,		/*!< Facing down. */
    LEFT = 1,		/*!< Facing left. */
    RIGHT = 2,		/*!< Facing right. */
    UP = 3		/*!< Facing up. */
};

/*! \enum TurnState
    \brief List of states the game can be in.
*/
enum TurnState {
	NULL_STATE = 0,	/*!< NULL state, this is used on startup. */
	PRE_GAME = 1,	/*!< Pre-Game state, used just after startup. */
	PRE_TURN = 2,	/*!< Pre-turn state, any actions that should be player before the active player's roll. */
	ROLL_PHASE = 3,	/*!< Rolling state, at this time we should display the rolling interface. */
	MOVE_PHASE = 4,	/*!< Movement phase, this is called in response to a dice roll or card event. */
	REACT_PHASE = 5,/*!< Reaction phase, this is called in response to a movement or card event. */	
	POST_TURN = 6,	/*!< Post turn state, this is called for any post-turn cleanup events. */
	TRADING = 7,	/*!< Trading state, this is called in response to a Player bid to trade. */
};

/*! \enum Positions
    \brief List of possible axis positions.
*/
enum Positions {
    X_POS = 0,	/*!< X-axis position. */
    Y_POS = 1	/*!< Y-axis position. */
};

/*! \enum PropertyType
    \brief List of possible property types.

    Property's are 'spaces' on around the board. Each space can be represented by
     one of these types.
*/
enum PropertyType {
    TYPE_NULL = 1,	/*!< Null property type. */
    TYPE_RAILROAD = 2,	/*!< Railroad property type. */
    TYPE_UTILITY = 3,	/*!< Utility property type. */
    TYPE_PROPERTY = 4,	/*!< Property than can be sold then built upon. */
    TYPE_CARD = 5,	/*!< This is an event card property. */
    TYPE_TAX = 6	/*!< This is an tax property type. */
};

/*! \enum PropertyValue
    \brief List of possible property value types.
*/
enum PropertyValue {
    VAL_NULL = 0,		/*!< Null, unowned property. */
    VAL_OWNED = 1,		/*!< Owned property, not a complete set. */
    VAL_OWNED_SET = 2,	/*!< Owned property, complete set. */
    VAL_1_HOUSE = 3,	/*!< Improved property, with 1 house. */
    VAL_2_HOUSE = 4,	/*!< Improved property, with 2 houses. */
    VAL_3_HOUSE = 5,	/*!< Improved property, with 3 houses. */
    VAL_4_HOUSE = 6,	/*!< Improved property, with 4 houses. */
    VAL_1_HOTEL = 7,	/*!< Improved property, with 1 hotel. */
    VAL_MORTGAGED = 8	/*!< Mortgaged property. */
};

#endif // COMMON_H
