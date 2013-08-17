
#include <iostream>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "common.h"
#include "database.h"
#include "property.h"
#include "player.h"
#include "game.h"

MonopolyGame::MonopolyGame() {
    // Nullfiy
    alDisplayDevice = NULL;
    alEventQueue = NULL;
    alTimer = NULL;
    alFrameTimer = NULL;

    for(int fCount = 0; fCount < 3; fCount++) {
    	fontCollection[fCount] = NULL;
    }

    alBoardImage = NULL;

    alCamera.cameraPosition[Positions::X_POS] = 0;
    alCamera.cameraPosition[Positions::Y_POS] = 0;
}

MonopolyGame::~MonopolyGame() {
    // Empty
}

void MonopolyGame::cameraUpdate(float *cameraPosition, float x, float y, int width, int height) {
    cameraPosition[Positions::X_POS] = -(WINDOW_WIDTH / 2) + (x + width / 2);
    cameraPosition[Positions::Y_POS] = -(WINDOW_HEIGHT / 2) + (y + height / 2);

    if(cameraPosition[Positions::X_POS] < 0) {
        cameraPosition[Positions::X_POS] = 0;
    }

    if(cameraPosition[Positions::Y_POS] < 0) {
        cameraPosition[Positions::Y_POS] = 0;
    }
}

