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

#ifndef COMMON_H
#define COMMON_H

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define MAX_FPS		60.0
#define MAX_FRAME_FPS	15.0
#define MAX_PROPERTIES	40
#define NUM_PLAYERS	2

#define DB_PROPERTY_TABLE	"landing"

enum GameMode { EASY, NORMAL, DIFFICULT };
enum Direction { DOWN, LEFT, RIGHT, UP };
enum TurnState { PRE_TURN, ROLL_PHASE, MOVE_PHASE, REACT_PHASE, POST_TURN };
enum Positions { X_POS = 0, Y_POS = 1 };

#endif // COMMON_H

