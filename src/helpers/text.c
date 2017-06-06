#include "../../includes/game.h"

/** Generate a font with it's ASCII characters **/
Font * loadFont(char * fontName, int size)
{
	char path[256], testPath[256];
	int c;
	struct stat buffer;   
	Font * font = allocate(sizeof(Font));
	FT_Library ft;
	FT_Face face;

	/*Load FreeType*/
    if(FT_Init_FreeType(&ft))
    {
        printf("Could not load FreeType\n");
        throwCriticalError();
    }

	/*Store font name*/
	strcpy(font->name, fontName);
	font->size = size;

	/*Build path*/
	strcpy(path, gameObj.theme);
	strcat(path, "fonts/");
    strcat(path, fontName);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	strcpy(testPath, path);
	strcat(testPath, ".otf");

  	if(stat(testPath, &buffer) == 0)
		strcat(path, ".otf");
	else
		strcat(path, ".ttf");

	/*Load Font*/
	if(FT_New_Face(ft, path, 0, &face))
	{
		printf("Could not load font : \"%s\"\n", path);
		throwCriticalError();
	}

	/*Set font size for rendering*/
	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*Generate and store all ASCII chars*/
	for(c = 0; c < 128; ++c)
	{
		font->chars[c] = getCharacter(face, c);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	/*Free FreeType*/
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return font;
}


/** Generate the asked character for the given face **/
Character getCharacter(FT_Face face, int charID)
{
	GLuint texture;
	Character character;

	/*Load glyph*/ 
	FT_Load_Char(face, charID, FT_LOAD_RENDER);

	/*Generate texture with the glyph*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(
		GL_TEXTURE_2D,	
		0,
		GL_INTENSITY,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_LUMINANCE,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);

	/*Set texture options*/
	glBindTexture(GL_TEXTURE_2D, 0); /*Leave the texture*/

	character.texture = texture;

	character.size.x = face->glyph->bitmap.width;
	character.size.y = face->glyph->bitmap.rows;

	character.bearing.x = face->glyph->bitmap_left;
	character.bearing.y = face->glyph->bitmap_top;

	character.advance = face->glyph->advance.x;

	return character;
}


/** Free the given font **/
void freeFont(Font * font)
{
	int i;

	for(i = 0; i < font->size; ++i)
		glDeleteTextures(1, &font->chars[i].texture);

	free(font);
	font = NULL;
}
