#include "../includes/game.h"

/**
 * Init gameObj with default values
 */
void initGame()
{
    gameObj.WINDOW_WIDTH = 1200;
    gameObj.WINDOW_HEIGHT = 800;

    strcpy(gameObj.theme, "themes/default/");

    gameObj.platformeHeight = 5;
    gameObj.platMaxSpeed = 25;
    gameObj.platSpeedUpFactor = 2.5;

    gameObj.brickWidth = 50;
    gameObj.brickHeight = 20;
    
    gameObj.ballSize = 15;
    gameObj.defaultSpeed = 5; 

    gameObj.bonusSize = 10;
    gameObj.bonusSpeed = 10;

    gameObj.gameState = MAINMENU;

    gameObj.textures = NULL;
    gameObj.nbrTextures = 0;

    gameObj.toPrint = NULL;
    gameObj.nbrToPrint = 0;
    gameObj.printContent = EMPTY;

}

void theLoop()
{
    int inGame = true;
    Uint32 startTime, elapsedTime;
    SDL_Event e;
    
    while(inGame) 
    {
        startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        doThings();

        printScreen();

        SDL_GL_SwapBuffers();

        while(SDL_PollEvent(&e)) 
        {
            if(e.type == SDL_QUIT) {
                inGame = false;
                break;
            }

            switch(e.type) 
            {
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case 'q' :
                        case SDLK_ESCAPE :
                      inGame = false;
                            break;
                        default : break;
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
}

void doThings()
{
    switch(gameObj.gameState)
    {
        case MAINMENU:
            mainMenu();
        break;
        default:
            return;
    }
}

/*Handle main menu*/
void mainMenu()
{
    if(gameObj.printContent != MAINMENU)
    {
        createMainMenu();
    }
}

/*Create Mainmenu elements*/
void createMainMenu()
{
    Picture * bigLogo = createPicture(-200, 200, "face.jpg");

    addToPrint(bigLogo, PICTURE);

    gameObj.printContent = MAINMENU;
}
