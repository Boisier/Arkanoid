#include "../../includes/game.h"

/** Load the asked texture from the theme folder. Handle already loaded textures **/
GLuint getTexture(char * imagePath)
{
	char path[356]; /* 256 for path, 100 for image should be enough */
	int rslt;
	SDL_Surface * image;
	GLuint * texture;
	GLenum format;

	/* Build path */
	strcpy(path, gameObj.theme);
    strcat(path, imagePath);
	
	rslt = textureLoaded(path);				/*Make sure the texture isn't already loaded*/

	if(rslt != -1)							/*Is the texture already loaded ?*/
	{
		return gameObj.textures[rslt];		/*Return the already loaded texture*/
	}

	/*This is a new texture*/

	image = IMG_Load(path);	/*Load the image*/

    if(image == NULL)						/*Loading ok ?*/
	{
        printf("No image > %s\n", path);
		return -1;
	}

	/*Increase space in texture holder to store the new one*/
	texture = addTextureSlot(path); /*And get the newly created space*/

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	/*Set filters*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	format = getImageFormat(image);	/*Get image format*/

	/*Send to OpenGL with this LOVELY function*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image); /*Free the image*/

	glBindTexture(GL_TEXTURE_2D, 0); /*Leave the texture*/

	return * texture; /*Return the texture ID*/
}

/** Return the image format to use with openGL **/
GLenum getImageFormat(SDL_Surface * image)
{
	switch(image->format->BytesPerPixel) 
	{
		case 1:
			return GL_RED;
		break;
		case 3:
			return GL_RGB;
		break;
		case 4:
			return GL_RGBA;
		break;
		default:
			fprintf(stderr, "Format des pixels de l'image non pris en charge\n");
			return 0;
	}
}

/** Add one more texture slot to store **/
GLuint * addTextureSlot(char * path)
{
	gameObj.nbrTextures++;

	if(gameObj.nbrTextures == 1)
	{
		gameObj.textures = allocate(sizeof(GLuint));
		gameObj.texturesPath = allocate(sizeof(char *));
	}
	else
	{
		gameObj.textures = reAllocate(gameObj.textures, sizeof(GLuint) * gameObj.nbrTextures);
		gameObj.texturesPath = reAllocate(gameObj.texturesPath, gameObj.nbrTextures * sizeof(char *));
	}

	gameObj.texturesPath[gameObj.nbrTextures - 1] = allocate(sizeof(char) * 256);

	strcpy(gameObj.texturesPath[gameObj.nbrTextures - 1], path);	/*Mark new texture as loaded*/

	return &gameObj.textures[gameObj.nbrTextures - 1];
}


/** Tell if the texture as already been loaded or not **/
int textureLoaded(char * needle)
{
	int i;

	for(i = 0; i < gameObj.nbrTextures; ++i)
	{
		if(strcmp(gameObj.texturesPath[i], needle) == 0)
		{
			return i;
		}
	}

	return -1;
}


/** Return the dimensions of the texture **/
void getTextureDimensions(GLuint texture, int * width, int * height)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);	

	glBindTexture(GL_TEXTURE_2D, 0);
}

/** Free all textures **/
void freeTextures()
{
    int i;

    /*Free all textures*/
    glDeleteTextures(gameObj.nbrTextures, gameObj.textures);

    for(i = 0; i < gameObj.nbrTextures; ++i)
    {
        free(gameObj.texturesPath[i]);
    }

    free(gameObj.texturesPath);
    
    gameObj.texturesPath = NULL;
    gameObj.nbrTextures = 0;
}

void setTheme(int themeID)
{
	DIR * themesFolder;
    struct dirent * tFold;
	char themePath[256];
	int i  = 0;

    themesFolder = opendir("./themes");

	/*Parse folder*/
	do
    {
        tFold = readdir(themesFolder);

        if(tFold != NULL)
        {
            /*Ignore dot and dotdot folders*/
            if(tFold->d_name[0] == '.')
                continue;
			
			++i;

			/*Is this the theme ?*/
			if(i != themeID)
				continue;

			/*This is the theme*/
			strcpy(themePath, "./themes/");
            strcat(themePath, tFold->d_name);
            strcat(themePath, "/");
			break;
        }
    } while(tFold != NULL);

    (void) closedir(themesFolder);

	strcpy(gameObj.theme, themePath);

	/*Clean up textures*/
	freeTextures();
}