bool MonopolyGame::mortgageProperty(MonopolyProperty &prop, MonopolyPlayer &plyr) {
	// TODO: This needs to be wrapped into a property management class.
	// 		  There is too much grouping that needs to be done.
	bool success = false;
	int numPropertiesInSet = 0;
	std::string tempQuery = "";

	tempQuery = sqlConn.Format( "SELECT COUNT(*) FROM %s WHERE %s = %i", DB_PROPERTY_TABLE, "set_id", prop.get_group() );
	if( sqlConn.SelectInt( numPropertiesInSet, tempQuery.c_str() ) ) {
		fprintf( stderr, "SQL QUERY ERROR: %s\n", tempQuery.c_str() );
	}
	
	// Build a list of the friendly properties.
	int friendlyProperties[numPropertiesInSet];
	for( int pCount=0; pCount < numPropertiesInSet; pCount++ ) {
		if( pCount == 0 ) {
			friendlyProperties[pCount] = prop.get_id();
		}
		else if(pCount == 1){
			tempQuery = sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i LIMIT 1", 
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id() );
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
				friendlyProperties[pCount] = tNum;
			}
		}
		else {
			tempQuery = sqlConn.Format( "SELECT %s FROM %s WHERE %s = %i AND %s != %i AND %s != %i LIMIT 1", 
				"id", DB_PROPERTY_TABLE, "set_id", prop.get_group(), "id", prop.get_id(), "id", friendlyProperties[1]);
			fprintf( stderr, "Executing: %s\n", tempQuery.c_str() );
			int tNum = 0;
			if( sqlConn.SelectInt( tNum, tempQuery.c_str() ) ) {
				friendlyProperties[pCount] = tNum;
			}
		}
	}
	
	// First ensure this player owns this property.
	if( prop.get_ownedBy() == plyr.get_id() ) {
		
		switch( prop.get_propertyValue() ) {
		
			case PropertyValue::VAL_NULL:
				// Null type, do nothing.
				break;
			case PropertyValue::VAL_OWNED:
				// Give the player the mortgage value of the property.
				plyr.set_money( prop.get_mortgagePrice() );
				// Mark the property as mortgaged.
				prop.set_isMortgaged( true );
				// Relinquish ownership of this property.
				prop.set_ownedBy( 0 );
				success = true;
				break;
			case PropertyValue::VAL_OWNED_SET:
				// Give the player the mortgage value of the property.
				plyr.set_money( prop.get_mortgagePrice() );
				// Mark the property as mortgaged.
				prop.set_isMortgaged( true );
				// Change the value of this property.
				prop.set_propertyValue( PropertyValue::VAL_OWNED );
				success = true;
				break;
			case PropertyValue::VAL_1_HOUSE:
				for( int pCount = 0; pCount < numPropertiesInSet; pCount++ ) {
					// Give the player the mortgage value of the property.
					int activeProperty = friendlyProperties[pCount];
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
					int activeProperty = friendlyProperties[pCount];
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
					int activeProperty = friendlyProperties[pCount];
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
					int activeProperty = friendlyProperties[pCount];
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
					int activeProperty = friendlyProperties[pCount];
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
	
	bool success = false;
	
	// If the property is not already owned.
	if( !prop.get_isOwned() ) {
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
	// The property has not been sold.
	return success;
}

int MonopolyGame::buildPropertyList() {
    std::string tempQuery = "";
    std::string tempString = "";
    int tempNum = 0;

    fprintf(stderr, "Preparing to build properties...\n");

    for( int propertyCount=0; propertyCount<MAX_PROPERTIES; propertyCount++ ) {
        // Set NAME
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "name", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectStr(tempString, tempQuery.c_str())) {
            propertyList[propertyCount].set_name(tempString);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set ID
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "id", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_id(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PURCHASE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "price", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set MORTGAGE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "mortgage", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_mortgagePrice(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_house", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_pricePerHouse(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set PRICE PER HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_hotel", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_pricePerHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_hotel", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rentHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 1 HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_1_house", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent1House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 2 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_2_house", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent2House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 3 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_3_house", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent3House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set RENT FOR 4 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_4_house", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_rent4House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
        // Set x location.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "x", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_x(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            //return -1;
        }
        // Set y location.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "y", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[propertyCount].set_y(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            //return -1;
        }

        // Set the property Type.
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "type", DB_PROPERTY_TABLE, "id", propertyCount+1);
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
        
        
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", propertyCount+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
        	propertyList[propertyCount].set_group(tempNum);
        }
        else {
        	fprintf(stderr, "SQL QUERY ERROR\n");
        	return -1;
        }
        
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "set_id", DB_PROPERTY_TABLE, "id", propertyCount+1);
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
    playersTurn = 0;

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
}

int MonopolyGame::randomNum(int max) {
    return rand() % max + 1;
}

void MonopolyGame::drawText(ALLEGRO_COLOR col, int x, int y, const char *msg, ...) {

	char buffer[256];
	va_list ap;

	va_start( ap, msg );
	vsprintf( buffer, msg, ap );
	va_end( ap );

	al_draw_text(
		fontCollection[0],
		al_map_rgb( 0, 0, 0 ),
		x+2,
		y+2,
		NULL,
		buffer );

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

	al_draw_text(
			fontCollection[0],
			al_map_rgb( 0, 0, 0 ),
			x+2,
			y+2,
			ALLEGRO_ALIGN_CENTRE,
			buffer );

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
			drawTextCen( al_map_rgb( 255, 0, 0 ), WINDOW_WIDTH/2, WINDOW_HEIGHT/2, "MONOPOLY!" );
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
			al_draw_bitmap( alBoardImage, 0, 0, NULL );

			// Next Draw each of the player's.
			//  This should be fine tuned later to only draw 'alive' players.
			for( int playerCounter = 0; playerCounter < numPlayers; playerCounter++) {
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
	drawText( al_map_rgb( 255, 255, 255 ), 0, 0, "Num. of Players: %i", numPlayers );
	drawText( al_map_rgb( 255, 255, 255 ), 0, 32, "Current Player: %i", playersTurn );
	drawText( al_map_rgb( 255, 255, 255 ), 0, 64, "Current Player's Money: $%i", playerList[playersTurn].get_money() );

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
    int initialSize = 24;
    for( int fontCounter=0; fontCounter<3; fontCounter++ ) {
        fontCollection[fontCounter] = al_load_font( sFileName.c_str(), initialSize, 0 );
        initialSize += 10;
        if( !fontCollection[fontCounter] ) {
            fprintf( stderr, "Failed loading Font: %s\n", sFileName.c_str() );
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

    al_init_image_addon();

    al_init_font_addon(); // Initialize the font addon.
    al_init_ttf_addon(); // Initialize the ttf (True Type Font) addon.

    al_set_new_display_flags(ALLEGRO_OPENGL);
    // Gets the biggest resolution supported.
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplayDevice = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!alDisplayDevice) {
        fprintf(stderr, "Failed to create Display: %i x %i\n", WINDOW_WIDTH, WINDOW_HEIGHT);
        al_destroy_timer(alTimer);
        al_destroy_timer(alFrameTimer);
        return -1;
    }

    alEventQueue = al_create_event_queue();
    if(!alEventQueue) {
        fprintf(stderr, "Failed to create Event Queue!\n");
        al_destroy_display(alDisplayDevice);
        al_destroy_timer(alTimer);
        al_destroy_timer(alFrameTimer);
        return -1;
    }

    al_register_event_source(alEventQueue, al_get_display_event_source(alDisplayDevice));
    al_register_event_source(alEventQueue, al_get_timer_event_source(alTimer));
    al_register_event_source(alEventQueue, al_get_timer_event_source(alFrameTimer));
    al_register_event_source(alEventQueue, al_get_keyboard_event_source());

    al_hide_mouse_cursor(alDisplayDevice);

    alCamera.cameraPosition[Positions::X_POS] = 0;
    alCamera.cameraPosition[Positions::Y_POS] = 0;

    return 0;
}

int MonopolyGame::run() {

    al_flip_display();

    al_start_timer( alTimer );
    al_start_timer( alFrameTimer );

    while( !exitGame )
    {
        ALLEGRO_EVENT alEvent;
        al_wait_for_event( alEventQueue, &alEvent );
        al_get_keyboard_state( &alKeyState );

        if( alEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
        {
            exitGame = true;
        }
        else if( al_key_down( &alKeyState, ALLEGRO_KEY_ESCAPE ) )
        {
            exitGame = true;
        }

        // Process any timed events that have been triggered.
        else if( alEvent.type == ALLEGRO_EVENT_TIMER )
        {
            if( alEvent.timer.source == alTimer )
            {
            	// We will update the camera position only if the timer has been triggered.
            	cameraUpdate( alCamera.cameraPosition, playerList[playersTurn].get_x(), playerList[playersTurn].get_y(), 32, 32 );
            }

            else if( alEvent.timer.source == alFrameTimer )
            {
				for( int pCounter = 0; pCounter < NUM_PLAYERS; pCounter++ ) {
					// If this player has more than '0' money.
					if( playerList[pCounter].get_money() > 0 ) {
						// For each player, update the image we will display this frame.
						playerList[pCounter].animationFrameLogic();
					}
				}
            }
            // Since the screen has been updated, we want to flag the screen to be redrawn.
            redrawScreen = true;

            // Do some camera transform magic.
            al_identity_transform( &alCamera.alCameraTransform );

            al_translate_transform( &alCamera.alCameraTransform,
            		-alCamera.cameraPosition[Positions::X_POS],
            		-alCamera.cameraPosition[Positions::Y_POS] );

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

    for(int pCount = 0; pCount < MAX_PROPERTIES; pCount++) {
        propertyList[pCount].cleanup();
    }

    for(int pCount = 0; pCount < NUM_PLAYERS; pCount++) {
    	playerList[pCount].cleanup();
    }

    if(alDisplayDevice) {
        al_destroy_display(alDisplayDevice);
    }

    if(alBoardImage) {
        al_destroy_bitmap(alBoardImage);
    }

    for(int fCount = 0; fCount < 3; fCount++) {
    	if(fontCollection[fCount]) {
    		al_destroy_font(fontCollection[fCount]);
    	}
    }

    if(alTimer) {
        al_destroy_timer(alTimer);
    }

    if(alFrameTimer) {
        al_destroy_timer(alFrameTimer);
    }

    if(alEventQueue) {
        al_destroy_event_queue(alEventQueue);
    }
}
