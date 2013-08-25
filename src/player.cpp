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

#include <allegro5/allegro.h>

#include "common.h"
#include "player.h"

MonopolyPlayer::MonopolyPlayer() {
	// Set some default values for our private member variables.
	//  These will be set appropriately later.
	_playerType = PlayerType::COMPUTER;
	_moveSpeed = 5.0;
	_isMoving = false;
	_image = NULL;
	_dir = DOWN;
	_passedGo = false;
	_id = 0;
	_money = 0;
	_score = 0;
	_location = 0;
	_x = 0;
	_y = 0;
	_sourceX = 0;
	_sourceY = 0;
	_moveSpeed = 5;
}

MonopolyPlayer::~MonopolyPlayer() {
    // Empty
}

bool MonopolyPlayer::get_firstRoll() {
	return _firstRollThisTurn;
}

void MonopolyPlayer::set_firstRoll(bool firstRoll) {
	_firstRollThisTurn = firstRoll;
}

void MonopolyPlayer::cleanup() {
	if(_image) {
		al_destroy_bitmap(_image);
	}
}

bool MonopolyPlayer::get_passedGo() {
    return _passedGo;
}

void MonopolyPlayer::set_passedGo(bool passedGo) {
    _passedGo = passedGo;
}

int MonopolyPlayer::get_id() {
    return _id;
}

void MonopolyPlayer::set_id(int id) {
    _id = id;
}

int MonopolyPlayer::get_money() {
    return _money;
}

void MonopolyPlayer::set_money(int money) {
    _money = money;
}

int MonopolyPlayer::get_score() {
    return _score;
}

void MonopolyPlayer::set_score(int score) {
    _score = score;
}

int MonopolyPlayer::get_location() {
    return _location;
}

void MonopolyPlayer::set_location(int location) {
    _location = location;
}

float MonopolyPlayer::get_x() {
    return _x;
}

void MonopolyPlayer::set_x(float x) {
    _x = x;
}

float MonopolyPlayer::get_y() {
    return _y;
}

void MonopolyPlayer::set_y(float y) {
    _y = y;
}

int MonopolyPlayer::get_animationX() {
    return _sourceX;
}

void MonopolyPlayer::set_animationX(int sourceX) {
    _sourceX = sourceX;
}

int MonopolyPlayer::get_animationY() {
    return _sourceY;
}

void MonopolyPlayer::set_animationY(int sourceY) {
    _sourceY = sourceY;
}

ALLEGRO_BITMAP* MonopolyPlayer::get_image() {
    return _image;
}

void MonopolyPlayer::set_image(ALLEGRO_BITMAP *image) {
    _image = image;
}

Direction MonopolyPlayer::get_direction() {
    return _dir;
}

void MonopolyPlayer::set_direction(Direction dir) {
    _dir = dir;
}

bool MonopolyPlayer::get_isMoving() {
    return _isMoving;
}

void MonopolyPlayer::set_isMoving(bool moving) {
    _isMoving = moving;
}

void MonopolyPlayer::animationLogic() {
	// If the player is moving, then increment the x value to the next frame.
	if(_isMoving)
		_sourceX += al_get_bitmap_width(_image) / 3;
	else
		_sourceX = 32;

	// Ensure the x value isn't outside the width of the image.
	//  If it is, reset it to 0.
	if(_sourceX >= al_get_bitmap_width(_image))
		_sourceX = 0;

	// The y value is based upon the direction of the player.
	_sourceY = _dir;
}

void MonopolyPlayer::move(Direction dir) {
    _dir = dir;
    switch(_dir) {
    case DOWN:
        _y += _moveSpeed;
        break;
    case UP:
        _y -= _moveSpeed;
        break;
    case RIGHT:
        _x += _moveSpeed;
        break;
    case LEFT:
        _x -= _moveSpeed;
        break;
    }
}

void MonopolyPlayer::draw() {
	// Create an allegro bitmap reprenting our player's image on the animation index.
	ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap( _image, _sourceX, _sourceY * 32, 32, 32 );
	al_draw_bitmap( subBitmap, _x, _y, 0 );
}

void MonopolyPlayer::draw(int x, int y) {
	al_draw_bitmap( _image, x, y, 0 );
}
