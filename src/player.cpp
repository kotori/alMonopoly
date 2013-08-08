
#include <allegro5/allegro.h>

#include "common.h"
#include "player.h"

MonopolyPlayer::MonopolyPlayer() {
    // Empty
}

MonopolyPlayer::~MonopolyPlayer() {
    // Empty
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

int MonopolyPlayer::get_x() {
    return _x;
}

void MonopolyPlayer::set_x(int x) {
    _x = x;
}

int MonopolyPlayer::get_y() {
    return _y;
}

void MonopolyPlayer::set_y(int y) {
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

