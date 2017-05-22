#include "../../includes/game.h"

void reshape(unsigned int width, unsigned int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gameObj.wWidth * .5, gameObj.wWidth * .5, -gameObj.wHeight * .5, gameObj.wHeight * .5);
}

void setVideoMode(unsigned int width, unsigned int height) 
{
    if(NULL == SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) 
	{
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        exit(EXIT_FAILURE);
    }

    reshape(width, height);
}
