#include "../../includes/game.h"

/** Create a new ball element width the defaultplateforme texture**/
Ball * createBall(float posX, float posY, int playerID, int BBox)
{
	Ball * ball = allocate(sizeof(Ball));

	ball->BBox = BBox;

	ball->x = posX;
	ball->y = posY;
	ball->size = gameObj.defVal.ball.size;

	ball->playerID = playerID;

	ball->glued = false;
	ball->gluedPlat = NULL;
	ball->gluedAt = 0;

	ball->texture = getTexture("ball.png");

	ball->direction.x = 0;
	ball->direction.y = 0;
	ball->speed = 0;

	ball->bonus = NONE;
	ball->bonusEnd = 0;

	return ball;
}


/** Create a new ball already glued to a plateforme **/
Ball * createGluedBall(float posX, float posY, int playerID)
{
	Ball * ball = createBall(0, 0, playerID, playerID);

	ball->glued = true;
	ball->gluedPlat = gameObj.game.players[playerID]->plateforme;
	ball->glueOffsetX = ball->gluedPlat->size * .5;
	ball->gluedAt = (int)SDL_GetTicks();

	return ball;
}


/** Reset the ball position and state **/
void resetBall(Ball * ball)
{
	ball->x = 0;
	ball->y = 0;
	ball->size = gameObj.defVal.ball.size;
	ball->BBox = gameObj.game.players[ball->playerID]->plateforme->BBox;

	ball->glued = true;
	ball->gluedPlat = gameObj.game.players[ball->playerID]->plateforme;
	ball->glueOffsetX = ball->gluedPlat->size * .5;
	ball->gluedAt = (int)SDL_GetTicks();

	ball->direction.x = 0;
	ball->direction.y = 0;
	ball->speed = 0;

	ball->bonus = NONE;
	ball->bonusEnd = 0;
}


/** Print the ball on the screen**/
void printBall(Ball * ball)
{	
	float x, y, w, h, angle;

	w = ball->size;
	h = ball->size;

	if(ball->glued)
	{
		/*Get ball position based on the plateforme position*/
		ball->y = ball->gluedPlat->y - ball->size * .5;
		ball->x = ball->gluedPlat->x + ball->glueOffsetX;
	}

	x = ball->x - ball->size * .5;
	y = ball->y + ball->size * .5;

	angle = bbAngle(ball->BBox);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ball->texture);
	
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(  x  ,   y  );
		glTexCoord2f(1, 0); glVertex2f(x + w,   y  );
		glTexCoord2f(1, 1); glVertex2f(x + w, y - h);
		glTexCoord2f(0, 1); glVertex2f(  x  , y - h);
	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


/** Free the ball, the texture is not free-ed **/
void freeBall(Ball * ball)
{
	free(ball);
	ball = NULL;
}


/** Build a circle for the given ball**/
Circle getBallCircle(Ball * ball)
{
	Circle circ;

	circ.BBox = ball->BBox;
	circ.position.x = ball->x;
	circ.position.y = ball->y;
	circ.radius = ball->size * .5;

	return circ;
}


