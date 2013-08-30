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
	m_playerType = PlayerType::COMPUTER;
    m_isAlive = true;
	m_moveSpeed = 5.0;
	m_isMoving = false;
	m_firstRollThisTurn = true;
	m_image = NULL;
	m_dir = DOWN;
	m_passedGo = false;
	m_id = 0;
	m_money = 0;
	m_score = 0;
	m_location = 0;
	m_x = 0;
	m_y = 0;
	m_sourceX = 0;
	m_sourceY = 0;
	m_moveSpeed = 5;
	m_inJail = false;
	m_getOutOfJailCards = 0;
}

MonopolyPlayer::~MonopolyPlayer() {
    // Empty
}

bool MonopolyPlayer::get_firstRoll() {
	return m_firstRollThisTurn;
}

void MonopolyPlayer::set_firstRoll(bool firstRoll) {
	m_firstRollThisTurn = firstRoll;
}

bool MonopolyPlayer::get_inJail() {
	return m_inJail;
}

void MonopolyPlayer::set_inJail(bool convicted) {
	m_inJail = convicted;
}

int MonopolyPlayer::get_jailCards() {
	return m_getOutOfJailCards;
}

void MonopolyPlayer::set_jailCards(int numCards) {
	m_getOutOfJailCards = numCards;
}

bool MonopolyPlayer::get_isAlive() {
	return m_isAlive;
}

void MonopolyPlayer::set_isAlive(bool alive) {
	m_isAlive = alive;
}

void MonopolyPlayer::cleanup() {
	if(m_image) {
		al_destroy_bitmap(m_image);
	}
}

bool MonopolyPlayer::get_passedGo() {
    return m_passedGo;
}

void MonopolyPlayer::set_passedGo(bool passedGo) {
    m_passedGo = passedGo;
}

int MonopolyPlayer::get_id() {
    return m_id;
}

void MonopolyPlayer::set_id(int id) {
    m_id = id;
}

int MonopolyPlayer::get_money() {
    return m_money;
}

void MonopolyPlayer::set_money(int money) {
    m_money = money;
}

int MonopolyPlayer::get_score() {
    return m_score;
}

void MonopolyPlayer::set_score(int score) {
    m_score = score;
}

int MonopolyPlayer::get_location() {
    return m_location;
}

void MonopolyPlayer::set_location(int location) {
    m_location = location;
}

float MonopolyPlayer::get_x() {
    return m_x;
}

void MonopolyPlayer::set_x(float x) {
    m_x = x;
}

float MonopolyPlayer::get_y() {
    return m_y;
}

void MonopolyPlayer::set_y(float y) {
    m_y = y;
}

int MonopolyPlayer::get_animationX() {
    return m_sourceX;
}

void MonopolyPlayer::set_animationX(int sourceX) {
    m_sourceX = sourceX;
}

int MonopolyPlayer::get_animationY() {
    return m_sourceY;
}

void MonopolyPlayer::set_animationY(int sourceY) {
    m_sourceY = sourceY;
}

ALLEGRO_BITMAP* MonopolyPlayer::get_image() {
    return m_image;
}

void MonopolyPlayer::set_image(ALLEGRO_BITMAP *image) {
    m_image = image;
}

Direction MonopolyPlayer::get_direction() {
    return m_dir;
}

void MonopolyPlayer::set_direction(Direction dir) {
    m_dir = dir;
}

bool MonopolyPlayer::get_isMoving() {
    return m_isMoving;
}

void MonopolyPlayer::set_isMoving(bool moving) {
    m_isMoving = moving;
}

void MonopolyPlayer::animationLogic() {
	// If the player is moving, then increment the x value to the next frame.
	if(m_isMoving)
		m_sourceX += al_get_bitmap_width(m_image) / 3;
	else
		m_sourceX = 32;

	// Ensure the x value isn't outside the width of the image.
	//  If it is, reset it to 0.
	if(m_sourceX >= al_get_bitmap_width(m_image))
		m_sourceX = 0;

	// The y value is based upon the direction of the player.
	m_sourceY = m_dir;
}

void MonopolyPlayer::move(Direction dir) {
    switch(dir) {
    case Direction::DOWN:
        m_y += m_moveSpeed;
        break;
    case Direction::UP:
        m_y -= m_moveSpeed;
        break;
    case Direction::RIGHT:
        m_x += m_moveSpeed;
        break;
    case Direction::LEFT:
        m_x -= m_moveSpeed;
        break;
    }
}

void MonopolyPlayer::draw() {
	// Create an allegro bitmap reprenting our player's image on the animation index.
	ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap( m_image, m_sourceX, m_sourceY * 32, 32, 32 );
	al_draw_bitmap( subBitmap, m_x, m_y, 0 );
}

void MonopolyPlayer::draw(int x, int y) {
	al_draw_bitmap( m_image, x, y, 0 );
}
