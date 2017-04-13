#include "../../includes/game.h"

Picture * createPicture(float posX, float posY, char * picturePath)
{
	Picture * picture = allocate(sizeof(Plateforme));
	int height, width;

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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pict->texture);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(pict->x, pict->y);
		glTexCoord2f(1, 0); glVertex2f(pict->x + pict->width, pict->y);
		glTexCoord2f(1, 1); glVertex2f(pict->x + pict->width, pict->y - pict->height);
		glTexCoord2f(0, 1); glVertex2f(pict->x, pict->y - pict->height);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void freePicture(Picture * pict)
{
	free(pict);
}