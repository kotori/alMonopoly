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
    alDisplayDevice = NULL;
    alEventQueue = NULL;
    alTimer = NULL;
    alFrameTimer = NULL;
    alBoardImage = NULL;

    // Nullify each font in the font collection.
    for(int fCount = 0; fCount < MAX_FONTS; fCount++) {
    	fontCollection[fCount] = NULL;
    }

    // Nullify each player piece.
    for(int ppCount = 0; ppCount < PLAYER_PIECES_COUNT; ppCount++) {
    	alpieceImages[ppCount] = NULL;
    }

    // Reset the camera position.
    alCamera.cameraPosition[Positions::X_POS] = 0;
    alCamera.cameraPosition[Positions::Y_POS] = 0;
}

MonopolyGame::~MonopolyGame() {
    // Empty
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
	tempQuery = sqlConn.Format( "SELECT COUNT(*) FROM %s WHERE %s = %i", DB_PROPERTY_TABLE, "set_id", prop.get_group() );
	if( sqlConn.SelectInt( numPropertiesInSet, tempQuery.c_str() ) ) {
		fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
	}
	
	// Build a list of the friendly properties.
	int friendlyPropertiesId[numPropertiesInSet];
	for( int pCount=0; pCount < numPropertiesInSet; pCount++ ) {
		if( pCount == 0 ) {
			friendlyPropertiesId[pCount] = prop.get_id();
		}
		else if(pCount == 1){
			tempQuery = sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i LIMIT 1", 
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id() );
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
				friendlyPropertiesId[pCount] = tNum;
			}
		}
		else {
			tempQuery = sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i AND %s != %i LIMIT 1", 
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id(), "id", friendlyPropertiesId[1]);
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
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
					if( !propertyList[activeProperty].get_isMortgaged() ) {
						propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_OWNED );
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
					int val = propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_OWNED_SET );
				}
				break;
			case PropertyValue::VAL_2_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_1_HOUSE );
				}
				break;
			case PropertyValue::VAL_3_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_2_HOUSE );
				}
				break;
			case PropertyValue::VAL_4_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = propertyList[activeProperty].get_pricePerHouse();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_3_HOUSE );
				}
				break;
			case PropertyValue::VAL_1_HOTEL:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyPropertiesId[pCount];
					int val = propertyList[activeProperty].get_pricePerHotel();
					// Give the player the mortgage value of the property.
					plyr.set_money( val );
					// Change the value of this property.
					propertyList[activeProperty].set_propertyValue( PropertyValue::VAL_4_HOUSE );
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
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "name", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectStr(tempString, tempQuery.c_str())) {
            propertyList[propertyCount].set_name(tempString);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set ID
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_id(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PURCHASE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "price", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set MORTGAGE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "mortgage", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_mortgagePrice(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_pricePerHouse(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_hotel", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_pricePerHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_hotel", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rentHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 1 HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_1_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent1House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 2 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_2_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent2House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 3 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_3_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent3House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 4 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_4_house", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent4House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set x location.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "x", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_x(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            //return -1;
        }
        // Set y location.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "y", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_y(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            //return -1;
        }

        // Set the property Type.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "type", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            switch(tempNum) {
            case TYPE_RAILROAD:
                propertyList[propertyCount].set_propertyType(TYPE_RAILROAD);
                break;
            case TYPE_UTILITY:
                propertyList[propertyCount].set_propertyType(TYPE_UTILITY);
                break;
            case TYPE_PROPERTY:
                propertyList[propertyCount].set_propertyType(TYPE_PROPERTY);
                break;
            case TYPE_CARD:
                propertyList[propertyCount].set_propertyType(TYPE_CARD);
                break;
            case TYPE_TAX:
                propertyList[propertyCount].set_propertyType(TYPE_TAX);
                break;

            }
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        
        
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	propertyList[propertyCount].set_group(tempNum);
        }
        else {
        	fprintf(stderr, "SQL QUERY ERROR\n");
        	return -1;
        }
        
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", tCounter);
        if(!sqlConn.SelectStr(tempString, tempQuery.c_str())) {
           	propertyList[propertyCount].set_groupName(tempString);
        }
        else {
        	fprintf(stderr, "SQL QUERY ERROR\n");
        	return -1;
        }
        
        

        fprintf(stderr, "Built Property: %s [ID: %i] \n", propertyList[propertyCount].get_name().c_str(), propertyList[propertyCount].get_id());
    }
    return 0;
}

