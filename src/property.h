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

/*! \file property.h
    \brief Property class header file.

    This header allows for an easy way to rapidly build properties. Inside the Game class we
     actually populate this class's objects with contents from a SQLite3 database.
*/

#ifndef PROPERTY_H
#define PROPERTY_H

#include "common.h"

/*! \class MonopolyProperty
    \brief Property class
    This class is used to build up a single property.
*/

class MonopolyProperty {
public:
    MonopolyProperty();
    MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel);
    ~MonopolyProperty();

    void set_isOwned(bool owned);
    bool get_isOwned();

    void set_ownedBy(int ownedBy);
    int get_ownedBy();

    void set_isMortgaged(bool isMortgaged);
    bool get_isMortgaged();

    void set_group(int group);
    int get_group();

    void set_groupName(std::string groupName);
    std::string get_groupName();

    void set_propertyType(PropertyType newType);
    PropertyType get_propertyType();

    void set_propertyValue(PropertyValue newValue);
    PropertyValue get_propertyValue();

    void set_id(int id);
    int get_id();

    void set_name(std::string name);
    std::string get_name();

    void set_purchasePrice(int money);
    int get_purchasePrice();

    void set_mortgagePrice(int money);
    int get_mortgagePrice();

    void set_pricePerHouse(int money);
    int get_pricePerHouse();

    void set_pricePerHotel(int money);
    int get_pricePerHotel();

    void set_image(ALLEGRO_BITMAP *image);
    ALLEGRO_BITMAP *get_image();

    void set_rent(int money);
    int get_rent();

    void set_rent1House(int money);
    int get_rent1House();

    void set_rent2House(int money);
    int get_rent2House();

    void set_rent3House(int money);
    int get_rent3House();

    void set_rent4House(int money);
    int get_rent4House();

    void set_rentHotel(int money);
    int get_rentHotel();

    void set_x(int x);
    int get_x();

    void set_y(int y);
    int get_y();

    void cleanup();

private:

    bool _owned;                        /*!< Property's ownership flag, false if not owned. */
    int _ownedBy;						/*!< Property's player owner id. */
    int _group;							/*!< Property's color group. */
    std::string _groupName;				/*!< Property's color group name. */
    bool _isMortgaged;					/*!< Property's mortgaged flag. */
    PropertyValue _valueOfProperty;		/*!< Property's current value. */
    PropertyType _typeOfProperty;		/*!< Property's type. \see PropertyType. */
    int _id;                            /*!< Property's unique ID code. */
    std::string _name;                  /*!< Property's name as a std::string. */
    int _purchasePrice;                 /*!< Property's initial purchase price. */
    int _mortgagePrice;                 /*!< Property's mortgage value. */
    int _pricePerHouse;                 /*!< Price to put 1 house on this property. */
    int _pricePerHotel;                 /*!< Price to put 1 hotel on this property. */
    ALLEGRO_BITMAP *_image;             /*!< Image representing this property. */
    int _rent;                          /*!< Unimproved lot rental price. */
    int _rent1House;                    /*!< Rental price for 1 house. */
    int _rent2House;                    /*!< Rental price for 2 houses. */
    int _rent3House;                    /*!< Rental price for 3 houses. */
    int _rent4House;                    /*!< Rental price for 4 houses. */
    int _rentHotel;                     /*!< Rental price for 1 hotel. */
    int _x;								/*!< Property's x-axis location. */
    int _y;								/*!< Property's y-axis location. */
};

#endif // PROPERTY_H
