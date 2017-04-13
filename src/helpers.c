#include "../includes/game.h"

void throwCriticalError()                   /*Clean the program then terminate it*/
{
    printf("A critical error occured and the program had to stop.\n");  /*Throw error message*/
     exit(0);                               /*End the program*/
}

void criticalIfNull(void * pointer)         /*Throw a critical error and stop the application if the given pointer is null*/
{
    if(pointer == NULL)                     /*Is the given pointer NULL?*/
        throwCriticalError();               /*Yes, then stop all*/
}

void * allocate(int size)                   /*malloc encapsulation with failing behavior handling*/
{
    void * var = malloc(size);              /*Allocate asked memory*/
    criticalIfNull(var);                    /*Did the allocation worked ?*/
    
    return var;                             /*Yes, return newly allocated pointer*/
}

void * reAllocate(void * var, int newSize)  /*realloc encapsulation with failing behavior handling*/
{
    var = realloc(var, newSize);            /*realloc asked memory to the given pointer*/
    criticalIfNull(var);                    /*Did the allocation worked ?*/

    return var;                             /*Yes, return the pointer*/
}




GLuint getTexture(char * imagePath)
{
	//Make sure the texture isn't already loaded
	char path[356]; /* 256 for path, 100 for image should be enough */
	strcpy(path, gameObj.theme);
    strcat(path, imagePath);

	int rslt = textureLoaded(path);

	if(rslt != -1)
	{
		//Texture already loaded
		return gameObj.textures[rslt];
	}

	//This is a new texture

	//Load it
	SDL_Surface * image = IMG_Load(path);

    if(image == NULL)
	{
        printf("No image > %s\n", path);
		return -1;
	}

	//Add space to store it
	gameObj.nbrTextures++;

	if(gameObj.nbrTextures == 1)
	{
		gameObj.textures = allocate(sizeof(GLuint) * gameObj.nbrTextures);
		gameObj.texturesPath = allocate(gameObj.nbrTextures * sizeof(char *));
	}
	else
	{
		gameObj.textures = reAllocate(gameObj.textures, sizeof(GLuint) * gameObj.nbrTextures);
		gameObj.texturesPath = reAllocate(gameObj.texturesPath, gameObj.nbrTextures * sizeof(char *));
	}

	gameObj.texturesPath[gameObj.nbrTextures - 1] = allocate(sizeof(char) * 100);
	
	//Mark current texture as loaded
	strcpy(gameObj.texturesPath[gameObj.nbrTextures - 1], path);

	//Get the newly created space
	GLuint * texture = &gameObj.textures[gameObj.nbrTextures - 1];

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	//Set filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Get image format
	GLenum format = getImageFormat(image);

	//Send to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

	//Free the image
    SDL_FreeSurface(image);

	glBindTexture(GL_TEXTURE_2D, 0); // on debind la texture sur laquelle des operations ont été faites

	//And return the texture
	return * texture;
}


GLenum getImageFormat(SDL_Surface * image)
{
	switch(image->format->BytesPerPixel) 
	{
		case 1:
			return GL_RED;
		break;
		case 3:
			/* Ne gere pas les machines big-endian (a confirmer...) */
			return GL_RGB;
		break;
		case 4:
			/* Ne gere pas les machines big-endian (a confirmer...) */
			return GL_RGBA;
		break;
		default:
			/* On ne traite pas les autres cas */
			fprintf(stderr, "Format des pixels de l'image non pris en charge\n");
			return 0;
	}
}






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




void getTextureDimensions(GLuint texture, int * width, int * height)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);	

	glBindTexture(GL_TEXTURE_2D, 0);
}