#include "../../includes/game.h"

/** Create a new plateforme element width the defaultplateforme texture**/
Plateforme * createPlateforme(float posX, float posY)
{
	Plateforme * plateforme = allocate(sizeof(Plateforme));

	plateforme->x = posX;
	plateforme->y = posY;
	plateforme->size = gameObj.defVal.plateforme.size;

	plateforme->speed = 0;
	plateforme->texture = getTexture("plateforme.png");

	plateforme->bonus = NONE;
	plateforme->bonusEnd = 0;

	return plateforme;
}

/** Print the plateforme on the screen**/
void printPlateforme(Plateforme * plateforme)
{	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, plateforme->texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(plateforme->x, plateforme->y);
		glTexCoord2f(1, 0); glVertex2f(plateforme->x + plateforme->size, plateforme->y);
		glTexCoord2f(1, 1); glVertex2f(plateforme->x + plateforme->size, plateforme->y - gameObj.defVal.plateforme.height);
		glTexCoord2f(0, 1); glVertex2f(plateforme->x, plateforme->y - gameObj.defVal.plateforme.height);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/** Free the plateforme, the texture is not free-ed **/
void freePlateforme(Plateforme * plateforme)
{
	free(plateforme);
}
