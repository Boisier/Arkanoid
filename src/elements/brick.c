#include "../../includes/game.h"

/** Create a new brick element **/
Brick * createBrick(float Xpos, float level, int type, int BBox)
{
	Brick * brick = allocate(sizeof(Button));
	float topWidth, bottomWidth, topBrickWidth, bottomBrickWidth;

	topWidth = bbWidthAt(gameObj.game.bb.height - level);
	topBrickWidth = topWidth / gameObj.game.bb.gridW;
	bottomWidth = bbWidthAt(gameObj.game.bb.height - level - gameObj.defVal.brick.height);
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
	brick->bottomLeft.y = level + gameObj.defVal.brick.height;

	brick->bottomRight.x = -bottomWidth + bottomBrickWidth * (Xpos + 1);
	brick->bottomRight.y = level + gameObj.defVal.brick.height;

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
			brick->texture1 = getTexture("brick/veryStrong_2.png"); 
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
}

/** Generate a baseRect for the given brick **/
BaseRect getBrickBaseRect(Brick * brick)
{
	BaseRect brickBase;

	brickBase.topLeftX = brick->topLeft.x;
	brickBase.topLeftY = brick->topLeft.y;

	brickBase.topRightX = brick->topRight.x;
	brickBase.topRightY = brick->topRight.y;

	brickBase.bottomRightX = brick->bottomRight.x;
	brickBase.bottomRightY = brick->bottomRight.y;
	
	brickBase.bottomLeftX = brick->bottomLeft.x;
	brickBase.bottomLeftY = brick->bottomLeft.y;
	
	return brickBase;
}
