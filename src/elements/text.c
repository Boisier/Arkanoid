#include "../../includes/game.h"

/** Create a new Text element **/
Text * createText(char * caption, float x, float y, Font * font)
{
	Text * txt = allocate(sizeof(Text));

	txt->BBox = -1;

	txt->x = x;
	txt->y = y;

	strcpy(txt->text, caption);

	txt->align = ALIGN_CENTER;
	txt->color = vec3(1.0, 1.0, 1.0);
	txt->opacity = 1.0;

	txt->font = font;

	return txt;
}


/** Print the given Text at proper location **/
void printText(Text * txt)
{
	int i, width = 0, startX = 0, length = strlen(txt->text), angle = 0;
	Character glyph;

	/*Start point based on alignement and length*/
	if(txt->align == ALIGN_CENTER || txt->align == ALIGN_RIGHT)
	{
		for(i = 0; i < length; ++i)
		{
			if(i+1 == length)
				width += txt->font->chars[(int)txt->text[i]].bearing.x + txt->font->chars[(int)txt->text[i]].size.x;
			else
			width += txt->font->chars[(int)txt->text[i]].advance / 64;
		}

		if(txt->align == ALIGN_CENTER)
			startX = - width * .5;
		else
			startX = - width;
	}

	/*Angle if rendered in a BBox*/
	if(txt->BBox != -1)
		angle = bbAngle(txt->BBox);

	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	glRotatef(angle, 0, 0, 1);
	glTranslatef(txt->x, txt->y, 0);

	if(txt->BBox != -1 && bboxIsReversed(txt->BBox))
	{
		glTranslatef(0, txt->font->size * 0.825, 0);
		glRotatef(180, 0, 0, 1);
	}

    glColor4f(txt->color.x, txt->color.y, txt->color.z, txt->opacity);

	for(i = 0; i < length; ++i)
	{
		glyph = txt->font->chars[(int)txt->text[i]];

		glBindTexture(GL_TEXTURE_2D, glyph.texture);

		glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(startX               , glyph.bearing.y);
            glTexCoord2f(1, 0); glVertex2f(startX + glyph.size.x, glyph.bearing.y);
            glTexCoord2f(1, 1); glVertex2f(startX + glyph.size.x, glyph.bearing.y - glyph.size.y);
            glTexCoord2f(0, 1); glVertex2f(startX               , glyph.bearing.y - glyph.size.y);
        glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		startX += glyph.advance / 64;
	}

	glPopAttrib();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


/** Free the given Text element. The font is NOT free-ed **/
void freeText(Text * text)
{
	free(text);
	text = NULL;
}
