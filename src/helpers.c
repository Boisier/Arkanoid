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

float percent(float percent, char direction)  /*return a percentage based on the window size*/
{
    if(direction == 'w')
        return (gameObj.wWidth / 100.0) * percent;
    else if(direction == 'h')
        return (gameObj.wHeight / 100.0) * percent;
    else
        return 0;
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



float fmax(float a, float b)
{
	return a > b ? a : b;
}

float fmin(float a, float b)
{
	return a < b ? a : b;
}

float clamp(float value, float min, float max)
{
	return fmax(min, fmin(value, max));
}



Collision SphereRectCollision(BaseSphere sphere, BaseRect rect, bool details)
{
	float deltaX, deltaY;
	Collision col;

	/*At start, assume no collision*/
	col.side = NO_COLLISION;

	/*Fast collision detection*/
	deltaX = sphere.x - clamp(sphere.x, rect.topLeftX, rect.topRightX);
	deltaY = sphere.y - clamp(sphere.y, rect.topLeftY, rect.bottomLeftY);

	if((deltaX * deltaX + deltaY * deltaY) >= (sphere.radius * sphere.radius))
		return col;				/*No collision*/
	
	/*There is a collision*/

	if(!details) 				/*Do we want more details ?*/
	{
		/*No need for details*/
		col.side = UNKNOWN;
		return col;
	}

	/*We need details*/
	return SphereRectCollisionDetails(sphere, rect);
}

Collision SphereRectCollisionDetails(BaseSphere sphere, BaseRect rect)
{
	Collision col;

	/*At start, assume no collision*/
	col.side = NO_COLLISION;

	/*Get collision side*/
	col.deltaTop = sphere.y - rect.topLeftY;
	col.deltaBottom = sphere.y - rect.bottomLeftY;
	col.deltaRight = sphere.x - rect.topRightX;
	col.deltaLeft = sphere.x - rect.topLeftX;

	/*printf("> TOP: %f > LEFT: %f > RIGHT: %f > BOTTOM : %f > RADIUS : %f\n", col.deltaTop, col.deltaLeft, col.deltaRight, col.deltaBottom, sphere.radius);*/

	if((col.deltaTop <= sphere.radius || col.deltaBottom <= sphere.radius) && col.deltaLeft >= -sphere.radius && col.deltaRight <= sphere.radius)
	{
		/*Collision on top or bottom*/
		col.x = sphere.x;

		if(col.deltaTop <= col.deltaBottom)
		{
			/*Sphere closer to top, assume coming from top*/
			col.y = sphere.y - col.deltaTop;
			col.side = TOP_SIDE;
		}
		else
		{
			/*Sphere closer to bottom or at equal distance*/
			/*Assume coming from bottom*/
			col.y = sphere.y + col.deltaBottom;
			col.side = BOTTOM_SIDE;
		}
	}
	else if((col.deltaLeft <= sphere.radius || col.deltaRight <= sphere.radius) && col.deltaLeft >= -sphere.radius && col.deltaRight <= sphere.radius)
	{
		/*Collision on left or right*/
		col.y = sphere.y;

		if(col.deltaLeft <= col.deltaRight)
		{
			/*Sphere closer to the left, assume coming from the left*/
			col.x = sphere.x - col.deltaLeft;
			col.side = LEFT_SIDE;
		}
		else
		{
			/*Sphere closer to the right or at equal distance*/
			/*Assume coming from thr right*/
			col.x = sphere.x + col.deltaRight;
			col.side = RIGHT_SIDE;
		}
	}
	else
	{
		col.side = UNKNOWN;
	}

	return col;
}
