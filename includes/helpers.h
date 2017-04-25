#pragma once

/** Basic helpers **/
void throwCriticalError();

void criticalIfNull(void * pointer);

void * allocate(int size);

void * reAllocate(void * var, int newSize);

float percent(float percent, char direction);




/** Video Helpers **/
void reshape(unsigned int width, unsigned int height);

void setVideoMode(unsigned int width, unsigned int height); 

bool createWindow();




/** Texture loadingHandler **/
GLuint getTexture(char * image);

GLenum getImageFormat(SDL_Surface * image);

GLuint * addTextureSlot(char * path);

int textureLoaded(char * needle);



/** Other actions around textures **/
void getTextureDimensions(GLuint texture, int * width, int * height);


float fmax(float a, float b);
float fmin(float a, float b);
float clamp(float value, float min, float max);


/** Collisions **/
Collision SphereRectCollision(BaseSphere sphere, BaseRect rect, bool details);
Collision SphereRectCollisionDetails(BaseSphere sphere, BaseRect rect);


