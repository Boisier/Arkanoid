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
  * gameObj.texturesPath = NULL;
    gameObj.nbrTextures = 0;

    gameObj.toPrint = NULL;
    gameObj.nbrToPrint = 0;
    gameObj.printContent = EMPTY;
}

void theLoop()
{
    printf("Here\n");
    GLuint texture = getTexture("heart.jpg");
    GLuint texture2 = getTexture("heart.jpg");


    int inGame = true;
    while(inGame) 
    {
        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture2);
 
        // For Ortho mode, of course
        int X = 0;
        int Y = 0;
        int Width = 256;
        int Height = -256;
        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(X, Y, 0);
            glTexCoord2f(1, 0); glVertex3f(X + Width, Y, 0);
            glTexCoord2f(1, 1); glVertex3f(X + Width, Y + Height, 0);
            glTexCoord2f(0, 1); glVertex3f(X, Y + Height, 0);
        glEnd();


	      glBindTexture(GL_TEXTURE_2D, 0);
	      glDisable(GL_TEXTURE_2D);

        SDL_GL_SwapBuffers();

        SDL_Event e;
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
        
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
}

