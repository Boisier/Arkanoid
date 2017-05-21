#include "../includes/game.h"

int main(int argc, char** argv) 
{
    /*Init the GameObj*/
    initGame();

    theLoop();
    
	glDisable(GL_TEXTURE_2D);
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
