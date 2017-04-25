#include "../../includes/game.h"

/** Create a new plateforme element width the defaultplateforme texture**/
Plateforme * createPlateforme(float posX, float posY, enum PlayerPos pos)
{
	Plateforme * plateforme = allocate(sizeof(Plateforme));

	plateforme->x = posX;
	plateforme->y = posY;
	plateforme->size = gameObj.defVal.plateforme.size;
	plateforme->pos = pos;

	plateforme->speed = 0;
	plateforme->texture = getTexture("plateforme.png");

	plateforme->sticky = false;

	plateforme->bonus = NONE;
	plateforme->bonusEnd = 0;

	return plateforme;
}

/** Print the plateforme on the screen**/
void printPlateforme(Plateforme * plateforme)
{	
	float x, y, w, h;

	x = plateforme->x;
	y = plateforme->y;
	w = plateforme->size;
	h = gameObj.defVal.plateforme.height;

	/*Reverse texture for top plateforme*/
	if(plateforme->pos == TOP)
		h *= -1;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, plateforme->texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(  x  ,   y  );
		glTexCoord2f(1, 0); glVertex2f(x + w,   y  );
		glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
		glTexCoord2f(0, 1); glVertex2f(  x  , y + h);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/** Free the plateforme, the texture is not free-ed **/
void freePlateforme(Plateforme * plateforme)
{
	free(plateforme);
}

/** Generate a baseRect for the given plateforme* */
BaseRect getPlateformeBaseRect(Plateforme * plat)
{
	BaseRect platBase;

	platBase.topLeftX = plat->x;
	platBase.topRightX = plat->x + plat->size;
	platBase.bottomRightX = plat->x + plat->size;
	platBase.bottomLeftX = plat->x;
	
	if(plat->pos == TOP)
	{
		platBase.topLeftY = plat->y;
		platBase.topRightY = plat->y;
		platBase.bottomRightY = plat->y - gameObj.defVal.plateforme.height;
		platBase.bottomLeftY = plat->y - gameObj.defVal.plateforme.height;
	}
	else
	{
		platBase.topLeftY = plat->y;
		platBase.topRightY = plat->y;
		platBase.bottomRightY = plat->y - gameObj.defVal.plateforme.height;
		platBase.bottomLeftY = plat->y - gameObj.defVal.plateforme.height;
	}

	return platBase;
}

/** Return the plateforme speed ratio to the max speed */
float platSpeedFactor(Plateforme * plat)
{
	return plat->speed / gameObj.defVal.plateforme.maxSpeed;
}
