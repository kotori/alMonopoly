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

    int get_id();
    void set_id(int id);

    int get_money();
    void set_money(int money);

    int get_score();
    void set_score(int score);

    int get_location();
    void set_location(int location);

    float get_x();
    void set_x(float x);

    float get_y();
    void set_y(float y);

    int get_animationX();
    void set_animationX(int sourceX);

    int get_animationY();
    void set_animationY(int sourceY);

    ALLEGRO_BITMAP* get_image();
    void set_image(ALLEGRO_BITMAP *image);

    Direction get_direction();
    void set_direction(Direction dir);

    void set_isMoving(bool moving);
    bool get_isMoving();

    /*! \fn void animationFrameLogic()
        \brief Perform any logic routines, such as updating the image to display from a tilesheet.
    */
    void animationFrameLogic();

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

private:

    int _moveSpeed; /*!< Rate of movement. */
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
