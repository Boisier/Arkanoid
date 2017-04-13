#include "../includes/game.h"

void reshape(unsigned int width, unsigned int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-600., 600., -400., 400.);
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

bool createWindow()
{
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) 
    {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return false;
    }

    setVideoMode(gameObj.WINDOW_WIDTH, gameObj.WINDOW_HEIGHT);

    SDL_WM_SetCaption("Clickodrome", NULL);

	return true;
}
