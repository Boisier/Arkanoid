#include "../includes/game.h"

/* Send an error and completely stop the program */
void throwCriticalError()                   
{
    printf("A critical error occured and the program had to stop.\n");
     exit(0);                               
}


/*Throw a critical error and stop the application if the given pointer is null*/
void criticalIfNull(void * pointer)         
{
    if(pointer == NULL)                     /*Is the given pointer NULL?*/
        throwCriticalError();               /*Yes, then stop all*/
}


/*malloc encapsulation with failing behavior handling*/
void * allocate(int size)                   
{
    void * var = malloc(size);              /*Allocate asked memory*/
    criticalIfNull(var);                    /*Did the allocation worked ?*/
    
    return var;                             /*Yes, return newly allocated pointer*/
}


/*realloc encapsulation with failing behavior handling*/
void * reAllocate(void * var, int newSize)  
{
    var = realloc(var, newSize);            /*realloc asked memory to the given pointer*/
    criticalIfNull(var);                    /*Did the allocation worked ?*/

    return var;                             /*Yes, return the pointer*/
}


/*Load the asked texture from the theme folder. Handle already loaded textures*/
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

/*Return the image format to use with openGL*/
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

/*Add one more texture slot to store*/
GLuint * addTextureSlot(char * path)
{
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

	strcpy(gameObj.texturesPath[gameObj.nbrTextures - 1], path);	/*Mark new texture as loaded*/

	return &gameObj.textures[gameObj.nbrTextures - 1];
}





/*Tell if the texture as already been loaded or not*/
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



/*Return the dimensions of the texture*/
void getTextureDimensions(GLuint texture, int * width, int * height)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);	

	glBindTexture(GL_TEXTURE_2D, 0);
}