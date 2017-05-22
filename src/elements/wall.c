#include "../../includes/game.h"

/** Create a new wall element **/
Wall * createWall(int BBox)
{
	Wall * wall = allocate(sizeof(Wall));
	float topWidth, bottomWidth;

	topWidth = gameObj.game.bb.width;
	bottomWidth = gameObj.game.bb.width;

	topWidth *= .5;
	bottomWidth *= .5;

	wall->BBox = BBox;

	/*Define wall position*/
	wall->topLeft.x = -topWidth;
	wall->topLeft.y = gameObj.game.bb.height;

	wall->topRight.x = topWidth;
	wall->topRight.y = gameObj.game.bb.height;

	wall->bottomRight.x = bottomWidth;
	wall->bottomRight.y = gameObj.game.bb.height;

	wall->bottomLeft.x = -bottomWidth;
	wall->bottomLeft.y = gameObj.game.bb.height;

	/*Get wall texture*/
	wall->texture = getTexture("wall.png"); 

	return wall;
}


/** Print the wall on the screen on the correct BBox*/
void printWall(Wall * wall)
{
	float angle = bbAngle(wall->BBox);
	float topWidth;
	
	if(gameObj.game.bb.height - wall->topLeft.y < gameObj.defVal.wall.height)
	{
		wall->topLeft.y--;
		wall->topRight.y--;

		topWidth = bbWidthAt(gameObj.game.bb.height - wall->topLeft.y);
		wall->topLeft.x = -topWidth* .5;
		wall->topRight.x = topWidth* .5;
	}

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, wall->texture);
	
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(wall->topLeft.x    , wall->topLeft.y);
		glTexCoord2f(1, 0); glVertex2f(wall->topRight.x   , wall->topRight.y);
		glTexCoord2f(1, 1); glVertex2f(wall->bottomRight.x, wall->bottomRight.y);
		glTexCoord2f(0, 1); glVertex2f(wall->bottomLeft.x , wall->bottomLeft.y);
	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


/** Free the wall element, the texture is NOT free-ed **/
void freeWall(Wall * wall)
{
	free(wall);
	wall = NULL;
}


/** Generate a polygon for the given wall **/
Polygon * getWallPolygon(Wall * wall)
{
	Polygon * poly = allocate(sizeof(Polygon));

	poly->BBox = wall->BBox;

	poly->points = allocate(sizeof(Vector2D) * 4);
	poly->nbrPoints = 4;

	/* EDGES ORDER : TOP RIGHT BOTTOM LEFT */

	poly->points[0] = wall->topLeft;
	poly->points[1] = wall->topRight;
	poly->points[2] = wall->bottomRight;
	poly->points[3] = wall->bottomLeft;
	
	return poly;
}
