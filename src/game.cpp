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
#include <fstream>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "common.h"
#include "database.h"
#include "property.h"
#include "player.h"
#include "game.h"

MonopolyGame::MonopolyGame() {
    // Nullfiy the Allegro objects.
	m_alDisplayDevice = NULL;
	m_alEventQueue = NULL;
	m_alTimer = NULL;
	m_alFrameTimer = NULL;
	m_alBoardImage = NULL;
	m_activeGameMode = GameMode::EASY;

    // Nullify each font in the font collection.
    for( int fCount = 0; fCount < MAX_FONTS; fCount++ ) {
    	m_fontCollection[fCount] = NULL;
    }

    // Nullify each player piece.
    for( int ppCount = 0; ppCount < PLAYER_PIECES_COUNT; ppCount++ ) {
    	m_alpieceImages[ppCount] = NULL;
    }
}

MonopolyGame::~MonopolyGame() {
    // Empty
}

// This should be called everytime the game is reset.
void MonopolyGame::reset() {

    // Seed the random number generator.
    srand( (unsigned)time( 0 ) );

	// Reset the camera position.
	m_alCamera.cameraPosition[Positions::X_POS] = 0;
	m_alCamera.cameraPosition[Positions::Y_POS] = 0;

	m_isActive = false;

	m_playersTurn = 0;
	m_numPlayers = NUM_PLAYERS;
	m_firstTurn = true;
	m_turnDone = false;
	m_firstMovePass = true;

	m_currentReaction = ReactionType::NULL_REACTION;
	m_turnState = TurnState::NULL_STATE;

	m_diceRoll = rollDice( NUM_DICE, 0 );

	m_doublesRollCounter = 0;

	m_moveDestination = 0;

	m_exitGame = false;
	m_redrawScreen = true;

    // Reset each player attribute.
    for( int pCount = 0; pCount < NUM_PLAYERS; pCount++ ) {
    	m_playerList[pCount].set_location(0);
    	m_playerList[pCount].set_score(0);

        if( m_activeGameMode == GameMode::EASY ) {
        	m_playerList[pCount].set_money( 3500 );
        }
        else if( m_activeGameMode == GameMode::NORMAL ) {
        	m_playerList[pCount].set_money( 2500 );
        }
        else if( m_activeGameMode == GameMode::DIFFICULT ) {
        	m_playerList[pCount].set_money( 1500 );
        }
    }

    // Reset each property attribute.
    for( int pCount = 0; pCount < MAX_PROPERTIES; pCount++ ) {
    	m_propertyList[pCount].set_isMortgaged( false );
    	m_propertyList[pCount].set_isOwned( false );
    	m_propertyList[pCount].set_ownedBy( 0 );
    }
}

bool MonopolyGame::fileExists(const char *filename)
{
	std::ifstream ifile( filename );
	return ifile;
}

void MonopolyGame::cameraUpdate(float *cameraPosition, float x, float y, int width, int height) {
	// Set the camera's x and y axis to be centered around the passed x and y values.
    cameraPosition[Positions::X_POS] = -(WINDOW_WIDTH / 2) + (x + width / 2);
    cameraPosition[Positions::Y_POS] = -(WINDOW_HEIGHT / 2) + (y + height / 2);

    // If the camera's x position is less than 0 reset the position.
    if(cameraPosition[Positions::X_POS] < 0) {
        cameraPosition[Positions::X_POS] = 0;
    }
    // If the camera's y position is less than 0 reset the position.
    if(cameraPosition[Positions::Y_POS] < 0) {
        cameraPosition[Positions::Y_POS] = 0;
    }
}

