#pragma once

typedef enum _GameState
{
    EMPTY,
    MAINMENU
} GameState;

typedef struct _GameObj
{
    /** Window Properties **/
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;


    /** Elements default values **/
    int platformeHeight;
    float platMaxSpeed;
    float platSpeedUpFactor;

    int brickWidth;
    int brickHeight;
    float ballSize;
    float defaultSpeed; 

    float bonusSize;
    float bonusSpeed;

    /** Game Properties **/
    int nbrPlayers;

    /** Loop storage **/
    GameState gameState;

	/** Textures **/
	GLuint * textures;
	char ** texturesPath;
	int nbrTextures;

    /** Printing**/
    PrintElement * toPrint;
    int nbrToPrint;
    GameState printContent;

} GameObj;

GameObj gameObj;