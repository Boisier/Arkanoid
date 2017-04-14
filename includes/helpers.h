#pragma once

/** Basic helpers **/
void throwCriticalError();

void criticalIfNull(void * pointer);

void * allocate(int size);

void * reAllocate(void * var, int newSize);




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