/** Try to unglue the ball from it's plateforme **/
void unglueBall(Ball * ball)
{
	bool actionKey = false;
    float speedFactor, relAngle, startAngle, speedRange;
    Plateforme * plat;

	/*Is this ball glued ?*/
	if(!ball->glued)
		return;

	/*Select actionKey based on plateforme Position*/
	if(gameObj.game.players[ball->BBox]->type == HUMAN)
	{
		/*HUMAN*/
		if(gameObj.game.players[ball->BBox]->controls == 0)
			actionKey = gameObj.keys.up;
		else if(gameObj.game.players[ball->BBox]->controls == 1)
			actionKey = gameObj.keys.z;
		else /*if(gameObj.game.players[ball->BBox]->controls == 2)*/
			actionKey = gameObj.keys.b;
	}
	else if(rand() % 50 == 0 || gameObj.game.players[ball->BBox]->life == 0)
	{
		/*AI*/
		actionKey = true;
	}

	/*Can we unglue this ball ?*/
	if(actionKey || ball->gluedAt + gameObj.defVal.ball.gluedMaxDuration < (int)SDL_GetTicks())
	{
		/*Unglue this ball*/
		plat = ball->gluedPlat;
		
		/*Let's calculate ball start angle and velocity*/
		speedFactor = platSpeedFactor(plat);
		relAngle = gameObj.defVal.ball.maxAngle * .5 * speedFactor;
		startAngle = relAngle * plat->dirFactor;

		setBallDirection(ball, startAngle);

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

	/*The ball is no longer in it's bbox, we need to find it's new one*/
	if(ball->y > gameObj.game.bb.height)
		return;

	updateBallBBox(ball);

	if(!gameObj.game.bb.squared)
		return; /*Nothing else to do for a triangular BBox*/


	/*Handle side bounce for 1v1 BBox*/
	if(ball->x < -gameObj.game.bb.width * .5 || ball->x > gameObj.game.bb.width * .5)
		ball->direction.x *= -1;
}


/** Update the ball BBox to it's new one **/
void updateBallBBox(Ball * ball)
{
	Vector2D ballPos;

	/*Rotate the ball until it is in the BBox*/
	while(!inBBox(ball))
	{
		ballPos.x = ball->x;
		ballPos.y = ball->y;

		ballPos = rotateVector(ballPos, - gameObj.game.bb.angle);
		ball->direction = rotateVector(ball->direction, -gameObj.game.bb.angle);

		ball->x = ballPos.x;
		ball->y = ballPos.y;
		
		ball->BBox++;

		if(ball->BBox == gameObj.game.nbrPlayers)
			ball->BBox = 0; /*Loop*/
	}
}


/** Check if the ball is lost, return the playerID who lost it*/
bool ballLost(Ball * ball, int * player)
{
	if(ball->y > gameObj.game.bb.height)
	{
		/*This ball is lost*/
		* player = ball->BBox;
		return true;
	}

	return false;
}


/** Check for ball collisions **/
void ballCollisions(Ball * ball)
{
	Circle ballCirc, rotatedCirc;
	Collision col;
	Polygon * poly;
	float angle;
	int i;

	ballCirc = getBallCircle(ball);

	/*For every element*/
	for(i = 0; i < gameObj.nbrToPrint; ++i)
	{
		/*Ignore hidden elements*/
		if(!gameObj.toPrint[i]->display)
			continue;	

		/*Ignore elements withour interactions with the balls, and unused elements*/
		switch(gameObj.toPrint[i]->type)
		{
			case PLATEFORME:
				if(gameObj.game.players[gameObj.toPrint[i]->element.plateforme->BBox]->life == 0)
					continue;

				poly = getPlateformePolygon(gameObj.toPrint[i]->element.plateforme);
			break;
			case BRICK:
				poly = getBrickPolygon(gameObj.toPrint[i]->element.brick);
			break;
			case WALL:
				poly = getWallPolygon(gameObj.toPrint[i]->element.wall);
			break;
			default: continue; break;
		}	

		rotatedCirc = ballCirc;

		/*Adjust ballCirc angle if different BBox*/ 
		if(poly->BBox != ballCirc.BBox)
		{
			angle = (poly->BBox - ballCirc.BBox) * -gameObj.game.bb.angle;
			
			rotatedCirc.position = rotateVector(rotatedCirc.position, angle);
		}

		/*Check collisions with the plateforme*/
		col = circleRectCollision(rotatedCirc, poly);
			
		if(col.side == NO_COLLISION)
		{
		freePolygon(poly);
			continue; /*No collision, let's move on*/
		}

		/*If it's a plateforme*/
		if(gameObj.toPrint[i]->type == PLATEFORME)
		{
			ballPlateformeCollision(ball, gameObj.toPrint[i]->element.plateforme, col);
		}
		else if(gameObj.toPrint[i]->type == BRICK)
		{
			ballBrickCollision(ball, gameObj.toPrint[i]->element.brick, poly, col, i);
		}
		else if(gameObj.toPrint[i]->type == WALL)
		{
			ballWallCollision(ball, gameObj.toPrint[i]->element.wall, col);
		}

		freePolygon(poly);

		return;
		/*Let's check again in case we are hitting something else*/
		/*i--;*/
	}
}


/**Handle ball collision with plateforme **/
void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col)
{
	float speedFactor, speedDelta, colPosPerc, angle;

	/*Do nothing if there's no collision*/
	if(col.side == UNKNOWN || col.side == NO_COLLISION)
		return;
	
	/*Glue the ball if the plateforme is sticky*/
	if(plat->sticky)
	{
		ball->glued = true;
		ball->gluedPlat = plat;
		ball->glueOffsetX = ball->x - plat->x;
		ball->gluedAt = (int)SDL_GetTicks();
		ball->BBox = plat->BBox;

		return;
	}

	switch(col.side)
	{
		case NO_COLLISION:
		case UNKNOWN:
		case CORNER:
			return;
		break;
		case TOP_SIDE:
		case BOTTOM_SIDE:
		case TOP_LEFT_CORNER:
		case TOP_RIGHT_CORNER:
		case BOTTOM_RIGHT_CORNER:
		case BOTTOM_LEFT_CORNER:

			/*Update ball speed based on plateforme speed*/
			speedFactor = platSpeedFactor(plat);
			speedDelta = (10 * speedFactor) - 2;
			ball->speed += speedDelta;

			ball->speed = clamp(ball->speed, gameObj.defVal.ball.minSpeed, gameObj.defVal.ball.maxSpeed);

			/*Update ball direction based on collision point*/
			colPosPerc = (col.point.x - plat->x) / plat->size;
			angle = (gameObj.defVal.ball.maxAngle * colPosPerc) - (gameObj.defVal.ball.maxAngle * .5);

			setBallDirection(ball, angle);
			
			ball->direction.y *= -1;

			if(col.side == TOP_SIDE || col.side == TOP_LEFT_CORNER || col.side == TOP_RIGHT_CORNER)
				ball->y -= col.delta;
			else /*if(col.side == BOTTOM_SIDE || col.side == BOTTOM_LEFT_CORNER || col.side == BOTTOM_RIGHT_CORNER)*/
				ball->y += col.delta;
			
		break;
		case LEFT_SIDE:
		case RIGHT_SIDE:
			/*Just reverse X direction*/
			ball->direction.x *= -1;

			if(col.side == LEFT_SIDE)
				ball->x -= col.delta;
			else /*if(col.side == RIGHT_SIDE)*/
				ball->x += col.delta;
		break;
	}

	updateBallBBox(ball);
}


