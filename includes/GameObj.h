#pragma once

typedef enum _GameState
{
    EMPTY,
    MAINMENU,
    PLAYERSELECTION,
    EXITING
} GameState;


typedef struct _keyPressed
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool space;
    bool enter;
    bool a;
    bool z;
    bool esc;
} KeyPressed;

typedef struct _GameObj
{
    /** Window Properties **/
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    /** Theme **/
    char theme[256];

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

    /** key watched **/
    KeyPressed keys;

    /** Interactions holders **/
    Button * currentlySelectedBtn;

} GameObj;

GameObj gameObj;