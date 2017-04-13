//
//  game.c
//  Arkanopong
//

#include "../includes/game.h"

/**
 * Init gameObj with default values
 */
void initGame()
{
    gameObj.WINDOW_WIDTH = 1200;
    gameObj.WINDOW_HEIGHT = 800;

    gameObj.gameState = MAINMENU;


    gameObj.textures = NULL;
  * gameObj.texturesPath = NULL;
    gameObj.nbrTextures = 0;
}


