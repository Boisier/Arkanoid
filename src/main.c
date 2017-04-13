#include "../includes/game.h"

int main(int argc, char** argv) 
{
    //Init the GameObj
    initGame();

    //Open the window
    if(!createWindow())
    {
        return 0;
    }

    theLoop();
    
	glDisable(GL_TEXTURE_2D);
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