bool MonopolyGame::mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr) {
	// Keep track of errors.
	bool success = false;
	// Set the number of properties in this set.
	int numPropertiesInSet = 0;

	std::string tempQuery = "";
	// Select the number of properties that are in the same color group as the passed property.
	tempQuery = m_sqlConn.Format( "SELECT COUNT(*) FROM %s WHERE %s = %i", DB_PROPERTY_TABLE, "set_id", prop.get_group() );
	if( m_sqlConn.SelectInt( numPropertiesInSet, tempQuery.c_str() ) ) {
		fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
	}
	
	// Build a list of the friendly properties.
	int friendlyPropertiesId[numPropertiesInSet];
	for( int pCount=0; pCount < numPropertiesInSet; pCount++ ) {
		if( pCount == 0 ) {
			friendlyPropertiesId[pCount] = prop.get_id();
		}
		else if(pCount == 1){
			tempQuery = m_sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i LIMIT 1",
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id() );
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( m_sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
				friendlyPropertiesId[pCount] = tNum;
			}
		}
		else {
			tempQuery = m_sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i AND %s != %i LIMIT 1",
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id(), "id", friendlyPropertiesId[1]);
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( m_sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
				friendlyPropertiesId[pCount] = tNum;
			}
		}
	}
	
	// First ensure this player owns this property.
	if( prop.get_ownedBy() == plyr.get_id() ) {
		
		switch( prop.get_propertyValue() ) {
		
			case PropertyValue::VAL_NULL:
				// Null type, do nothing.
				break;
			case PropertyValue::VAL_MORTGAGED:
				// Mortgaged property, also do nothing.
				break;
			case PropertyValue::VAL_OWNED:
				// Give the player the mortgage value of the property.
				plyr.set_money( prop.get_mortgagePrice() );
				// Mark the property as mortgaged.
				prop.set_isMortgaged( true );
				prop.set_propertyValue( PropertyValue::VAL_MORTGAGED );
				// Relinquish ownership of this property.
				prop.set_ownedBy( 0 );
				success = true;
				break;
			case PropertyValue::VAL_OWNED_SET:
				// Give the player the mortgage value of the property.
				plyr.set_money( prop.get_mortgagePrice() );
				// Mark the property as mortgaged.
				prop.set_isMortgaged( true );

				// For each other property in the set, decrease their value as this is no longer a set.
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					if( !m_propertyList[activeProperty].get_isMortgaged() ) {
						m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_OWNED );
					}
					else {
						// Change the value of this property.
						prop.set_propertyValue( PropertyValue::VAL_MORTGAGED );
					}
				}
				success = true;
				break;
			case PropertyValue::VAL_1_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = m_propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_OWNED_SET );
				}
				break;
			case PropertyValue::VAL_2_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = m_propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_1_HOUSE );
				}
				break;
			case PropertyValue::VAL_3_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = m_propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_2_HOUSE );
				}
				break;
			case PropertyValue::VAL_4_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = m_propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_3_HOUSE );
				}
				break;
			case PropertyValue::VAL_1_HOTEL:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = m_propertyList[activeProperty].get_pricePerHotel();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					m_propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_4_HOUSE );
				}
				break;
		}
	}
	return success;
}

bool MonopolyGame::purchaseProperty(MonopolyProperty &prop, MonopolyPlayer &plyr, int priceMod) {
	// Keep track of errors.
	bool success = false;
	
	// If the property is not already owned.
	if( !prop.get_isOwned() ) {
		// Keep track of the player's money.
		int pMoney = plyr.get_money();

		// Now we will check if this property is mortgaged and work with that price.
		if( prop.get_isMortgaged() ) {
			int cost = prop.get_mortgagePrice() - priceMod;
			if( pMoney > cost ) {
				plyr.set_money( pMoney - cost );
				prop.set_isOwned( true );
				prop.set_isMortgaged( true );
				prop.set_ownedBy( plyr.get_id() );
				fprintf( stderr, "Player #: %i has purchased property: %s", plyr.get_id(), prop.get_name().c_str() );
				success = true;
			}
			else {
				// Oops, this player can't afford this property.
				fprintf( stderr, "Player #: %i Cannot afford property: %s", plyr.get_id(), prop.get_name().c_str() );
				success = false;
			}
		}
		else {
			// Now that we've determined the property is not owned or mortgaged, we will use the full price.
			int cost = prop.get_purchasePrice() - priceMod;
			if( pMoney > cost ) {
				plyr.set_money( pMoney - cost );
				prop.set_isOwned( true );
				prop.set_isMortgaged( true );
				prop.set_ownedBy( plyr.get_id() );
				fprintf( stderr, "Player #: %i has purchased property: %s", plyr.get_id(), prop.get_name().c_str() );
				success = true;
			}
			else {
				// Oops, this player can't afford this property.
				fprintf( stderr, "Player #: %i Cannot afford property: %s", plyr.get_id(), prop.get_name().c_str() );
				success = false;
			}
		}
	}
	else {
		fprintf( stderr, "Player: %i already owns this property!\n", plyr.get_id() );
	}
	// The property has not been sold.
	return success;
}

