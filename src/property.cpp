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

#include <iostream>
#include <allegro5/allegro.h>

#include "property.h"

MonopolyProperty::MonopolyProperty() {
	m_owned = false;
    m_image = NULL;
}

// This class construct with override will allow you to build up an entire property via the constructor.
//  The only additional command will be set_image().
MonopolyProperty::MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel) {

    m_image = NULL;
    m_owned = false;
    m_isMortgaged = false;
    m_ownedBy = 0;
    m_group = 0;
    m_groupName = "";

    m_valueOfProperty = PropertyValue::VAL_NULL;

    m_name = name;
    m_id = id;
    m_purchasePrice = purchasePrice;
    m_mortgagePrice = mortgageValue;
    m_pricePerHouse = pricePerHouse;
    m_pricePerHotel = pricePerHotel;
    m_rent = rental;
    m_rent1House = rent1House;
    m_rent2House = rent2House;
    m_rent3House = rent3House;
    m_rent4House = rent4House;
    m_rentHotel = rentHotel;
}

MonopolyProperty::~MonopolyProperty() {
    // Empty
}

void MonopolyProperty::cleanup() {
    if(m_image) {
        al_destroy_bitmap(m_image);
    }
}

void MonopolyProperty::set_isOwned(bool owned) {
    m_owned = owned;
}

bool MonopolyProperty::get_isOwned() {
    return m_owned;
}

void MonopolyProperty::set_ownedBy(int ownedBy) {
	m_ownedBy = ownedBy;
}

int MonopolyProperty::get_ownedBy() {
	return m_ownedBy;
}

void MonopolyProperty::set_isMortgaged(bool isMortgaged) {
	m_isMortgaged = isMortgaged;
}

bool MonopolyProperty::get_isMortgaged() {
	return m_isMortgaged;
}

void MonopolyProperty::set_group(int group) {
	m_group = group;
}

int MonopolyProperty::get_group() {
	return m_group;
}

void MonopolyProperty::set_groupName(std::string groupName) {
	m_groupName = groupName;
}

std::string MonopolyProperty::get_groupName() {
	return m_groupName;
}


void MonopolyProperty::set_propertyValue(PropertyValue newValue) {
	m_valueOfProperty = newValue;
}

PropertyValue MonopolyProperty::get_propertyValue() {
	return m_valueOfProperty;
}

void MonopolyProperty::set_propertyType(PropertyType newType) {
    m_typeOfProperty = newType;
}

PropertyType MonopolyProperty::get_propertyType() {
    return m_typeOfProperty;
}

void MonopolyProperty::set_id(int id) {
    m_id = id;
}

int MonopolyProperty::get_id() {
    return m_id;
}

void MonopolyProperty::set_name(std::string name) {
    m_name = name;
}

std::string MonopolyProperty::get_name() {
    return m_name;
}

void MonopolyProperty::set_purchasePrice(int money) {
    m_purchasePrice = money;
}

int MonopolyProperty::get_purchasePrice() {
    return m_purchasePrice;
}

void MonopolyProperty::set_mortgagePrice(int money) {
    m_mortgagePrice = money;
}

int MonopolyProperty::get_mortgagePrice() {
    return m_mortgagePrice;
}

void MonopolyProperty::set_pricePerHouse(int money) {
    m_pricePerHouse = money;
}

int MonopolyProperty::get_pricePerHouse() {
    return m_pricePerHouse;
}

void MonopolyProperty::set_pricePerHotel(int money) {
    m_pricePerHotel = money;
}

int MonopolyProperty::get_pricePerHotel() {
    return m_pricePerHotel;
}

void MonopolyProperty::set_image(ALLEGRO_BITMAP *image) {
    m_image = image;
}

ALLEGRO_BITMAP* MonopolyProperty::get_image() {
    return m_image;
}

void MonopolyProperty::set_rent(int money) {
    m_rent = money;
}

int MonopolyProperty::get_rent() {
    return m_rent;
}

void MonopolyProperty::set_rent1House(int money) {
    m_rent1House = money;
}

int MonopolyProperty::get_rent1House() {
    return m_rent1House;
}

void MonopolyProperty::set_rent2House(int money) {
    m_rent2House = money;
}

int MonopolyProperty::get_rent2House() {
    return m_rent2House;
}

void MonopolyProperty::set_rent3House(int money) {
    m_rent3House = money;
}

int MonopolyProperty::get_rent3House() {
    return m_rent3House;
}

void MonopolyProperty::set_rent4House(int money) {
    m_rent4House = money;
}

int MonopolyProperty::get_rent4House() {
    return m_rent4House;
}

void MonopolyProperty::set_rentHotel(int money) {
    m_rentHotel = money;
}

int MonopolyProperty::get_rentHotel() {
    return m_rentHotel;
}

void MonopolyProperty::set_x(int x) {
    m_x = x;
}

int MonopolyProperty::get_x() {
    return m_x;
}

void MonopolyProperty::set_y(int y) {
    m_y = y;
}

int MonopolyProperty::get_y() {
    return m_y;
}
