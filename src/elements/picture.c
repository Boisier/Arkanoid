#include "../../includes/game.h"

/** Create a new picture element with the given texture **/
Picture * createPicture(float posX, float posY, char * picturePath)
{
	Picture * picture = allocate(sizeof(Button));
	int height, width;

	picture->BBox = -1;

	picture->x = posX;
	picture->y = posY;

	picture->texture = getTexture(picturePath);

	getTextureDimensions(picture->texture, &width, &height);

	picture->width = width;
	picture->height = height;

	return picture;
}

void printPicture(Picture * pict)
{
	float angle = 0;

	if(pict->BBox != -1)
		angle = bbAngle(pict->BBox);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pict->texture);

	glPushMatrix();
	
	glRotatef(angle, 0, 0, 1);
	glTranslatef(pict->x, pict->y, 0);

	if(pict->BBox != -1 && bboxIsReversed(pict->BBox))
		glRotatef(180, 0, 0, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(pict->width * -0.5, pict->height * 0.5);
		glTexCoord2f(1, 0); glVertex2f(pict->width * 0.5 , pict->height * 0.5);
		glTexCoord2f(1, 1); glVertex2f(pict->width * 0.5 , pict->height * -0.5);
		glTexCoord2f(0, 1); glVertex2f(pict->width * -0.5, pict->height * -0.5);
	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/** Free the picture element, the texture is NOT free-ed **/
void freePicture(Picture * pict)
{
	free(pict);
}