int MonopolyGame::buildPropertyList() {
    std::string tempQuery = "";
    std::string tempString = "";
    int tempNum = 0;

    fprintf(stderr, "Preparing to build properties...\n");

    // Since the entries in the database are offset use this variable to store that new offset.
    int tCounter = 0;
    int offset = 1;
    // Extract attributes for each property in the database.
    for( int propertyCount = 0; propertyCount < MAX_PROPERTIES; propertyCount++ ) {
    	tCounter = propertyCount + offset;
        // Set NAME
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "name", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectStr(tempString, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_name(tempString);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set ID
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_id(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PURCHASE PRICE
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "price", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set MORTGAGE PRICE
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "mortgage", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_mortgagePrice(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOUSE
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_pricePerHouse(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOTEL
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_hotel", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_pricePerHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR HOTEL
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_hotel", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rentHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 1 HOUSE
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_1_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent1House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 2 HOUSES
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_2_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent2House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 3 HOUSES
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_3_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent3House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 4 HOUSES
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_4_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_rent4House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set x location.
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "x", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_x(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            // return -1;  Bypass this check for now.
        }
        // Set y location.
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "y", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_y(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            // return -1;  Bypass this check for now.
        }

        // Select the type of property from the database and find its matching enum type.
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "type", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            switch(tempNum) {
            case PropertyType::TYPE_RAILROAD:
            	m_propertyList[propertyCount].set_propertyType(PropertyType::TYPE_RAILROAD);
                break;
            case PropertyType::TYPE_UTILITY:
            	m_propertyList[propertyCount].set_propertyType(PropertyType::TYPE_UTILITY);
                break;
            case PropertyType::TYPE_PROPERTY:
            	m_propertyList[propertyCount].set_propertyType(PropertyType::TYPE_PROPERTY);
                break;
            case PropertyType::TYPE_CARD:
            	m_propertyList[propertyCount].set_propertyType(PropertyType::TYPE_CARD);
                break;
            case PropertyType::TYPE_TAX:
            	m_propertyList[propertyCount].set_propertyType(PropertyType::TYPE_TAX);
                break;

            }
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        
        
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_group(tempNum);
        }
        else {
        	fprintf(stderr, "SQL QUERY ERROR\n");
        	return -1;
        }
        
        tempQuery = m_sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!m_sqlConn.SelectStr(tempString, tempQuery.c_str())) {
        	m_propertyList[propertyCount].set_groupName(tempString);
        }
        else {
        	fprintf(stderr, "SQL QUERY ERROR\n");
        	return -1;
        }
        
        fprintf(stderr, "Built Property: %s [ID: %i] \n",
        	m_propertyList[propertyCount].get_name().c_str(),
        	m_propertyList[propertyCount].get_id());
    }
    return 0;
}

int* MonopolyGame::rollDice(size_t numOfDice, int sides) {
	// Create a new integer array to fill with random integers.
	int *ret = new int[numOfDice];

	// For the number of dice passed, generate a random number no greater than sides.
	for(size_t dieCount = 0; dieCount < numOfDice; dieCount++) {
		// Add +1 to the result to ensure a value above 0.
		int floor = 0, range = (sides - floor);
		int ranNum = floor + int((range * rand()) / (RAND_MAX + 1.0));
		// Copy the random number into the array to be returned.
		ret[dieCount] = ranNum;
	}
	// Return the populated random integer array.
	return ret;
}

void MonopolyGame::handleDiceRoll(MonopolyPlayer &plyr) {
	// Roll 2, six sided dice.
	m_diceRoll = rollDice( NUM_DICE, NUM_SIDES_PER_DIE );

	// If this is the player's first dice roll, reset the doubles counter.
	if( plyr.get_firstRoll() ) {
		m_doublesRollCounter = 0;
		plyr.set_firstRoll( false );
	}

	// If die 1 equals die 2, then increment the doubles counter.
	if( m_diceRoll[0] == m_diceRoll[1] ) {
		//  Need to ensure that when this player's turn ends, this counter is reset to 0.
		m_doublesRollCounter++;
	}
}

void MonopolyGame::drawText(ALLEGRO_COLOR col, int x, int y, const char *msg, ...) {

	char buffer[256];
	va_list ap;

	va_start( ap, msg );
	vsprintf( buffer, msg, ap );
	va_end( ap );

	// First print the shadow, which is slightly off-centered.
	int offset = 2;
	al_draw_textf(
		m_fontCollection[0],
		al_map_rgb( 0, 0, 0 ),
		x + offset,
		y + offset,
		NULL,
		buffer );

	// Finally we print the foreground text.
	al_draw_textf(
		m_fontCollection[0],
		col,
		x,
		y,
		NULL,
		buffer );
}

void MonopolyGame::drawTextCen(ALLEGRO_COLOR col, int x, int y, const char *msg, ...) {

	char buffer[256];
	va_list ap;

	va_start( ap, msg );
	vsprintf( buffer, msg, ap );
	va_end( ap );

	// First print the shadow, which is slightly off-centered.
	int offset = 2;
	al_draw_text(
		m_fontCollection[0],
		al_map_rgb( 0, 0, 0 ),
		x + offset,
		y + offset,
		ALLEGRO_ALIGN_CENTRE,
		buffer );

	// Finally we print the foreground text.
	al_draw_text(
		m_fontCollection[0],
		col,
		x,
		y,
		ALLEGRO_ALIGN_CENTRE,
		buffer );
}

void MonopolyGame::draw() {

	// Based upon the current turnstate we will display different things.
	switch(m_turnState) {
		case NULL_STATE:
		case PRE_TURN:
			drawTextCen( al_map_rgb( 255, 0, 0 ), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "MONOPOLY!" );
			break;
		case PRE_GAME:
			// This is where I need to implement a 'menu class' for user input.
			// enum MenuSelection { PRE_GAME_MENU = 0, IN_GAME_MENU = 1, POST_GAME_MENU = 2 };
			//  menu[PRE_GAME_MENU].draw();
			// Later a logic portion will need to be added as well.
			// menu[PRE_GAME_MENU].doLogic();
			break;
		case ROLL_PHASE:
		case MOVE_PHASE:
		case REACT_PHASE:
		case POST_TURN:
			// Draw the board first.
			al_draw_bitmap( m_alBoardImage, 0, 0, 0 );

			// Next Draw each of the player's.
			//  This should be fine tuned later to only draw 'alive' players.
			for( int playerCounter = 0; playerCounter < m_numPlayers; playerCounter++ ) {
				m_playerList[m_playersTurn].draw();
			}
			break;
		case TRADING:
			// TODO: implement trading.
			break;
	}

	//
	// Print some debugging info.
	//
	drawText( al_map_rgb( 0, 200, 240 ), 0, 0, "Num. of Players: %i", m_numPlayers );
	drawText( al_map_rgb( 0, 200, 240 ), 0, 32, "Current Player: %i", m_playersTurn );
	drawText( al_map_rgb( 0, 200, 240 ), 0, 64, "Current Player's Money: $%i", m_playerList[m_playersTurn].get_money() );

	// Print FPS - This is currently not working quite right.
	drawText( al_map_rgb( 0, 200, 240 ), 0, 96, "FPS: %.2f", m_currFps );

	// Next, we flip the display to make the changes visible,
	//  and clear the background before we print again.
	al_flip_display();
	al_clear_to_color( al_map_rgb( 255, 255, 255 ) );
}

int MonopolyGame::loadResources() {

	reset();

    // Normalize the paths.
    ALLEGRO_PATH *path = al_get_standard_path( ALLEGRO_RESOURCES_PATH );
    al_append_path_component( path, "etc" );
    al_change_directory( al_path_cstr( path, '/' ) );  // change the working directory

    //
    // Load the board's bitmap
    //
    al_set_path_filename(path, "board.png");
    if( !fileExists( al_path_cstr(path, '/') ) ) {
       	fprintf( stderr, "[ERROR] Cannot find bitmap: %s\n", al_path_cstr(path, '/') );
       	return -1;
    }
    else {
    	m_alBoardImage = al_load_bitmap( al_path_cstr(path, '/') );
    	if( !m_alBoardImage ) {
    		fprintf( stderr, "[ERROR] Failed loading board bitmap: %s\n", al_path_cstr(path, '/') );
    		return -1;
    	}
    }

    //
    // Load the font set.
    //
    al_set_path_filename(path, "Emblem.ttf");
    // Start building the fonts from 24pt.
    int initialSize = 24;
    if( !fileExists( al_path_cstr(path, '/') ) ) {
    	fprintf( stderr, "[ERROR] Cannot find font file: %s\n", al_path_cstr(path, '/') );
    	return -1;
    }
    else
    {
    	for( int fontCounter = 0; fontCounter < MAX_FONTS; fontCounter++ ) {
    		m_fontCollection[fontCounter] = al_load_font( al_path_cstr(path, '/'), initialSize, 0 );
    		initialSize += 10;
    		if( !m_fontCollection[fontCounter] ) {
    			fprintf( stderr, "[ERROR] Failed loading font: %s size: %i\n", al_path_cstr(path, '/'), initialSize );
    			return -1;
    		}
    	}
    }

    // Now load the possible player pieces into memory.
    //  The piece file locations are stored in the database.
    for( int pieceCounter = 0; pieceCounter < PLAYER_PIECES_COUNT; pieceCounter++ ) {
    	std::string tempQuery = "";
    	std::string sFileName = "";
    	tempQuery = m_sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i", "path", DB_PIECES_TABLE, "id", pieceCounter );

    	if( !m_sqlConn.SelectStr( sFileName, tempQuery.c_str() ) ) {
    		// Append the filename to the resource path.
    		al_set_path_filename( path, sFileName.c_str() );
        	if( !fileExists( al_path_cstr(path, '/') ) ) {
        		fprintf( stderr, "[ERROR] Cannot find bitmap: %s\n", al_path_cstr(path, '/') );
        		return -1;
        	}
        	else {
        		// Load the bitmap from the filename from above.
        		m_alpieceImages[pieceCounter] = al_load_bitmap( al_path_cstr(path, '/') );
        	}
    		if( !m_alpieceImages[pieceCounter] ) {
    			// TODO: CURRENTLY I HAVE NO PLACE HOLDER IMAGES, SO THE APPLICATION WILL FAIL HERE!
    			fprintf( stderr, "[ERROR] Failed loading bitmap: %s\n", al_path_cstr(path, '/') );
    			return -1;
    		}
    	}
    	else {
    		fprintf( stderr, "[ERROR] Error processing SQL statement: %s\n", tempQuery.c_str() );
    		return -1;
    	}
    }

    // Assign random pieces to each player.
    for( int plCount = 0; plCount < NUM_PLAYERS; plCount++ ) {
    	int* randomPlayerSel = rollDice( 1, PLAYER_PIECES_COUNT );
    	m_playerList[plCount].set_image( m_alpieceImages[randomPlayerSel[0]] );

    }

    al_destroy_path( path );

    //
    // Finally we will pull the property list from the database.
    //
    if( buildPropertyList() )
    {
        fprintf( stderr, "Failure building Property List, Please ensure the database is available!\n" );
        return -1;
    }

    return 0;
}

int MonopolyGame::init() {

    // Now begin initializing the Allegro library.
    if(!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro.\n");
        return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "Failed to initialize the Keyboard!\n");
        return -1;
    }

    m_alTimer = al_create_timer(1.0 / MAX_FPS);
    if(!m_alTimer) {
        fprintf(stderr, "Failed to create Timer!\n");
        return -1;
    }

    m_alFrameTimer = al_create_timer(1.0 / MAX_FRAME_FPS);
    if(!m_alFrameTimer) {
        fprintf(stderr, "Failed to create Timer!\n");
        al_destroy_timer(m_alTimer);
        return -1;
    }

    al_init_image_addon(); // Initialize the image addon.

    al_init_font_addon(); // Initialize the font addon.
    al_init_ttf_addon(); // Initialize the ttf (True Type Font) addon.

    // Setup the display device in windowed mode.
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    m_alDisplayDevice = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!m_alDisplayDevice) {
        fprintf(stderr, "Failed to create Display: %i x %i\n", WINDOW_WIDTH, WINDOW_HEIGHT);
        al_destroy_timer(m_alTimer);
        al_destroy_timer(m_alFrameTimer);
        return -1;
    }

    // Create an event queue. This is where timed events, etc are pulled from.
    m_alEventQueue = al_create_event_queue();
    if(!m_alEventQueue) {
        fprintf(stderr, "Failed to create Event Queue!\n");
        al_destroy_display(m_alDisplayDevice);
        al_destroy_timer(m_alTimer);
        al_destroy_timer(m_alFrameTimer);
        return -1;
    }

    // Register an event source per input.
    al_register_event_source(m_alEventQueue, al_get_display_event_source(m_alDisplayDevice));
    al_register_event_source(m_alEventQueue, al_get_timer_event_source(m_alTimer));
    al_register_event_source(m_alEventQueue, al_get_timer_event_source(m_alFrameTimer));
    al_register_event_source(m_alEventQueue, al_get_keyboard_event_source());

    // Ensure the mouse cursor is not visible.
    al_hide_mouse_cursor(m_alDisplayDevice);

    // Set some default values for the camera.
    m_alCamera.cameraPosition[Positions::X_POS] = 0;
    m_alCamera.cameraPosition[Positions::Y_POS] = 0;

    return 0;
}

