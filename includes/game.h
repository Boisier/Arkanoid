#pragma once

/*First things first, let's get the libraries*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*Now let's declare some global things... */
typedef enum { false, true } bool;

static const unsigned int BIT_PER_PIXEL = 32;
static const float DEGTORAD = 57.2957795131;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*Finally, include our own libraries*/
#include "enums.h"
#include "structs.h"
#include "GameObj.h"
#include "helpers.h"
#include "elementsFunctions.h"
#include "screen.h"
#include "mainFunctions.h"
