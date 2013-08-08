
#ifndef COMMON_H
#define COMMON_H

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define MAX_FPS		60.0
#define MAX_FRAME_FPS	15.0
#define MAX_PROPERTIES	40
#define NUM_PLAYERS		2

#define DB_PROPERTY_TABLE	"landing"

enum GameMode { EASY, NORMAL, DIFFICULT };
enum Direction { DOWN, LEFT, RIGHT, UP };
enum TurnState { PRE_TURN, ROLL_PHASE, MOVE_PHASE, REACT_PHASE, POST_TURN };

#endif // COMMON_H

