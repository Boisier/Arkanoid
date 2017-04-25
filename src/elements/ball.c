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
		/*Get ball position based on the plateforme position*/
		if(ball->gluedPlat->pos == TOP)
			ball->y = ball->gluedPlat->y + ball->size / 2;
		else
			ball->y = ball->gluedPlat->y - ball->size / 2;

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

	/*Is this ball glued ?*/
	if(!ball->glued)
		return;

	/*Select actionKey based on plateforme Position*/
	if(ball->gluedPlat->pos == TOP)
		actionKey = gameObj.keys.space;
	else
		actionKey = gameObj.keys.up;
	
	/*Can we unglue this ball ?*/
	if(actionKey)
	{
		/*Unglue this ball*/
		plat = ball->gluedPlat;
		
		/*Let's calculate ball start angle and velocity*/
		speedFactor = platSpeedFactor(plat);
		relAngle = gameObj.defVal.ball.maxAngle / 2 * speedFactor;
		startAngle = relAngle * plat->dirFactor;

		setBallDirection(ball, startAngle);

		if(plat->pos == BOTTOM)
			ball->direction.y *= -1;

		speedRange = gameObj.defVal.ball.maxSpeed - gameObj.defVal.ball.minSpeed;
		ball->speed = gameObj.defVal.ball.minSpeed + speedRange * speedFactor;

		/*Finally, set the ball as unglued*/
		ball->gluedPlat = NULL;
		ball->glued = false;
	}
}

/** Move the ball along its direction. Bounce on screen sides*/
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
	if(ball->x + ball->size > gameObj.wWidth)
	{
		ball->x = gameObj.wWidth - (ball->x + ball->size - gameObj.wWidth);
		ball->direction.x *= -1;
	}	
}

/** Check if the ball is lost, return playerID who lost it*/
bool ballLost(Ball * ball, int * player)
{
	if(ball->y - ball->size > gameObj.wHeight)
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

/** CHeck for ball collisions **/
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

	/*For every element*/
	for(i = 0; i < gameObj.nbrToPrint; ++i)
	{
		/*Ignore elements withour interactions with the balls, and unused elements*/
		if(gameObj.toPrint[i].type == BALL   || 
		   gameObj.toPrint[i].type == BONUS  || 
		   gameObj.toPrint[i].type == BUTTON || 
		   gameObj.toPrint[i].type == PICTURE||
		   !gameObj.toPrint[i].display)
			continue;
		
		/*If it's a plateforme*/
		if(gameObj.toPrint[i].type == PLATEFORME)
		{
			plat = gameObj.toPrint[i].element.plateforme;
			
			/*CHeck collisions with the plateforme*/
			platBase = getPlateformeBaseRect(plat);
			col = SphereRectCollision(ballBase, platBase, true);

			if(col.side == NO_COLLISION)
				continue; /*No collision, let's go to the next element*/

			/*Collisions -> handle it*/
			ballPlateformeCollision(ball, plat, col);
		}
	}
}

/**Handle ball collision with plateforme **/
void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col)
{
	float speedFactor, speedDelta, colPosPerc, angle;

	/*switch (col.side) 
	{
		case NO_COLLISION: printf("NO_COLLISION\n"); break;
		case UNKNOWN: printf("UNKNOWN\n"); break;
		case TOP_SIDE: printf("TOP_SIDE\n"); break;
		case RIGHT_SIDE: printf("RIGHT_SIDE\n"); break;
		case BOTTOM_SIDE: printf("BOTTOM_SIDE\n"); break;
		case LEFT_SIDE: printf("LEFT_SIDE\n"); break;
	}*/

	/**Do nothing if there's no collision**/
	if(col.side == UNKNOWN || col.side == NO_COLLISION)
		return;

	if(col.side == TOP_SIDE || col.side == BOTTOM_SIDE)
	{
		/*Update ball speed based on plateforme speed*/
		speedFactor = platSpeedFactor(plat);
		speedDelta = (10 * speedFactor) - 2;
		ball->speed += speedDelta;

		ball->speed = clamp(ball->speed, gameObj.defVal.ball.minSpeed, gameObj.defVal.ball.maxSpeed);

		/*Update ball direction based on collision point*/
		colPosPerc = (col.x - plat->x) / plat->size;
		angle = (gameObj.defVal.ball.maxAngle * colPosPerc) - (gameObj.defVal.ball.maxAngle / 2);

		setBallDirection(ball, angle);

		if(plat->pos == BOTTOM)
			ball->direction.y *= -1;
	}
	else if(col.side == LEFT_SIDE || col.side == RIGHT_SIDE)
	{
		ball->direction.x *= -1;
	}

	/*Move the ball away from the plateforme by the distance it was inside it*/
	switch (col.side) 
	{
		case TOP_SIDE: ball->y += col.deltaTop; break;
		case RIGHT_SIDE: ball->y += col.deltaRight; break;
		case BOTTOM_SIDE: ball->y += col.deltaBottom; break;
		case LEFT_SIDE: ball->y += col.deltaLeft; break;
		default: break;
	}
}

/** Set ball direction from a given angle **/
void setBallDirection(Ball * ball, float angle)
{
	ball->direction.x = sin(angle / DEGTORAD);
	ball->direction.y = cos(angle / DEGTORAD);
}
