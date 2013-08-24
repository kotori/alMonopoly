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
	/*! \fn MonopolyProperty()
        \brief Class constructor.
	*/
    MonopolyProperty();

    /*! \fn MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel)
        \brief Class constructor w/ overrides
	*/
    MonopolyProperty(std::string name, int id, int purchasePrice, int mortgageValue, int pricePerHouse, int pricePerHotel, int rental, int rent1House, int rent2House, int rent3House, int rent4House, int rentHotel);

	/*! \fn ~MonopolyProperty()
        \brief Class destructor.
	*/
    ~MonopolyProperty();

	/*! \fn void set_isOwned(bool owned)
        \brief Set the ownership flag for this property.
        \param owned Boolean variable, true represents owned, false is no owner.
	*/
    void set_isOwned(bool owned);

	/*! \fn bool get_isOwned()
        \brief Get the ownership flag for this property.
        \return true if this property is owned, false if this property has no owner.
	*/
    bool get_isOwned();

	/*! \fn void set_ownedBy(int ownedBy)
        \brief Set the ownership id for this property.
        \param ownedBy Integer representing a player object. \see MonopolyPlayer::get_id()
	*/
    void set_ownedBy(int ownedBy);

	/*! \fn int get_ownedBy()
        \brief Get the ownership id for this property.
        \return Integer representing a player object.
	*/
    int get_ownedBy();

    /*! \fn void set_isMortgaged(bool isMortgaged)
        \brief Set true or false based on if this property has been mortgaged.
        \param isMortgaged True if the property is mortgaged. False if not.
    */
    void set_isMortgaged(bool isMortgaged);

    /*! \fn bool get_isMortgaged()
        \brief Get true or false based on if this property has been mortgaged.
        \return True if the property is mortgaged. False if not.
    */
    bool get_isMortgaged();

    /*! \fn void set_group(int group)
        \brief Set this property's group id.
        \param group Integer representing this property's group.
    */
    void set_group(int group);

    /*! \fn int get_group()
        \brief Get this property's group id.
        \return Integer representing this property's group.
    */
    int get_group();

    /*! \fn void set_groupName(std::string groupName)
        \brief Set this property group's name.
        \param groupName std::string representing this property group's name.
    */
    void set_groupName(std::string groupName);

    /*! \fn std::string get_groupName()
        \brief Get this property group's name.
        \return std::string representing this property group's name.
    */
    std::string get_groupName();

    /*! \fn void set_propertyType(PropertyType newType)
        \brief Set this property's type enumerator.
        \param newType PropertyType enumerator.
    */
    void set_propertyType(PropertyType newType);

    /*! \fn PropertyType get_propertyType()
        \brief Get this property's type enumerator.
        \return PropertyType enumerator.
    */
    PropertyType get_propertyType();

    /*! \fn void set_propertyValue(PropertyValue newValue)
        \brief Set this property's value enumerator.
        \param newValue PropertyValue value enumerator.
    */
    void set_propertyValue(PropertyValue newValue);

    /*! \fn PropertyValue get_propertyValue()
        \brief Get this property's type enumerator.
        \return PropertyValue value enumerator.
    */
    PropertyValue get_propertyValue();

    /*! \fn void set_id(int id)
        \brief Set this property's unique ID.
        \param id Integer representing this property's ID.
    */
    void set_id(int id);

    /*! \fn int get_id()
        \brief Get this property's unique ID.
        \return Integer representing this property's ID.
    */
    int get_id();

    /*! \fn void set_name(std::string name)
        \brief Set this property's name.
        \param name std::string representing this property's name.
    */
    void set_name(std::string name);

    /*! \fn std::string get_name()
        \brief Set this property's name.
        \return A std::string representing this property's name.
    */
    std::string get_name();

    /*! \fn void set_purchasePrice(int money)
        \brief Set this property's purchase price.
        \param money Integer representing this property's value.
    */
    void set_purchasePrice(int money);

    /*! \fn int get_purchasePrice()
        \brief Get this property's purchase price.
        \return Integer representing this property's value.
    */
    int get_purchasePrice();

    /*! \fn void set_mortgagePrice(int money)
        \brief Set this property's mortgage price.
        \param money Integer representing this property's mortgage value.
    */
    void set_mortgagePrice(int money);

    /*! \fn int get_mortgagePrice()
        \brief Get this property's mortgage price.
        \return Integer representing this property's mortgage value.
    */
    int get_mortgagePrice();

    /*! \fn void set_pricePerHouse(int money)
        \brief Set the price to purchase a house on this property.
        \param money Integer representing the price for one house.
    */
    void set_pricePerHouse(int money);

    /*! \fn int get_pricePerHouse()
        \brief Get the price to purchase a house on this property.
        \return Integer representing the price for one house.
    */
    int get_pricePerHouse();

    /*! \fn void set_pricePerHotel(int money)
        \brief Set the price to purchase a hotel on this property.
        \param money Integer representing the price for one hotel.
    */
    void set_pricePerHotel(int money);

    /*! \fn int get_pricePerHotel()
        \brief Get the price to purchase a hotel on this property.
        \return Integer representing the price for one hotel.
    */
    int get_pricePerHotel();

    /*! \fn void set_image(ALLEGRO_BITMAP *image)
        \brief Set this property's image.
        \param image ALLEGRO_BITMAP pointer representing this property's image.
    */
    void set_image(ALLEGRO_BITMAP *image);

    /*! \fn ALLEGRO_BITMAP *get_image()
        \brief Get this property's image.
        \return ALLEGRO_BITMAP pointer representing this property's image.
    */
    ALLEGRO_BITMAP *get_image();

    /*! \fn void set_rent(int money)
        \brief Set the rent of this property if its undeveloped.
        \param money Integer representing the rent of this property.
    */
    void set_rent(int money);

    /*! \fn int get_rent()
        \brief Get the rent of this property if its undeveloped.
        \return Integer Integer representing the rent of this property.
    */
    int get_rent();

    /*! \fn void set_rent1House(int money)
        \brief Set the rent of this property if it has one house on it.
        \param money Integer representing the rent of this property.
    */
    void set_rent1House(int money);

    /*! \fn int get_rent1House()
        \brief Get the rent of this property if it has one house on it.
        \return Integer representing the rent of this property.
    */
    int get_rent1House();

    /*! \fn void set_rent2House(int money)
        \brief Set the rent of this property if it has two houses on it.
        \param money Integer representing the rent of this property.
    */
    void set_rent2House(int money);

    /*! \fn int get_rent2House()
        \brief Get the rent of this property if it has two houses on it.
        \return Integer representing the rent of this property.
    */
    int get_rent2House();

    /*! \fn void set_rent3House(int money)
        \brief Set the rent of this property if it has three houses on it.
        \param money Integer representing the rent of this property.
    */
    void set_rent3House(int money);

    /*! \fn int get_rent3House()
        \brief Get the rent of this property if it has three houses on it.
        \return Integer representing the rent of this property.
    */
    int get_rent3House();

    /*! \fn void set_rent4House(int money)
        \brief Set the rent of this property if it has four houses on it.
        \param money Integer representing the rent of this property.
    */
    void set_rent4House(int money);

    /*! \fn int get_rent4House()
        \brief Get the rent of this property if it has four houses on it.
        \return Integer representing the rent of this property.
    */
    int get_rent4House();

    /*! \fn void set_rentHotel(int money)
        \brief Set the rent of this property if it has one hotel on it.
        \param money Integer representing the rent of this property.
    */
    void set_rentHotel(int money);

    /*! \fn int get_rentHotel()
        \brief Get the rent of this property if it has one hotel on it.
        \return Integer representing the rent of this property.
    */
    int get_rentHotel();

    /*! \fn void set_x(int x)
        \brief Set the property's X coordinate location.
        \param x Integer representing this property's location.
    */
    void set_x(int x);

    /*! \fn int get_x()
        \brief Get the property's X coordinate location.
        \return Integer representing this property's location.
    */
    int get_x();

    /*! \fn void set_y(int y)
        \brief Set the property's Y coordinate location.
        \param y Integer representing this property's location.
    */
    void set_y(int y);

    /*! \fn int get_y()
        \brief Get the property's Y coordinate location.
        \return Integer representing this property's location.
    */
    int get_y();

	/*! \fn void cleanup()
        \brief Release any resources that this object has stored.

        This function will release from memory anything built/stored with Allegro.
        This is mainly the ALLEGRO_BITMAP objects.
	*/
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
