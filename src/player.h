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

/*! \file player.h
    \brief Player class header file.

    This header provides an interface for building a character to play with or against.
     This includes the player's image, x/y coords, and various activity flags.
*/

#ifndef PLAYER_H
#define PLAYER_H

/*! \class MonopolyPlayer
    \brief Player class
    This class is used to build up a player.
*/
class MonopolyPlayer {
public:

    /*! \fn MonopolyPlayer()
        \brief Class constructor.
    */
    MonopolyPlayer();

    /*! \fn ~MonopolyPlayer()
        \brief Class destructor.
    */
    ~MonopolyPlayer();

    /*! \fn bool get_firstRoll()
        \brief Get true or false based on if this is the player's first roll this turn.
        \return True if this is the player's first roll.
    */
    bool get_firstRoll();

    /*! \fn void set_firstRoll(bool firstRoll)
        \brief Set true or false based on if this is the player's first roll this turn.
        \param firstRoll True if this is the player's first roll.
    */
    void set_firstRoll(bool firstRoll);

    /*! \fn bool get_isAlive()
        \brief Get true or false based on if this is the player is still in the game.
        \return True if this is capable of continue playing.
    */
    bool get_isAlive();

    /*! \fn void set_isAlive(bool alive)
        \brief Set true or false based on if this is the player is still in the game.
        \param alive True if this is capable of continue playing.
    */
    void set_isAlive(bool alive);

    /*! \fn bool get_passedGo()
        \brief Get true or false based on if this player has just passed GO.
        \return True if the player passed GO, false if not.
    */
    bool get_passedGo();

    /*! \fn void set_passedGo(bool passedGo)
        \brief Set true or false based on if this player has just passed GO.
        \param passedGo True if the player passed GO, false if not.
    */
    void set_passedGo(bool passedGo);

    /*! \fn int get_id()
        \brief Get the player's ID.
        \return Integer representing this player's ID.
    */
    int get_id();

    /*! \fn void set_id(int id)
        \brief Set the player's ID.
        \param id Integer representing this player's ID.
    */
    void set_id(int id);

    /*! \fn int get_money()
        \brief Get the player's money value.
        \return Integer representing this player's cash.
    */
    int get_money();

    /*! \fn int set_money()
        \brief Set the player's money value.
        \param money Integer representing this player's cash.
    */
    void set_money(int money);

    /*! \fn int get_score()
        \brief Get the player's score.
        \return Integer representing this player's score
    */
    int get_score();

    /*! \fn void set_score(int score)
        \brief Set the player's score.
        \param score Integer representing this player's score.
    */
    void set_score(int score);

    /*! \fn int get_location()
        \brief Get the player's current location represented as a single integer.
        \return Integer representing this player's location.
    */
    int get_location();

    /*! \fn void set_location(int location)
        \brief Set the player's current location represented as a single integer.
        \param location Integer representing this player's location.
    */
    void set_location(int location);

    /*! \fn float get_x()
        \brief Get the player's current location represented as an X coordinate.
        \return Float number representing this player's location.
    */
    float get_x();

    /*! \fn void set_x(float x)
        \brief Set the player's current location represented as an X coordinate.
        \param x Float number representing this player's location.
    */
    void set_x(float x);

    /*! \fn float get_y()
        \brief Get the player's current location represented as an Y coordinate.
        \return Float number representing this player's location.
    */
    float get_y();

    /*! \fn void set_y(float y)
        \brief Set the player's current location represented as an Y coordinate.
        \param y Float number representing this player's location.
    */
    void set_y(float y);

    /*! \fn int get_animationX()
        \brief Get the X coordinate location of this player in the image tilesheet.
        \return Integer X coordinate location.
    */
    int get_animationX();

    /*! \fn void set_animationX(int sourceX)
        \brief Get the X coordinate location of this player in the image tilesheet.
        \param sourceX Integer representing this player's location.
    */
    void set_animationX(int sourceX);

    /*! \fn int get_animationY()
        \brief Get the Y coordinate location of this player in the image tilesheet.
        \return Integer Y coordinate location.
    */
    int get_animationY();

    /*! \fn void set_animationY(int sourceY)
        \brief Get the Y coordinate location of this player in the image tilesheet.
        \param sourceY Integer representing this player's location.
    */
    void set_animationY(int sourceY);

    /*! \fn ALLEGRO_BITMAP* get_image()
        \brief Get the Allegro bitmap representing this player's image.
        \return ALLEGRO_BITMAP pointer.
    */
    ALLEGRO_BITMAP* get_image();

    /*! \fn void set_image(ALLEGRO_BITMAP *image)
        \brief Set the Allegro bitmap representing this player's image.
        \return image ALLEGRO_BITMAP pointer.
    */
    void set_image(ALLEGRO_BITMAP *image);

    /*! \fn Direction get_direction()
        \brief Get the player's current direction.
        \return Direction enumerator.
    */
    Direction get_direction();

    /*! \fn void set_direction(Direction dir)
        \brief Set the player's current direction.
        \return dir Direction enumerator.
    */
    void set_direction(Direction dir);

    /*! \fn bool get_isMoving()
        \brief Get the flag for movement.
        \return Boolean value representing movement. True if moving, false if still.
    */
    bool get_isMoving();

    /*! \fn void set_isMoving(bool moving)
        \brief Get the flag for movement.
        \return moving Boolean value representing movement. True if moving, false if still.
    */
    void set_isMoving(bool moving);

    /*! \fn void animationLogic()
        \brief Perform any logic routines, such as updating the image to display from a tilesheet.
    */
    void animationLogic();

    /*! \fn void move(Direction dir)
        \brief Move the player in the chosen direction.
		\param dir This value represents which direction the player is facing. Pulled from the Direction enum in common.h
    */
    void move(Direction dir);

    /*! \fn void cleanup()
        \brief Release any stored resources.
    */
    void cleanup();

    /*! \fn void draw()
		\brief Draw this player to the screen.
    */
    void draw();

    /*! \fn void draw(int x, int y)
    	\brief Draw this player to the screen at the specified coordinates.
    	\param x Integer X-axis coordinate.
    	\param y Integer Y-axis coordinate.
     */
    void draw(int x, int y);

private:

    bool _firstRollThisTurn; /*!< Flag for keeping track of die rolls. */

    bool _isAlive; /*!< Flag for keeping track of player activity. */

    float _moveSpeed; /*!< Rate of movement. */
    bool _isMoving; /*!< Flag for handling movement. */

    ALLEGRO_BITMAP *_image; /*!< Bitmap tilesheet image representing the player. */
    Direction _dir;	/*!< Represents the direction this player is facing. */
    bool _passedGo; /*!< Flag for handling if this player has completed 1 pass around the board. */
    int _id;		/*!< Player's unique ID. */
    int _money;		/*!< Player's accumulated money. */
    int _score;		/*!< Player's score. */
    int _location;	/*!< Player's current location on the gameboard. */
    float _x;		/*!< Player's x-axis location. */
    float _y;		/*!< Player's y-axis location. */
    int _sourceX;	/*!< Player's x-axis location in the animation map. */
    int _sourceY;	/*!< Player's y-axis location in the animation map. */

    PlayerType _playerType; /*!< Represents if this player is played by the CPU. */
};

#endif // PLAYER_H
