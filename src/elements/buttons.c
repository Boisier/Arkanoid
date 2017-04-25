#include "../../includes/game.h"

/** Create a new button element without any texture **/
Button * createButton(float posX, float posY, float width, float height, char callback)
{
	Button * btn = allocate(sizeof(Button));

	btn->x = posX;
	btn->y = posY;
	btn->width = width;
	btn->height = height;

	btn->state = IDLE;
	btn->callback = callback;

	btn->leftBtn = NULL;
	btn->topBtn = NULL;
	btn->rightBtn = NULL;
	btn->bottomBtn = NULL;

	return btn;
}

/** Print the given button to the screen */
void printButton(Button * btn)
{
	GLuint texture;

	if(btn->state == SELECTED)
		texture = btn->selectedTexture;
	else
		texture = btn->idleTexture;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(btn->x             , btn->y);
		glTexCoord2f(1, 0); glVertex2f(btn->x + btn->width, btn->y);
		glTexCoord2f(1, 1); glVertex2f(btn->x + btn->width, btn->y + btn->height);
		glTexCoord2f(0, 1); glVertex2f(btn->x             , btn->y + btn->height);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/** Free the button element, the textures are NOT free-ed **/
void freeButton(Button * btn)
{
	free(btn);
}