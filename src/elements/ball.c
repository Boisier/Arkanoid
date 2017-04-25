#include "../../includes/game.h"

/** Create a new ball element width the defaultplateforme texture**/
Ball * createBall(float posX, float posY, int playerID)
{
	Ball * ball = allocate(sizeof(Ball));

	ball->x = posX;
	ball->y = posY;
	ball->size = gameObj.defVal.ball.size;

	ball->playerID = playerID;

	ball->glued = true;
	ball->gluedPlat = gameObj.players[playerID]->plateforme;

	ball->texture = getTexture("ball.png");

	ball->direction.x = 0;
	ball->direction.y = 0;
	ball->speed = 0;

	ball->bonusType = NONE;
	ball->bonusEnd = 0;

	return ball;
}

/** Print the ball on the screen**/
void printBall(Ball * ball)
{	
	float x, y, w, h;

	w = ball->size;
	h = ball->size;

	if(ball->glued)
	{
		if(ball->gluedPlat->pos == TOP)
			ball->y = ball->gluedPlat->y + gameObj.defVal.plateforme.height + ball->size / 2;
		else
			ball->y = ball->gluedPlat->y - gameObj.defVal.plateforme.height - ball->size / 2;

		ball->x = ball->gluedPlat->x + ball->glueOffsetX;
	}

	x = ball->x - ball->size / 2;
	y = ball->y - ball->size / 2;

	/*if(ball->gluedPlat->pos == BOTTOM)
		printf(">> %f : %f %f <<\n", ball->speed, x, y);*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ball->texture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(  x  ,   y  );
		glTexCoord2f(1, 0); glVertex2f(x + w,   y  );
		glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
		glTexCoord2f(0, 1); glVertex2f(  x  , y + h);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/** Free the ball, the texture is not free-ed **/
void freeBall(Ball * ball)
{
	free(ball);
}

void unglueBall(Ball * ball)
{
	bool actionKey;
    float speedFactor, relAngle, startAngle, speedRange;
    Plateforme * plat;

	/*Select actionKey based on plateforme Position*/
	if(ball->gluedPlat->pos == TOP)
		actionKey = gameObj.keys.space;
	else
		actionKey = gameObj.keys.up;
	
	/*Is this ball glued ?*/
	if(ball->glued && actionKey)
	{
		plat = ball->gluedPlat;
		
		/*Unglue this ball*/
		speedFactor = plat->speed / gameObj.defVal.plateforme.maxSpeed;
		relAngle = gameObj.defVal.ball.maxStartAngle / 100.0;
		startAngle = speedFactor * relAngle * plat->dirFactor;

		ball->direction.x = sin(startAngle);
		ball->direction.y = cos(startAngle);

		if(plat->pos == BOTTOM)
			ball->direction.y *= -1;

		speedRange = gameObj.defVal.ball.maxSpeed - gameObj.defVal.ball.minSpeed;

		ball->speed = gameObj.defVal.ball.minSpeed + speedRange * speedFactor;

		ball->glued = false;
	}
}

void moveBall(Ball * ball)
{
	ball->x += ball->direction.x * ball->speed;
	ball->y += ball->direction.y * ball->speed;

	/*Too much on the left ?*/
	if(ball->x < 0)
	{
		ball->x = abs(ball->x);
		ball->direction.x *= -1;
	}
	/*Too much on the right ?*/
	if(ball->x + ball->size > gameObj.WINDOW_WIDTH)
	{
		ball->x = gameObj.WINDOW_WIDTH - (ball->x + ball->size - gameObj.WINDOW_WIDTH);
		ball->direction.x *= -1;
	}	
}

bool ballLost(Ball * ball, int * player)
{
	if(ball->y - ball->size > gameObj.WINDOW_HEIGHT)
	{
		/*Lost by bottom player*/
		* player = 0;
		return true;
	}

	if(ball->y < 0)
	{
		/*Lost by top player*/
		* player = 1;
		return true;
	}

	return false;
}


void ballCollisions(Ball * ball)
{
	int i;
	Plateforme * plat;
	BaseSphere ballBase;
	BaseRect platBase;
	Collision col;

	ballBase.x = ball->x;
	ballBase.y = ball->y;
	ballBase.radius = ball->size / 2;

	for(i = 0; i < gameObj.nbrToPrint; ++i)
	{
		if(gameObj.toPrint[i].type == BALL   || 
		   gameObj.toPrint[i].type == BONUS  || 
		   gameObj.toPrint[i].type == BUTTON || 
		   gameObj.toPrint[i].type == PICTURE)
			continue;

		if(gameObj.toPrint[i].type == PLATEFORME)
		{
			plat = gameObj.toPrint[i].element.plateforme;
			
			platBase = getPlateformeBaseRect(plat);

			col = SphereRectCollision(ballBase, platBase, true);

			if(col.side != NO_COLLISION)
			{
				ballPlateformeCollision(ball, plat, col);
				continue;
			}
		}
	}
}

void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col)
{
	float speedFactor, speedDelta;

	switch (col.side) 
	{
		case NO_COLLISION: printf("NO_COLLISION\n"); break;
		case UNKNOWN: printf("UNKNOWN\n"); break;
		case TOP_SIDE: printf("TOP_SIDE\n"); break;
		case TOP_RIGHT_CORNER: printf("TOP_RIGHT_CORNER\n"); break;
		case RIGHT_SIDE: printf("RIGHT_SIDE\n"); break;
		case BOTTOM_RIGHT_CORNER: printf("BOTTOM_RIGHT_CORNER\n"); break;
		case BOTTOM_SIDE: printf("BOTTOM_SIDE\n"); break;
		case BOTTOM_LEFT_CORNER: printf("BOTTOM_LEFT_CORNER\n"); break;
		case LEFT_SIDE: printf("LEFT_SIDE\n"); break;
		case LEFT_TOP_CORNER: printf("LEFT_TOP_CORNER\n"); break;
	}

	if(col.side == TOP_SIDE || col.side == BOTTOM_SIDE)
	{
		speedFactor = plat->speed / gameObj.defVal.plateforme.maxSpeed;

		/*printf("%f\n", speedFactor);*/

		speedDelta = (10 * speedFactor) - 2;
		ball->speed += speedDelta;

		ball->speed = clamp(ball->speed, gameObj.defVal.ball.minSpeed, gameObj.defVal.ball.maxSpeed);

		ball->direction.y *= -1;

		if(col.side == TOP_SIDE)
		{
			ball->y += col.deltaTop;
		}
		else if(col.side == BOTTOM_SIDE)
		{
			ball->y += col.deltaBottom;
		}

		/*printf("%f\n", ball->speed);*/
		/*printf("> %f -- %f\n", ball->direction.y, ball->y);*/
	}
	else if(col.side == LEFT_SIDE || col.side == RIGHT_SIDE)
	{
		ball->direction.x *= -1;
	}
}
