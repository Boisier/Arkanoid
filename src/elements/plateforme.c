#include "../../includes/game.h"

Plateforme * createPlateforme(float posX, float posY)
{
	Plateforme * plateforme = allocate(sizeof(Plateforme));

	plateforme->x = posX;
	plateforme->y = posY;

	plateforme->speed = 0;

	plateforme->bonus = NONE;
	plateforme->bonusEnd = 0;

	return plateforme;
}

void printPlateforme(Plateforme * plateforme)
{
	GLuint texture = getTexture("plateforme.png");
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(plateforme->x, plateforme->y);
		glTexCoord2f(1, 0); glVertex2f(plateforme->x + plateforme->size, plateforme->y);
		glTexCoord2f(1, 1); glVertex2f(plateforme->x + plateforme->size, plateforme->y - gameObj.platformeHeight);
		glTexCoord2f(0, 1); glVertex2f(plateforme->x, plateforme->y - gameObj.platformeHeight);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void freePlateforme(Plateforme * plateforme)
{
	free(plateforme);
}