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

/** Handle movement of the players **/
void playerMovements();

/** Handle movement of the balls **/
void ballMovements();





/*******************/
/** GAME FUNCTION **/
/*******************/

/** Handle the main menu **/
void mainMenu();

/** Create the main menu **/
void createMainMenu();

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