void MonopolyGame::calcFramerate() {
	double gameTime = al_get_time();
	if( gameTime - m_oldFps >= 1.0 ) {
	 	m_currFps = m_framesDone / ( gameTime - m_oldFps );
	   	m_framesDone = 0;
	   	m_oldFps = gameTime;
	}
}

int MonopolyGame::run() {

    // Start the timer's so that revisions are properly drawn.
    al_start_timer( m_alTimer );
    al_start_timer( m_alFrameTimer );

	// Perform the first display update.
    al_flip_display();

    // Zero the FPS counters.
    m_oldFps = 0.0;
    m_currFps = 0.0;
    m_framesDone = 0.0;

    al_flush_event_queue( m_alEventQueue );

    // Run until the exit flag is thrown.
    while( !m_exitGame )
    {
        ALLEGRO_EVENT alEvent;
        al_wait_for_event( m_alEventQueue, &alEvent );
        al_get_keyboard_state( &m_alKeyState );

        // If the user clicks the window's 'close (X)' button.
        if( alEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
        {
        	// Exit the game.
        	m_exitGame = true;
        }
        // Or if the user hits the 'ESC' key.
        else if( al_key_down( &m_alKeyState, ALLEGRO_KEY_ESCAPE ) )
        {
        	// Exit the game.
        	m_exitGame = true;
        }
        // Process any timed events that have been triggered.
        else if( alEvent.type == ALLEGRO_EVENT_TIMER )
        {
            if( alEvent.timer.source == m_alTimer )
            {
            	// This is the hook into the turn logic.
            	handleTurn();

            	// We will update the camera position only if the timer has been triggered.
            	cameraUpdate( m_alCamera.cameraPosition, // X and Y location of the camera.
            		m_playerList[m_playersTurn].get_x(), // Player's x position.
            		m_playerList[m_playersTurn].get_y(), // Player's y position.
            		32, 32 ); // Width and height of player's image.
            }

            else if( alEvent.timer.source == m_alFrameTimer )
            {
				for( int pCounter = 0; pCounter < NUM_PLAYERS; pCounter++ ) {
					// If this player has more than '0' money.
					if( m_playerList[pCounter].get_money() > 0 ) {
						// For each player, update the image we will display this frame.
						m_playerList[pCounter].animationLogic();
					}
				}
            }

            // Do some camera transform magic.
            al_identity_transform( &m_alCamera.alCameraTransform );

            // Adjust the camera so that it is centered over our player's position.
            al_translate_transform( &m_alCamera.alCameraTransform,
            	-m_alCamera.cameraPosition[Positions::X_POS],
            	-m_alCamera.cameraPosition[Positions::Y_POS] );

            // Push the camera's changes.
            al_use_transform( &m_alCamera.alCameraTransform );

            // Since the screen has been updated, we want to flag the screen to be redrawn.
            m_redrawScreen = true;
        }

         //if( m_redrawScreen ) {
        if( m_redrawScreen && al_is_event_queue_empty( m_alEventQueue ) ) {
        	m_redrawScreen = false;
        	m_framesDone++;
            draw();
        }
        calcFramerate();

    }
    return 0;
}

// NOT IMPLEMENTED YET.
void MonopolyGame::handleMove() {

	int pLoc = m_playerList[m_playersTurn].get_location();

	/* TODO: Perform actual movment routines, this will
	 *  involve determining where the player is on the board
	 *  in comparison to the location of the destination point.
	 */

	// Find direction based on player's current location.
	if( pLoc > 0 && pLoc < 12 ) {
		m_playerList[m_playersTurn].set_direction( Direction::LEFT );
	}
	else if( pLoc > 11 && pLoc < 21 ) {
		m_playerList[m_playersTurn].set_direction( Direction::UP );
	}
	else if( pLoc > 20 && pLoc < 32 ) {
		m_playerList[m_playersTurn].set_direction( Direction::RIGHT );
	}
	else if( pLoc > 31 && pLoc < 41 ) {
		m_playerList[m_playersTurn].set_direction( Direction::DOWN );
	}
	else {
		fprintf( stderr, "Invalid player location [%i] detected!\n", pLoc );
	}

	// If the player has arrived at the destination
	//  then enter the reaction phase.
	if( pLoc == m_moveDestination ) {
		// We've reached our destination.
		m_turnState = TurnState::REACT_PHASE;
	}
}

int MonopolyGame::findCost(MonopolyProperty &prop) {
	// Calculate the cost of landing on this location.
	PropertyValue propVal = prop.get_propertyValue();
	// Zero the initial cost.
	int cost = 0;
	if( propVal == PropertyValue::VAL_OWNED ) {
		// Set the cost of rent on this property without the full set.
		cost = prop.get_rent();
	}
	else if( propVal == PropertyValue::VAL_OWNED_SET ) {
		// Set the cost of rent on this property in a full set.
		cost = prop.get_rent() * 2;
	}
	else if( propVal == PropertyValue::VAL_1_HOUSE ) {
		// Set the cost of rent on this property with one house on it.
		cost = prop.get_rent1House();
	}
	else if( propVal == PropertyValue::VAL_2_HOUSE ) {
		// Set the cost of rent on this property with two houses on it.
		cost = prop.get_rent2House();
	}
	else if( propVal == PropertyValue::VAL_3_HOUSE ) {
		// Set the cost of rent on this property with three houses on it.
		cost = prop.get_rent3House();
	}
	else if( propVal == PropertyValue::VAL_4_HOUSE ) {
		// Set the cost of rent on this property with four houses on it.
		cost = prop.get_rent4House();
	}
	else if( propVal == PropertyValue::VAL_1_HOTEL ) {
		// Set the cost of rent on this property with one hotel on it.
		cost = prop.get_rentHotel();
	}

	return cost;
}

void MonopolyGame::handleReaction() {
	/* This state is called when a player lands on a board space.
	 *  If the property is owned, pay the appropriate rent.
	 *  If unowned, present the title, and ask to purchase.
	 *  Pay tax if required.
	 *  If the player passed go, collect $200
	 *  If a card is drawn, move phase may be called again, and then immediately set back to REACT_PHASE.
	 */
	int pLoc = m_playerList[m_playersTurn].get_location();

	// First lets ensure our player hasn't rolled 3 doubles.
	if( m_doublesRollCounter >= 3 ) {
		m_doublesRollCounter = 0;
		m_playerList[m_playersTurn].set_inJail( true );
		m_turnState = TurnState::POST_TURN;
	}
	// Now check if the property the player is currently on is owned
	//  but someone other than the current player and the propery is not mortgaged.
	else if( m_propertyList[pLoc].get_isOwned() &&
		m_propertyList[pLoc].get_ownedBy() != m_playersTurn &&
		m_propertyList[pLoc].get_propertyValue() != PropertyValue::VAL_MORTGAGED ) {

		int rentDue = findCost( m_propertyList[pLoc] );
		if( m_playerList[m_playersTurn].get_money() < rentDue ) {
			m_turnState = TurnState::POST_TURN;
		}
		else {

			// Get the player's wallet size, and subtract the property value from it.
			rentDue -= m_playerList[m_playersTurn].get_money();
			m_playerList[m_playersTurn].set_money( rentDue );
		}

	}
	else if( !m_propertyList[pLoc].get_isOwned() ) {
		// Buy a free property.
	}
}

// NOT IMPLEMENTED YET.
// TODO: Hook this function into the game loop.
void MonopolyGame::handleState() {

	/*
	 * Pre turn state.
	 */
	if( m_turnState == TurnState::PRE_TURN ) {
		// Display pre-turn menu
		// - Improve Property
		// - Mortgage Property
		// - Roll Dice (turnState = ROLL_PHASE)

		// Handle the logic for this menu, this includes what menu entry is currently selected.
		//menu[PRE_TURN].logic(); // Then in the drawing phase we call, menu[PRE_TURN].draw()
	}
	/*
	 * Rolling the dice phase.
	 */
	else if( m_turnState == TurnState::ROLL_PHASE ) {
		// If the player clicks the ROLL button:
		handleDiceRoll( m_playerList[m_playersTurn] );
		m_firstMovePass = true;
	}
	/*
	 * Move the player's icon phase.
	 */
	else if( m_turnState == TurnState::MOVE_PHASE ) {
		if( m_firstMovePass ) {
			// Ensure our destination is within bounds.
			if( m_moveDestination > MAX_PROPERTIES  ) {
				m_moveDestination -= MAX_PROPERTIES;
			}
			m_firstMovePass = false;
		}

		handleMove();
	}
	/*
	 * Reacting the the dice results phase.
	 */
	else if( m_turnState == TurnState::REACT_PHASE ) {
		handleReaction();
	}
	/*
	 * Wrapping up the turn.
	 */
	else if( m_turnState == TurnState::POST_TURN ) {
		// This is the turn's wrap-up phase. A player can mortgage properties, trade or end their turn.
		m_turnDone = true; // debugging.
		m_doublesRollCounter = 0;
	}
	else {
		fprintf( stderr, "Invalid state encountered: %i\n", m_turnState );
	}
}

// NOT IMPLEMENTED YET.
void MonopolyGame::handleTurn() {
    // If this is the player's first turn,
	//  not the first turn of the game,
	//  just the first pass for this set of turns.
    if( m_firstTurn ) {
        // Reset the firstTurn flag.
    	m_firstTurn = false;
        // Ensure the turnState is reset to start from the beginning.
    	m_turnState = TurnState::PRE_TURN;
    }

	// Set this value to false. If this player can continue its turn,
	//  then a true will be set below.
    bool canPlay = false;
    // First make sure the player we have selected can actually play.
    //  If the player is broke, we will skip this one, and increment the list.
    if( m_playerList[m_playersTurn].get_money() <= 0 && m_playerList[m_playersTurn].get_isAlive() == true ) {
        // Cycle through each property. If the current player owns property, and has a zero balance, they
        //  are still allowed to play as they can sell the property to get their balance above zero.
        for( int pCount = 0; pCount > MAX_PROPERTIES; pCount++ ) {
            // If a property is owned by the active player, and is not flagged for mortgage.
            if( m_propertyList[pCount].get_isMortgaged() == false &&
            	m_propertyList[pCount].get_ownedBy() == m_playerList[m_playersTurn].get_id() ) {

            	canPlay = true;
            }
        }
        // If the active player has no property, this is the end of the turn, and end of the game for this player.
        if( canPlay == false ) {
			// Kill the active player.
        	m_playerList[m_playersTurn].set_isAlive( false );
            fprintf( stderr, "Player: %i, is now out of the game!\n", m_playersTurn );
            m_turnDone = true;
        }
    }

    /*
     * Now for as long as this player's turn is not flagged as being over, handle the current state.
    */
    if( m_turnDone == false && m_playerList[m_playersTurn].get_isAlive() == true ) {
        handleState();
    }
	/*
	 * This is executed if the player is not allowed to continue their turn or its the end of a turn.
	*/
    else {
        // If the turnDone flag is true, increment to the next player in the list.
        // This code will increment the active player's turn.
    	bool foundGoodPlayer = false;
    	int nextPlayer = m_playersTurn++;

    	while( foundGoodPlayer == false ) {
    		// Make sure the next player isn't out of bounds.
    		if( nextPlayer >= NUM_PLAYERS ) {
    			nextPlayer = 0;
    		}
    		// If the next player in the list isn't alive, we will increment the counter.
    		if( m_playerList[nextPlayer].get_isAlive() == false ) {
    			nextPlayer++;
    		}
    		// If the next player in the list is alive, then they become the active player.
    		else if( m_playerList[nextPlayer].get_isAlive() == true ) {
    			m_playersTurn = nextPlayer;
    			// Ensure that the first turn flag is reset.
    			m_firstTurn = true;
    			// Ensure that we reset the turnDone flag.
    			m_turnDone = false;
    			// Ensure that the player flag for first roll is also reset.
    			m_playerList[m_playersTurn].set_firstRoll( true );
    			m_playerList[m_playersTurn].set_isMoving( false );
    			foundGoodPlayer = true;
    		}
    	}
    }
}

void MonopolyGame::halt() {

	// Cleanup all properties.
    for(int pCount = 0; pCount < MAX_PROPERTIES; pCount++) {
    	m_propertyList[pCount].cleanup();
    }

    // Cleanup all players.
    for(int pCount = 0; pCount < NUM_PLAYERS; pCount++) {
    	m_playerList[pCount].cleanup();
    }

    // Free the display device.
    if(m_alDisplayDevice) {
        al_destroy_display(m_alDisplayDevice);
    }

    // Free the board image bitmap.
    if(m_alBoardImage) {
        al_destroy_bitmap(m_alBoardImage);
    }

    // Nullify each player piece.
    for(int ppCount = 0; ppCount < PLAYER_PIECES_COUNT; ppCount++) {
    	if(m_alpieceImages[ppCount]) {
    		al_destroy_bitmap(m_alpieceImages[ppCount]);
    	}
    }

    // Free the loaded fonts.
    for(int fCount = 0; fCount < MAX_FONTS; fCount++) {
    	if(m_fontCollection[fCount]) {
    		al_destroy_font(m_fontCollection[fCount]);
    	}
    }

    // Free the FPS timer.
    if(m_alTimer) {
        al_destroy_timer(m_alTimer);
    }

    // Free the frame timer.
    if(m_alFrameTimer) {
        al_destroy_timer(m_alFrameTimer);
    }

    // Finally we will free the event queue.
    if(m_alEventQueue) {
        al_destroy_event_queue(m_alEventQueue);
    }
}
