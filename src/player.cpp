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
	_moveSpeed = 5;
}

MonopolyPlayer::~MonopolyPlayer() {
    // Empty
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
