#pragma once

/*******************/
/** BASIC HELPERS **/
/*******************/

/** Send an error and completely stop the program **/
void throwCriticalError();

/** Throw a critical error and stop the application if the given pointer is null **/
void criticalIfNull(void * pointer);

/** malloc encapsulation with failing behavior handling **/
void * allocate(int size);

/** realloc encapsulation with failing behavior handling **/
void * reAllocate(void * var, int newSize);

/** return a percentage based on the window size **/
float percent(float percent, char direction);

/** Return the higher value between a and b **/
float fmax(float a, float b);

/** Return the lower value between a and b **/
float fmin(float a, float b);

/** Clamp value between min and max **/
float clamp(float value, float min, float max);





/***********/
/** VIDEO **/
/***********/

void reshape(unsigned int width, unsigned int height);

void setVideoMode(unsigned int width, unsigned int height); 

bool createWindow();





/**************/
/** TEXTURES **/
/**************/

/** Load the asked texture from the theme folder. Handle already loaded textures **/
GLuint getTexture(char * image);

/** Return the image format to use with openGL **/
GLenum getImageFormat(SDL_Surface * image);

/** Add one more texture slot to store **/
GLuint * addTextureSlot(char * path);

/** Tell if the texture as already been loaded or not **/
int textureLoaded(char * needle);

/** Return the dimensions of the texture **/
void getTextureDimensions(GLuint texture, int * width, int * height);

/** Free all textures **/
void freeTextures();



/****************/
/** COLLISIONS **/
/****************/

/** Detect a collision between a sphere and a rect **/
Collision SphereRectCollision(BaseSphere sphere, BaseRect rect, bool details);

/** Detect a collision between a sphere and a rect and get details for it **/
Collision SphereRectCollisionDetails(BaseSphere sphere, BaseRect rect);