// This should be called everytime the game is reset.
void MonopolyGame::reset() {

	fprintf( stderr, "Resetting Game.\n");

    playersTurn = 0;

    // Reset each player attribute.
    for(int pCount = 0; pCount < NUM_PLAYERS; pCount++) {
        playerList[pCount].set_location(0);
        playerList[pCount].set_score(0);

        if(activeGameMode == GameMode::EASY) {
            playerList[pCount].set_money(3500);
        }
        else if(activeGameMode == GameMode::NORMAL) {
            playerList[pCount].set_money(2500);
        }
        else if(activeGameMode == GameMode::DIFFICULT) {
            playerList[pCount].set_money(1500);
        }
    }

    // Reset each property attribute.
    for(int pCount = 0; pCount < MAX_PROPERTIES; pCount++) {
    	propertyList[pCount].set_isMortgaged(false);
    	propertyList[pCount].set_isOwned(false);
    	propertyList[pCount].set_ownedBy(0);
    }
}

int* MonopolyGame::rollDice(size_t numOfDice, int sides) {
	// Create a new integer array to fill with random integers.
	int *ret = new int[numOfDice];
	// For the number of dice passed, generate a random number no greater than sides.
	for(size_t dieCount = 0; dieCount < numOfDice; dieCount++) {
		// Add +1 to the result to ensure a value above 0.
		int ranNum = rand() % sides + 1;
		// Copy the random number into the array to be returned.
		ret[dieCount] = ranNum;
	}
	// Return the populated random integer array.
	return ret;
}

