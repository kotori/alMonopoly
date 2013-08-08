
#include <iostream>
#include <allegro5/allegro.h>

#include "property.h"

MonopolyProperty::MonopolyProperty() {
    _owned = false;
    _image = NULL;
}

// This class construct with override will allow you to build up an entire property via the constructor.
//  The only additional command will be set_image().
MonopolyProperty::MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel) {

    _image = NULL;
    _owned = false;

    _name = name;
    _id = id;
    _purchasePrice = purchasePrice;
    _mortgagePrice = mortgageValue;
    _pricePerHouse = pricePerHouse;
    _pricePerHotel = pricePerHotel;
    _rent = rental;
    _rent1House = rent1House;
    _rent2House = rent2House;
    _rent3House = rent3House;
    _rent4House = rent4House;
    _rentHotel = rentHotel;
}

MonopolyProperty::~MonopolyProperty() {
    // Empty
}

void MonopolyProperty::cleanup() {
    if(_image) {
        al_destroy_bitmap(_image);
    }
}

void MonopolyProperty::set_isOwned(bool owned) {
    _owned = owned;
}

bool MonopolyProperty::get_isOwned() {
    return _owned;
}

void MonopolyProperty::set_id(int id) {
    _id = id;
}

int MonopolyProperty::get_id() {
    return _id;
}

void MonopolyProperty::set_name(std::string name) {
    _name = name;
}

std::string MonopolyProperty::get_name() {
    return _name;
}

void MonopolyProperty::set_purchasePrice(int money) {
    _purchasePrice = money;
}

int MonopolyProperty::get_purchasePrice() {
    return _purchasePrice;
}

void MonopolyProperty::set_mortgagePrice(int money) {
    _mortgagePrice = money;
}

int MonopolyProperty::get_mortgagePrice() {
    return _mortgagePrice;
}

void MonopolyProperty::set_pricePerHouse(int money) {
    _pricePerHouse = money;
}

int MonopolyProperty::get_pricePerHouse() {
    return _pricePerHouse;
}

void MonopolyProperty::set_pricePerHotel(int money) {
    _pricePerHotel = money;
}

int MonopolyProperty::get_pricePerHotel() {
    return _pricePerHotel;
}

void MonopolyProperty::set_image(ALLEGRO_BITMAP *image) {
    _image = image;
}

ALLEGRO_BITMAP* MonopolyProperty::get_image() {
    return _image;
}

void MonopolyProperty::set_rent(int money) {
    _rent = money;
}

int MonopolyProperty::get_rent() {
    return _rent;
}

void MonopolyProperty::set_rent1House(int money) {
    _rent1House = money;
}

int MonopolyProperty::get_rent1House() {
    return _rent1House;
}

void MonopolyProperty::set_rent2House(int money) {
    _rent2House = money;
}

int MonopolyProperty::get_rent2House() {
    return _rent2House;
}

void MonopolyProperty::set_rent3House(int money) {
    _rent3House = money;
}

int MonopolyProperty::get_rent3House() {
    return _rent3House;
}

void MonopolyProperty::set_rent4House(int money) {
    _rent4House = money;
}

int MonopolyProperty::get_rent4House() {
    return _rent4House;
}

void MonopolyProperty::set_rentHotel(int money) {
    _rentHotel = money;
}

int MonopolyProperty::get_rentHotel() {
    return _rentHotel;
}