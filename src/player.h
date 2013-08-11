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

#ifndef PLAYER_H
#define PLAYER_H

//
// MONOPOLY PLAYER CLASS
//
class MonopolyPlayer {
public:
    MonopolyPlayer();
    ~MonopolyPlayer();

    bool get_passedGo();
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

    void move(Direction dir);

private:
    int _moveSpeed;

    ALLEGRO_BITMAP *_image;
    Direction _dir;
    bool _passedGo; /*!< Flag for handling if this player has completed 1 pass around the board. */
    int _id;		/*!< Player's unique ID. */
    int _money;		/*!< Player's accumulated money. */
    int _score;		/*!< Player's score. */
    int _location;	/*!< Player's current location on the gameboard. */
    float _x;
    float _y;
    int _sourceX;
    int _sourceY;
};

#endif // PLAYER_H
