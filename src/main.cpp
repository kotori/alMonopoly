/*
	monopoly turn structure

	1.) <BUY/SELL/TRADE PHASE>
	2.) <ROLL PHASE>
	3.) Move according to roll
	4.) Action according to new space.
	5.) If passed go, collect $200.00
	6.) If roll from step2, was a double, go back to step 1.
	7.) <BUY/SELL/TRADE PHASE>
	8.) End Turn
*/

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

float cameraPosition[2] = { 0, 0 };

//
// MONOPOLY GAME CLASS
//
class MonopolyGame {

public:
    MonopolyGame();
    ~MonopolyGame();

    int init();
    int loadResources(); /*!< Ensure's all gfx & sfx resources are loaded. */
    int run();
    void halt();

    int randomNum(int max);

private:
    void draw();
    void reset();

    Database sqlConn;

    int buildPropertyList(); /*!< Populates the propertyList. */
    MonopolyProperty propertyList[MAX_PROPERTIES]; /*!< Maximum number of properties. */

    MonopolyPlayer playerList[NUM_PLAYERS];

    // Allegro5 objects.
    ALLEGRO_DISPLAY		*alDisplayDevice;
    ALLEGRO_EVENT_QUEUE		*alEventQueue;
    ALLEGRO_TIMER		*alTimer;
    ALLEGRO_TIMER		*alFrameTimer;
    ALLEGRO_DISPLAY_MODE	alDisplayData;
    ALLEGRO_KEYBOARD_STATE 	alKeyState;
    ALLEGRO_TRANSFORM		alCameraTransform;

    // Actual game objects.
    int playersTurn; 		/*!< Current player's turn. */
    int numPlayers;			/*!< Number of players. */

    GameMode activeGameMode;

    int doublesRollCounter; /*!< Keeps track the number of times a double was rolled. */

    bool exitGame;	/*!< Boolean flag for exiting the main loop. */
    bool redrawScreen;	/*!< Boolean flag for handling screen redraws. */
    bool isActive;	/*!< Boolean flag for handling animation renders. */
};

MonopolyGame::MonopolyGame() {
    // Nullfiy
    alDisplayDevice = NULL;
    alEventQueue = NULL;
    alTimer = NULL;
    alFrameTimer = NULL;
}

MonopolyGame::~MonopolyGame() {
    // Empty
}

void cameraUpdate(float *cameraPosition, float x, float y, int width, int height)
{
    cameraPosition[0] = -(WINDOW_WIDTH / 2) + (x + width / 2);
    cameraPosition[1] = -(WINDOW_HEIGHT / 2) + (y + height / 2);
 
    if(cameraPosition[0] < 0)
        cameraPosition[0] = 0;
    if(cameraPosition[1] < 0)
        cameraPosition[1] = 0;
}