void MonopolyGame::handleDiceRoll(MonopolyPlayer &plyr) {
	// Roll 2 six sided dice.
	diceRoll = rollDice( NUM_DICE, NUM_SIDES_PER_DIE );

	// If this is the player's first dice roll, reset the doubles counter.
	if( plyr.get_firstRoll() ) {
		doublesRollCounter = 0;
		plyr.set_firstRoll( false );
	}

	// If die 1 equals die 2, then increment the doubles counter.
	if( diceRoll[0] == diceRoll[1] ) {
		//  Need to ensure that when this player's turn ends, this counter is reset to 0.
		doublesRollCounter++;
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
	al_draw_text(
		fontCollection[0],
		al_map_rgb( 0, 0, 0 ),
		x + offset,
		y + offset,
		NULL,
		buffer );

	// Finally we print the foreground text.
	al_draw_text(
		fontCollection[0],
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
		fontCollection[0],
		al_map_rgb( 0, 0, 0 ),
		x + offset,
		y + offset,
		ALLEGRO_ALIGN_CENTRE,
		buffer );

	// Finally we print the foreground text.
	al_draw_text(
		fontCollection[0],
		col,
		x,
		y,
		ALLEGRO_ALIGN_CENTRE,
		buffer );
}

void MonopolyGame::draw() {

	// Based upon the current turnstate we will display different things.
	switch(turnState) {
		case NULL_STATE:
			drawTextCen( al_map_rgb( 255, 0, 0 ), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "MONOPOLY!" );
			break;
		case PRE_GAME:
			// This is where I need to implement a 'menu class' for user input.
			// enum MenuSelection { PRE_GAME_MENU = 0, IN_GAME_MENU = 1, POST_GAME_MENU = 2 };
			//  menu[PRE_GAME_MENU].draw();
			// Later a logic portion will need to be added as well.
			// menu[PRE_GAME_MENU].doLogic();
			break;
		case PRE_TURN:
		case ROLL_PHASE:
		case MOVE_PHASE:
		case REACT_PHASE:
		case POST_TURN:
			// Draw the board first.
			al_draw_bitmap( alBoardImage, 0, 0, 0 );

			// Next Draw each of the player's.
			//  This should be fine tuned later to only draw 'alive' players.
			for( int playerCounter = 0; playerCounter < numPlayers; playerCounter++ ) {
				playerList[playersTurn].draw();
			}
			break;
		case TRADING:
			// TODO: implement trading.
			break;
	}

	//
	// Print some debugging info.
	//
	drawText( al_map_rgb( 0, 200, 240 ), 0, 0, "Num. of Players: %i", numPlayers );
	drawText( al_map_rgb( 0, 200, 240 ), 0, 32, "Current Player: %i", playersTurn );
	drawText( al_map_rgb( 0, 200, 240 ), 0, 64, "Current Player's Money: $%i", playerList[playersTurn].get_money() );

	// Next, we flip the display to make the changes visible,
	//  and clear the background before we print again.
	al_flip_display();
	al_clear_to_color( al_map_rgb( 255, 255, 255 ) );
}

int MonopolyGame::loadResources() {

    // Normalize the paths.
    ALLEGRO_PATH *path = al_get_standard_path( ALLEGRO_RESOURCES_PATH );
    al_append_path_component( path, "etc" );
    al_change_directory( al_path_cstr( path, '/' ) );  // change the working directory
    al_destroy_path( path );

    // Load the board's bitmap
    std::string sFileName = "board.jpg";
    alBoardImage = al_load_bitmap( sFileName.c_str() );
    if( !alBoardImage ) {
        fprintf( stderr, "Failed loading Board Bitmap: %s\n", sFileName.c_str() );
        return -1;
    }

    // Load the font set.
    sFileName = "DejaVuSans.ttf";
    // Start building the fonts from 24pt.
    int initialSize = 24;
    for( int fontCounter = 0; fontCounter < MAX_FONTS; fontCounter++ ) {
        fontCollection[fontCounter] = al_load_font( sFileName.c_str(), initialSize, 0 );
        initialSize += 10;
        if( !fontCollection[fontCounter] ) {
            fprintf( stderr, "Failed loading Font: %s\n", sFileName.c_str() );
            return -1;
        }
    }

    // Now load the possible player pieces into memory.
    //  The piece file locations are stored in the database.
    for( int pieceCounter = 0; pieceCounter < PLAYER_PIECES_COUNT; pieceCounter++ ) {
    	std::string tempQuery = "";
    	std::string filepath = "";
    	tempQuery = sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i", "path", DB_PIECES_TABLE, "id", pieceCounter );
    	if( !sqlConn.SelectStr( filepath, tempQuery.c_str() ) ) {
    		// Load the bitmap from the filename from above.
    		alpieceImages[pieceCounter] = al_load_bitmap( filepath.c_str() );
    		if( !alpieceImages[pieceCounter] ) {
    			// TODO: CURRENTLY I HAVE NO PLACE HOLDER IMAGES, SO THE APPLICATION WILL FAIL HERE!
    			fprintf( stderr, "Failed loading Bitmap: %s\n", filepath.c_str() );
    			return -1;
    		}
    	}
    	else {
    		fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
    		return -1;
    	}


    }

    // Finally we will pull the property list from the database.
    if( buildPropertyList() )
    {
        fprintf( stderr, "Failure building Property List, Please ensure the database is available!\n" );
        return -1;
    }

    return 0;
}

int MonopolyGame::init() {

    // First set the main loop exit condition to false.
    exitGame = false;

    // Seed the random number generator.
    srand((unsigned)time(0));

    activeGameMode = GameMode::EASY;
    turnState = TurnState::NULL_STATE;

    // Now begin initializing the Allegro library.
    if(!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro.\n");
        return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "Failed to initialize the Keyboard!\n");
        return -1;
    }

    alTimer = al_create_timer(1.0 / MAX_FPS);
    if(!alTimer) {
        fprintf(stderr, "Failed to create Timer!\n");
        return -1;
    }

    alFrameTimer = al_create_timer(1.0 / MAX_FRAME_FPS);
    if(!alFrameTimer) {
        fprintf(stderr, "Failed to create Timer!\n");
        al_destroy_timer(alTimer);
        return -1;
    }

    al_init_image_addon(); // Initialize the image addon.

    al_init_font_addon(); // Initialize the font addon.
    al_init_ttf_addon(); // Initialize the ttf (True Type Font) addon.

    // Setup the display device in windowed mode.
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplayDevice = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!alDisplayDevice) {
        fprintf(stderr, "Failed to create Display: %i x %i\n", WINDOW_WIDTH, WINDOW_HEIGHT);
        al_destroy_timer(alTimer);
        al_destroy_timer(alFrameTimer);
        return -1;
    }

    // Create an event queue. This is where timed events, etc are pulled from.
    alEventQueue = al_create_event_queue();
    if(!alEventQueue) {
        fprintf(stderr, "Failed to create Event Queue!\n");
        al_destroy_display(alDisplayDevice);
        al_destroy_timer(alTimer);
        al_destroy_timer(alFrameTimer);
        return -1;
    }

    // Register an event source per input.
    al_register_event_source(alEventQueue, al_get_display_event_source(alDisplayDevice));
    al_register_event_source(alEventQueue, al_get_timer_event_source(alTimer));
    al_register_event_source(alEventQueue, al_get_timer_event_source(alFrameTimer));
    al_register_event_source(alEventQueue, al_get_keyboard_event_source());

    // Ensure the mouse cursor is not visible.
    al_hide_mouse_cursor(alDisplayDevice);

    // Set some default values for the camera.
    alCamera.cameraPosition[Positions::X_POS] = 0;
    alCamera.cameraPosition[Positions::Y_POS] = 0;

    return 0;
}

