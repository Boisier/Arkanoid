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
	btn->functionBtn = false;

	btn->isNumberBox = false;

	btn->opacity = 1.0;

	btn->leftBtn = NULL;
	btn->topBtn = NULL;
	btn->rightBtn = NULL;
	btn->bottomBtn = NULL;

	return btn;
}


/** Print the given button to the screen **/
void printButton(Button * btn)
{
	GLuint texture;

	if(btn->state == SELECTED)
		texture = btn->selectedTexture;
	else
		texture = btn->idleTexture;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glTranslatef(btn->x, btn->y, 0);

	glColor4f(1.0, 1.0, 1.0, btn->opacity);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(btn->width * -0.5, btn->height * 0.5);
		glTexCoord2f(1, 0); glVertex2f(btn->width * 0.5 , btn->height * 0.5);
		glTexCoord2f(1, 1); glVertex2f(btn->width * 0.5 , btn->height * -0.5);
		glTexCoord2f(0, 1); glVertex2f(btn->width * -0.5, btn->height * -0.5);
	glEnd();

	glPopMatrix();
	glPopAttrib();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


/** Free the button element, the textures are NOT free-ed **/
void freeButton(Button * btn)
{
	free(btn);
	btn = NULL;
}
