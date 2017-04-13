#pragma once


void throwCriticalError();

void criticalIfNull(void * pointer);

void * allocate(int size);

void * reAllocate(void * var, int newSize);




/* Video Helpers */
void reshape(unsigned int width, unsigned int height);

void setVideoMode(unsigned int width, unsigned int height); 

bool createWindow();




/* Texture loadingHandler */
GLuint getTexture(char * path);

GLenum getImageFormat(SDL_Surface * image);

int textureLoaded(char * needle);



#define printOpenGLError() printOglError(__FILE__, __LINE__)