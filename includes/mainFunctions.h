# pragma once

/********************/
/** ROOT FUNCTIONS **/
/********************/

/** Main loop of the app**/
void theLoop();

/** Call current game state function **/
void doThings();






/************************/
/** IGNITING FUNCTIONS **/
/************************/

/** Init gameObj with default values **/
void initGame();

/** Inite the game obj with default values*/
bool initGameObj();

/** Init the SDL **/
bool initSDL();

/** Load elements depending on the SDL library **/
void loadSDLDependants();



/********************/
/** CORE FUNCTIONS **/
/********************/

/** Watch for user events **/
void watcher();

/** Handle menu interactions **/
char btnHandler(Mix_Chunk * sound);

/**Define the playable area for each player**/
void defineBoundingBox();

/** Handle movement of the players **/
void playerMovements();

/** Handle movement of the balls **/
void ballMovements();

/** Create the bricks for every player with the given level*/
void createBricks(char * levelName);

/** Handle movement of the bonus **/
void bonusMovements();

/** Add a life to the given player and update display **/
void addLifePlayer(Player * player);

/** Remove a life to the given player and update display **/
void removeLifePlayer(Player * player);




/*******************/
/** GAME FUNCTION **/
/*******************/

/** Load ressources such as fonts and menu textures **/
void loadGame();

/** Handle the main menu **/
void mainMenu();

/** Create the main menu **/
void createMainMenu();

/** Theme switch screen **/
void themeSelection();

/** Create theme selection screen **/
void createThemeSelection();

/** Handle nbr of player selection **/
void playerSelection();

/** Create the nbr of player selector **/
void createPlayerSelection();

/** Select the bricks layout **/
void levelSelection();

/** Create the brick layout selection **/
void createLevelSelection();

/** Start the game **/
void startGame();

/** Create a new player **/
void createPlayer(enum PlayerType type, int playerNbr);

/** Handle ingame events **/
void ingame();

/** create the game board **/
void createGameBoard();

/** Create the pause menu*/
void createPauseMenu();

/** Create the start game countdown animation **/
void createStartGameAnimation();

/** Open the pause menu */
void enterPause();

/*restarte the game*/
void hidePause();
void quitPause();

/** Display endgame messages **/
void endgame();