/** Handle collision between a ball and a wall **/
void ballWallCollision(Ball * ball, Wall * wall, Collision col)
{
	switch(col.side)
	{
		case NO_COLLISION:
		case UNKNOWN:
		case CORNER:
			return;
		break;
		case TOP_SIDE:
		case BOTTOM_SIDE:
		case TOP_LEFT_CORNER:
		case TOP_RIGHT_CORNER:
		case BOTTOM_RIGHT_CORNER:
		case BOTTOM_LEFT_CORNER:
			
			/*Just reverse Y direction*/
			ball->direction.y *= -1;

			if(col.side == TOP_SIDE || col.side == TOP_LEFT_CORNER || col.side == TOP_RIGHT_CORNER)
				ball->y -= col.delta;
			else /*if(col.side == BOTTOM_SIDE || col.side == BOTTOM_LEFT_CORNER || col.side == BOTTOM_RIGHT_CORNER)*/
				ball->y += col.delta;
			
		break;
		case LEFT_SIDE:
		case RIGHT_SIDE:
			/*Just reverse X direction*/
			ball->direction.x *= -1;

			if(col.side == LEFT_SIDE)
				ball->x -= col.delta;
			else /*if(col.side == RIGHT_SIDE)*/
				ball->x += col.delta;
		break;
	}
}


/**Handle collision between a ball and a brick**/
void ballBrickCollision(Ball * ball, Brick * brick, Polygon * brickPoly, Collision col, int brickID)
{
	Vector2D edge, normal, u, v;
	float mag;

	/*Make sure we are in the same BBox*/
	changePolyBBox(brickPoly, ball->BBox);

	/*Get the reflection vector (called normal here)*/
	/*CORNER -> BISECTOR*/
	/*EDGE -> NORMAL*/
	switch (col.side) 
	{
		case NO_COLLISION:
		case UNKNOWN:
		case CORNER: 
			return;	
		break;
		case TOP_LEFT_CORNER:
			u = subVector(brickPoly->points[1], brickPoly->points[0]);
			v = subVector(brickPoly->points[3], brickPoly->points[0]);
		break;
		case TOP_SIDE:
			edge = subVector(brickPoly->points[1], brickPoly->points[0]);
		break;
		case TOP_RIGHT_CORNER:
			u = subVector(brickPoly->points[0], brickPoly->points[1]);
			v = subVector(brickPoly->points[2], brickPoly->points[1]);
		break;
		case RIGHT_SIDE:
			edge = subVector(brickPoly->points[2], brickPoly->points[1]);
		break;
		case BOTTOM_RIGHT_CORNER:
			u = subVector(brickPoly->points[1], brickPoly->points[2]);
			v = subVector(brickPoly->points[3], brickPoly->points[2]);
		break;
		case BOTTOM_SIDE: 
			edge = subVector(brickPoly->points[3], brickPoly->points[2]);
		break;
		case BOTTOM_LEFT_CORNER:
			u = subVector(brickPoly->points[0], brickPoly->points[3]);
			v = subVector(brickPoly->points[2], brickPoly->points[3]);
		break;
		case LEFT_SIDE: 
			edge = subVector(brickPoly->points[0], brickPoly->points[3]);
		break;
	}

	if(col.side == TOP_SIDE || col.side == RIGHT_SIDE || col.side == BOTTOM_SIDE || col.side == LEFT_SIDE)
	{
		/*Rotate and normalize*/
		mag = norm(edge);
		normal.x = edge.y / mag;
		normal.y = -edge.x / mag;
	}
	else
	{
		/*Normalize*/
		normal = addVector(multVector(v, norm(u)), multVector(u, norm(v)));
		mag = norm(normal);

		normal.x = -normal.x / mag;
		normal.y = -normal.y / mag;
	}

	/*Reflect ball direction with the reflection vector*/
	ball->direction = subVector(ball->direction, multVector(normal, 2 * dotP(ball->direction, normal)));

	updateBallBBox(ball);

	brickHit(brick, brickID);
}


/** Set ball direction from a given angle **/
void setBallDirection(Ball * ball, float angle)
{
	ball->direction.x = sin(angle * DEGTORAD);
	ball->direction.y = cos(angle * DEGTORAD);
}


/** Update bonus for the given ball **/
void updateBallBonus(Ball * ball)
{
	float offset;

	if(ball->bonus != NONE && ball->bonusEnd - (int)SDL_GetTicks() <= 0)
	{
		ball->bonus = NONE;
		ball->bonusEnd = 0;

		offset = (ball->size - gameObj.defVal.ball.size) * .5;

		ball->size = gameObj.defVal.ball.size;
		ball->x += offset;
	}
}
