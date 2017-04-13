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

    GLuint texture = getTexture("heart.jpg");
    GLuint texture2 = getTexture("face.jpg");

    GLuint texture3 = gameObj.textures[1];

    int loop = 1;

    while(loop) 
    {
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture3);
 
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
                loop = 0;
                break;
            }

            switch(e.type) 
            {
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case 'q' :
                        case SDLK_ESCAPE :
                            loop = 0;
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
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
