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

	if(gameObj.game.guidelines)
	{
		glBegin(GL_LINE_LOOP);

			/*glVertex2f(0                , 0);*/
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

/** Generate a polygon for the given plateforme* */
Polygon getPlateformePolygon(Plateforme * plat)
{
	Polygon poly;

	poly.BBox = plat->BBox;

	poly.points = allocate(sizeof(Vector2D) * 4);
	poly.nbrPoints = 4;

	/* EDGES ORDER : TOP RIGHT BOTTOM LEFT */

	poly.points[0].x = plat->x;
	poly.points[0].y = plat->y;

	poly.points[1].x = plat->x + plat->size;
	poly.points[1].y = plat->y;

	poly.points[2].x = plat->x + plat->size;
	poly.points[2].y = plat->y + gameObj.defVal.plateforme.height;
	
	poly.points[3].x = plat->x;
	poly.points[3].y = plat->y + gameObj.defVal.plateforme.height;
	
	return poly;
}

/** Return the plateforme speed ratio to the max speed */
float platSpeedFactor(Plateforme * plat)
{
	return plat->speed / gameObj.defVal.plateforme.maxSpeed;
}

/** Update bonus for the given plateforme **/
void updatePlateformeBonus(Plateforme * plate)
{
	if(plate->sticky && plate->stickyEnd - (int)SDL_GetTicks() <= 0)
	{
		plate->sticky = false;
		plate->stickyEnd = 0;
	}

	if(plate->bonus != NONE && plate->bonusEnd - (int)SDL_GetTicks() <= 0)
	{
		plate->bonus = NONE;
		plate->bonusEnd = 0;
		plate->size = gameObj.defVal.plateforme.size;
	}
}

/** Return the closest ball circle to the plateforme**/
Circle closestBall(Plateforme * plate)
{
	Circle ballCircle, closestCircle;
	Vector2D plateCenter;
	float closestDistanceSquared = FLT_MAX, distanceSquared;
	int i;

	plateCenter.x = plate->x + plate->size / 2;
	plateCenter.y = plate->y;

	closestCircle.position = plateCenter;

	for(i = 0; i < gameObj.nbrToPrint; ++i)
	{
		if(gameObj.toPrint[i].type != BALL || gameObj.toPrint[i].display == false)
			continue; /*Only care about balls*/

		if(gameObj.toPrint[i].element.ball->glued)
			continue; /*Ignore glued balls*/

		ballCircle = getBallCircle(gameObj.toPrint[i].element.ball);
		
		changeCircleBBox(&ballCircle, plate->BBox);

		distanceSquared = normSquared(subVector(ballCircle.position, plateCenter));

		if(distanceSquared < closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			closestCircle = ballCircle;
		}
	}

	return closestCircle;
}
