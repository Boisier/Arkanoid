#pragma once

/** First things first, let's get the libraries **/

/*SDL*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/*OpenGL*/
#include <GL/gl.h>
#include <GL/glu.h>

/*FreeType Lybrary*/
#include <ft2build.h>
#include FT_FREETYPE_H  

/*Standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <float.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

/** Now let's declare some global things... **/

/*Boolean type*/
typedef enum { false, true } bool;

/*some constants**/
static const unsigned int BIT_PER_PIXEL = 32;
static const float DEGTORAD = 57.2957795131; /* PI/180*/
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*Finally, include our own libraries*/
#include "enums.h"				/*All the enumerations*/
#include "structs.h"			/*All (but one) structs used in the app*/
#include "GameObj.h"			/*The main struct used to store game infos*/
#include "helpers.h"			/*All utilisty functions used for various things*/
#include "elementsFunctions.h"  /*The functions to use game elements (ball, buttons, etc.)*/
#include "screen.h"				/*Functions to display the elements on the screen*/
#include "animationsCallbacks.h"	/*All the smalls animations callbacks*/
#include "mainFunctions.h"		/*All the games main functions (menus, watchers, etc.)*/
