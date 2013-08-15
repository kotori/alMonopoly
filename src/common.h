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

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define MAX_FPS		60.0
#define MAX_FRAME_FPS	15.0
#define MAX_PROPERTIES	40
#define NUM_PLAYERS	2

#define DB_PROPERTY_TABLE	"landing"

/*! \enum GameMode
    \brief List of possible game modes.
*/
enum GameMode {
    EASY,	/*!< Easy difficulty game. */
    NORMAL,	/*!< Normal difficulty game. */
    DIFFICULT	/*!< Hard difficulty game. */
};

/*! \enum layerType
    \brief List of possible player types.
*/
enum PlayerType {
    COMPUTER = 0,	/*!< Computer controlled player. */
    PLAYER = 1		/*!< Human player. */
};

enum Direction {
    DOWN,
    LEFT,
    RIGHT,
    UP
};

enum TurnState {
	NULL_STATE,
	PRE_GAME,
	PRE_TURN,
	ROLL_PHASE,
	MOVE_PHASE,
	REACT_PHASE,
	POST_TURN,
	TRADING
};

enum Positions {
    X_POS = 0,
    Y_POS = 1
};

enum PropertyType {
    TYPE_NULL = 1,
    TYPE_RAILROAD = 2,
    TYPE_UTILITY = 3,
    TYPE_PROPERTY = 4,
    TYPE_CARD = 5,
    TYPE_TAX = 6
};

#endif // COMMON_H
