# pragma once

/********************/
/** ROOT FUNCTIONS **/
/********************/

/** Init gameObj with default values **/
void initGame();

/** Main loop of the app**/
void theLoop();

/** Call current game state function **/
void doThings();





/********************/
/** CORE FUNCTIONS **/
/********************/

/** Watch for user events **/
void watcher();

/** Handle menu interactions **/
char btnHandler();

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

/** Start the game **/
void startGame();

/** Create a new player **/
void createPlayer(enum PlayerType type, int playerNbr);

/** Handle ingame events **/
void ingame();

/** create the game board **/
void createGameBoard();