int MonopolyGame::run() {

	// Reset the game's run data.
	reset();

    // Start the timer's so that revisions are properly drawn.
    al_start_timer( alTimer );
    al_start_timer( alFrameTimer );
    fprintf(stderr, "Current turnstate: %i\n", turnState );

	// Perform the first display update.
    al_flip_display();

    while( !exitGame )
    {
        ALLEGRO_EVENT alEvent;
        al_wait_for_event( alEventQueue, &alEvent );
        al_get_keyboard_state( &alKeyState );

        // If the user clicks the window's 'close (X)' button.
        if( alEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
        {
        	// Exit the game.
            exitGame = true;
        }
        // Or if the user hits the 'ESC' key.
        else if( al_key_down( &alKeyState, ALLEGRO_KEY_ESCAPE ) )
        {
        	// Exit the game.
            exitGame = true;
        }

        // Process any timed events that have been triggered.
        else if( alEvent.type == ALLEGRO_EVENT_TIMER )
        {
            if( alEvent.timer.source == alTimer )
            {
            	// We will update the camera position only if the timer has been triggered.
            	cameraUpdate( alCamera.cameraPosition, // X and Y location of the camera.
            		playerList[playersTurn].get_x(), // Player's x position.
            		playerList[playersTurn].get_y(), // Player's y position.
            		32, 32 ); // Width and height of player's image.
            }

            else if( alEvent.timer.source == alFrameTimer )
            {
				for( int pCounter = 0; pCounter < NUM_PLAYERS; pCounter++ ) {
					// If this player has more than '0' money.
					if( playerList[pCounter].get_money() > 0 ) {
						// For each player, update the image we will display this frame.
						playerList[pCounter].animationLogic();
					}
				}
            }
            // Since the screen has been updated, we want to flag the screen to be redrawn.
            redrawScreen = true;

            // Do some camera transform magic.
            al_identity_transform( &alCamera.alCameraTransform );

            // Adjust the camera so that it is centered over our player's position.
            al_translate_transform( &alCamera.alCameraTransform,
            	-alCamera.cameraPosition[Positions::X_POS],
            	-alCamera.cameraPosition[Positions::Y_POS] );

            // Push the camera's changes.
            al_use_transform( &alCamera.alCameraTransform );
        }

        if( redrawScreen ) {
            redrawScreen = false;
            draw();
        }
    }
    return 0;
}

void MonopolyGame::handleMove() {
    //
}

void MonopolyGame::handleTurn() {
	// Handle current state.
	switch(turnState) {
		case NULL_STATE:
			break;
		case PRE_GAME:
			// menu[PRE_GAME_MENU].doLogic();
			break;
		case PRE_TURN:
			break;
		case ROLL_PHASE:
			break;
		case MOVE_PHASE:
			break;
		case REACT_PHASE:
			break;
		case POST_TURN:
			break;
		case TRADING:
			break;
	}
}

void MonopolyGame::halt() {

	// Cleanup all properties.
    for(int pCount = 0; pCount < MAX_PROPERTIES; pCount++) {
        propertyList[pCount].cleanup();
    }

    // Cleanup all players.
    for(int pCount = 0; pCount < NUM_PLAYERS; pCount++) {
    	playerList[pCount].cleanup();
    }

    // Free the display device.
    if(alDisplayDevice) {
        al_destroy_display(alDisplayDevice);
    }

    // Free the board image bitmap.
    if(alBoardImage) {
        al_destroy_bitmap(alBoardImage);
    }

    // Nullify each player piece.
    for(int ppCount = 0; ppCount < PLAYER_PIECES_COUNT; ppCount++) {
    	if(alpieceImages[ppCount]) {
    		al_destroy_bitmap(alpieceImages[ppCount]);
    	}
    }

    // Free the loaded fonts.
    for(int fCount = 0; fCount < MAX_FONTS; fCount++) {
    	if(fontCollection[fCount]) {
    		al_destroy_font(fontCollection[fCount]);
    	}
    }

    // Free the FPS timer.
    if(alTimer) {
        al_destroy_timer(alTimer);
    }

    // Free the frame timer.
    if(alFrameTimer) {
        al_destroy_timer(alFrameTimer);
    }

    // Finally we will free the event queue.
    if(alEventQueue) {
        al_destroy_event_queue(alEventQueue);
    }
}
