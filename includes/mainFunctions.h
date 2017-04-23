# pragma once

void initGame();

void theLoop();

void doThings();

/** Core functions **/
void watcher();

char btnHandler();

/** Screen functions **/
void mainMenu();
void createMainMenu();

void playerSelection();
void createPlayerSelection();

void startGame();
void createPlayer(enum PlayerType type, int playerNbr);

void ingame();