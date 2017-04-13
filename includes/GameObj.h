#pragma once

typedef enum _GameState
{
    MAINMENU
} GameState;

typedef struct _GameObj
{
    /** Window Properties **/
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    /** Game Properties **/
    int nbrPlayers;

    /** Loop storage **/
    GameState gameState;


	/** Textures **/
	GLuint * textures;
	char ** texturesPath;
	int nbrTextures;


} GameObj;

GameObj gameObj;