int MonopolyGame::buildPropertyList() {
    std::string tempQuery = "";
    std::string tempString = "";
    int tempNum = 0;

    fprintf(stderr, "Preparing to build properties...\n");

    for( int i=0; i<MAX_PROPERTIES; i++ ) {
        // Set NAME
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "name", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectStr(tempString, tempQuery.c_str())) {
            propertyList[i].set_name(tempString);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
            return -1;
        }
	// Set ID
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "id", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_id(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set PURCHASE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "price", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set MORTGAGE PRICE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "mortgage", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_mortgagePrice(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set PRICE PER HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_house", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_pricePerHouse(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set PRICE PER HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "per_hotel", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_pricePerHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT FOR HOTEL
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_hotel", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rentHotel(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT FOR 1 HOUSE
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_1_house", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent1House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT FOR 2 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_2_house", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent2House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT FOR 3 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_3_house", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent3House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }
        // Set RENT FOR 4 HOUSES
        tempQuery = sqlConn.Format("SELECT %s FROM %s WHERE %s = %i", "rent_4_house", DB_PROPERTY_TABLE, "id", i+1);
        if(!sqlConn.SelectInt(tempNum, tempQuery.c_str())) {
            propertyList[i].set_rent4House(tempNum);
        }
        else {
            fprintf(stderr, "SQL QUERY ERROR\n");
        }

        fprintf(stderr, "Built Property: %s [ID: %i] \n", propertyList[i].get_name().c_str(), propertyList[i].get_id());
    }
    return 0;
}

void MonopolyGame::reset() {
    playersTurn = 0;
    for(int i=0; i<NUM_PLAYERS; i++) {
        playerList[i].set_location(0);
        playerList[i].set_score(0);
        if(activeGameMode == EASY) {
            playerList[i].set_money(3500);
        }
        else if(activeGameMode == NORMAL) {
            playerList[i].set_money(2500);
        }
        else if(activeGameMode == DIFFICULT) {
            playerList[i].set_money(1500);
        }
    }
}

int MonopolyGame::randomNum(int max) {
    int rNum = rand() % max + 1;
    return rNum;
}

void MonopolyGame::draw() {
    // Drawing logic goes here...

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

int MonopolyGame::loadResources() {

    if(buildPropertyList())
    {
        fprintf(stderr, "Failure building Property List, Please ensure the database is available!\n");
        return -1;
    }

    return 0;
}

int MonopolyGame::init() {

    // First set the main loop exit condition to false.
    exitGame = false;

    srand((unsigned)time(0));

    activeGameMode = EASY;

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

    al_init_font_addon(); // Initialize the font addon.
    al_init_ttf_addon(); // Initialize the ttf (True Type Font) addon.

    // Gets the biggest resolution supported.
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplayDevice = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!alDisplayDevice) {
        fprintf(stderr, "Failed to create Display: %i x %i\n", alDisplayData.width, alDisplayData.height);
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

    return 0;
}

int MonopolyGame::run() {

    al_flip_display();

    al_start_timer(alTimer);
    al_start_timer(alFrameTimer);

    while(!exitGame)
    {
        ALLEGRO_EVENT alEvent;
        al_wait_for_event(alEventQueue, &alEvent);
        al_get_keyboard_state(&alKeyState);

        if(alEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            exitGame = true;
        }

        else if(alEvent.type == ALLEGRO_EVENT_TIMER)
        {
            if(alEvent.timer.source == alTimer)
            {
		cameraUpdate(cameraPosition, playerList[playersTurn].get_x(), playerList[playersTurn].get_y(), 32, 32);
            }

            else if(alEvent.timer.source == alFrameTimer)
            {
		for(int i=0; i<NUM_PLAYERS; i++) {
			if(playerList[i].get_money() > 0) {
				if(playersTurn == i) {
				    int tIndex = playerList[i].get_animationX();
				    tIndex += al_get_bitmap_width(playerList[i].get_image()) / 3;

				    if(playerList[i].get_animationX() >= al_get_bitmap_width(playerList[i].get_image())) {
				    	playerList[i].set_animationX(0);
				    }
				}
				else {
				    playerList[i].set_animationX(32);
				}
		 
				// This should be set to the direction of the character, where directions equal:
				//  DOWN, LEFT, RIGHT, UP
				playerList[i].set_animationY(playerList[i].get_direction());
			}
		}
            }
            redrawScreen = true;
        }

        if(redrawScreen) {
            redrawScreen = false;
            draw();
        }
    }
    return 0;
}

void MonopolyGame::halt() {

    for(int i=0; i<MAX_PROPERTIES; i++) {
        propertyList[i].cleanup();
    }

    if(alDisplayDevice) {
        al_destroy_display(alDisplayDevice);
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

int main(int argc, char **argv) {

    // Create new instance of the MonopolyGame object.
    MonopolyGame *monopoly = new MonopolyGame();

    // Initialize the Allegro routines
    if(monopoly->init()) {
        return -1;
    }

    // Load Resources
    if(monopoly->loadResources()) {
        return -1;
    }

    // Run the game loop until the exit condition is met.
    if(monopoly->run()) {
        monopoly->halt();
        return -1;
    }

    // Cleanup when we are done.
    monopoly->halt();

    return 0;
}

