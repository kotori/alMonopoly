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

#ifndef PROPERTY_H
#define PROPERTY_H

//
// MONOPOLY PROPERTY CLASS
//
class MonopolyProperty {
public:
    MonopolyProperty();
    MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel);
    ~MonopolyProperty();

    void set_isOwned(bool owned);
    bool get_isOwned();

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

    void cleanup();

private:

    bool _owned;                        /*!< Property's ownership flag, false if not owned. */
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
};

#endif // PROPERTY_H
