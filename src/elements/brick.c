#include "../../includes/game.h"

/** Create a new brick element **/
Brick * createBrick(float Xpos, float level, int height, int type, int BBox)
{
	Brick * brick = allocate(sizeof(Button));
	float topWidth, bottomWidth, topBrickWidth, bottomBrickWidth;

	topWidth = bbWidthAt(gameObj.game.bb.height - level);
	topBrickWidth = topWidth / gameObj.game.bb.gridW;
	bottomWidth = bbWidthAt(gameObj.game.bb.height - level - height);
	bottomBrickWidth = bottomWidth / gameObj.game.bb.gridW;

	topWidth *= .5;
	bottomWidth *= .5;

	brick->BBox = BBox;

	/*Define brick position*/
	brick->topLeft.x = -topWidth + topBrickWidth * Xpos;
	brick->topLeft.y = level;

	brick->topRight.x = -topWidth + topBrickWidth * (Xpos + 1);
	brick->topRight.y = level;

	brick->bottomLeft.x = -bottomWidth + bottomBrickWidth * Xpos;
	brick->bottomLeft.y = level + height;

	brick->bottomRight.x = -bottomWidth + bottomBrickWidth * (Xpos + 1);
	brick->bottomRight.y = level + height;

	brick->strength = 1;

	/*Get brick texture(s)*/
	switch(type)
	{
		case 1: 
			brick->type = BASIC;
			brick->texture = getTexture("brick/basic.png"); 
		break;
		case 2: 
			brick->type = INVINCIBLE;
			brick->texture = getTexture("brick/invincible.png"); 
		break;
		case 3: 
			brick->type = STRONG;
			brick->strength = 2;
			brick->texture = getTexture("brick/strong_0.png"); 
			brick->texture1 = getTexture("brick/strong_1.png"); 
		break;
		case 4: 
			brick->type = VERYSTRONG;
			brick->strength = 3;
			brick->texture = getTexture("brick/veryStrong_0.png"); 
			brick->texture1 = getTexture("brick/veryStrong_1.png"); 
			brick->texture2 = getTexture("brick/veryStrong_2.png"); 
		break;
		case 5: 
			brick->type = TRANSPARENT;
			brick->texture = getTexture("brick/transparent.png"); 
		break;
	}

	return brick;
}


/** Print the brick on the screen on the correct BBox*/
void printBrick(Brick * brick)
{
	float angle = bbAngle(brick->BBox);

	glEnable(GL_TEXTURE_2D);

	if(brick->strength == 3)
		glBindTexture(GL_TEXTURE_2D, brick->texture2);
	else if(brick->strength == 2)
		glBindTexture(GL_TEXTURE_2D, brick->texture1);
	else
		glBindTexture(GL_TEXTURE_2D, brick->texture);
	
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(brick->topLeft.x    , brick->topLeft.y);
		glTexCoord2f(1, 0); glVertex2f(brick->topRight.x   , brick->topRight.y);
		glTexCoord2f(1, 1); glVertex2f(brick->bottomRight.x, brick->bottomRight.y);
		glTexCoord2f(0, 1); glVertex2f(brick->bottomLeft.x , brick->bottomLeft.y);
	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


/** Free the brick element, the texture is NOT free-ed **/
void freeBrick(Brick * brick)
{

	free(brick);
	brick = NULL;
}


/** Generate a polygon for the given brick **/
Polygon * getBrickPolygon(Brick * brick)
{
	Polygon * poly = allocate(sizeof(Polygon));

	poly->BBox = brick->BBox;

	poly->points = allocate(sizeof(Vector2D) * 4);
	poly->nbrPoints = 4;

	/* EDGES ORDER : TOP RIGHT BOTTOM LEFT */

	poly->points[0] = brick->topLeft;
	poly->points[1] = brick->topRight;
	poly->points[2] = brick->bottomRight;
	poly->points[3] = brick->bottomLeft;
	
	return poly;
}


/** Handle a hit made to a brick*/
void brickHit(Brick * brick, int brickID)
{
	/*Ignore invincible bricks because they are... invincible*/
	if(brick->type == INVINCIBLE)
		return;

	brick->strength--;

	if(brick->strength == 0)
	{
		/*Remove the brick*/
		gameObj.toPrint[brickID]->display = false;


		if(rand() % 4 > 0)
			return;		/*No bonus this time*/

		/*Give a bonus*/
		generateBonus(brick);
	}
}
