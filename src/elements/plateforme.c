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

	plateforme->sticky = false;

	plateforme->bonus = NONE;
	plateforme->bonusEnd = 0;

	return plateforme;
}

/** Print the plateforme on the screen**/
void printPlateforme(Plateforme * plateforme)
{	
	float x, y, w, h, angle;

	x = plateforme->x;
	y = plateforme->y;
	w = plateforme->size;
	h = gameObj.defVal.plateforme.height;
	angle = bbAngle(plateforme->BBox);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, plateforme->texture);

	glPushMatrix();
	glRotatef(angle, 0, 0, 1);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(  x  ,   y  );
		glTexCoord2f(1, 0); glVertex2f(x + w,   y  );
		glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
		glTexCoord2f(0, 1); glVertex2f(  x  , y + h);
	glEnd();

	if(gameObj.game.bb.squared)
	{
		glBegin(GL_LINE_LOOP);

			glVertex2f(0                , 0);
			glVertex2f(gameObj.game.bb.width / 2, gameObj.game.bb.height);
			glVertex2f(-gameObj.game.bb.width / 2, gameObj.game.bb.height);

		glEnd();
	}

	glPopMatrix();

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
	platBase.topLeftY = plat->y;

	platBase.topRightX = plat->x + plat->size;
	platBase.topRightY = plat->y;

	platBase.bottomRightX = plat->x + plat->size;
	platBase.bottomRightY = plat->y - gameObj.defVal.plateforme.height;
	
	platBase.bottomLeftX = plat->x;
	platBase.bottomLeftY = plat->y - gameObj.defVal.plateforme.height;
	
	return platBase;
}

/** Return the plateforme speed ratio to the max speed */
float platSpeedFactor(Plateforme * plat)
{
	return plat->speed / gameObj.defVal.plateforme.maxSpeed;
}
