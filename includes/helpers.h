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

/** Convert an int to a string **/
char * itoa(int val);

/** Free the given polygon **/
void freePolygon(Polygon * poly);

/** Free all players **/
void freePlayers();





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

/** Set theme to use **/
void setTheme(int themeID);



/****************/
/** COLLISIONS **/
/****************/

/** Detect a collision between a sphere and a rect **/
Collision circleRectCollision(Circle circle, Polygon * poly);




/***********/
/** MATHS **/
/***********/

/** Create a 2D vector **/
Vector2D vec2(float x, float y);

/** Create a 3D vector **/
Vector3D vec3(float x, float y, float z);

/** Add two vectors **/
Vector2D addVector(Vector2D A, Vector2D B);

/** Substract two vectors **/
Vector2D subVector(Vector2D A, Vector2D B);

/** Times vectors with factor **/
Vector2D multVector(Vector2D A, float factor);

/** Vector Norm **/
float norm(Vector2D A);

/** Vector Norm squared**/
float normSquared(Vector2D A);

/** Dot Product **/
float dotP(Vector2D A, Vector2D B);

/** Angle of the vector*/
float vectorAngleOrigin(Vector2D vec);

/** Rotate vector **/
Vector2D rotateVector(Vector2D vec, float angle);

/** Get point coordinate at the given angle and distance from the origin **/
void getCoordinatesAngle(float angle, float distance, Vector2D * coord);




/******************/
/** BOUNDING BOX **/
/******************/

/** Size of the BBox at the given level*/
float bbWidthAt(float dist);

/** Angle of the given BBox **/
float bbAngle(int BBox);

/** Tell if the given Ball is inside it's BBox **/
bool inBBox(Ball * ball);

/** Rotate the given polygon to the new BBox*/
void changePolyBBox(Polygon * poly, int BBox);

/** Rotate the given circle to the new BBox*/
void changeCircleBBox(Circle * circle, int BBox);

/** Tell if the BBox is reversed (on the bottom part of the screen) **/
bool bboxIsReversed(int BBox);





/**********/
/** TEXT **/
/**********/

/** Generate a font with it's ASCII characters **/
Font * loadFont(char * fontName, int size);

/** Generate the asked character for the given face **/
Character getCharacter(FT_Face face, int charID);

/** Free the given font **/
void freeFont(Font * font);





/************/
/** PARSER **/
/************/

/** Parse the given folder and return a FolderContent object with a list of containing files **/
FolderContent * parseFolder(char * folderPath);

/** Free a FolderContent **/
void closeFolder(FolderContent * folder);
