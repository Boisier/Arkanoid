#pragma once

/*First things first, let's get the libraries*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/*Now let's declare some global things... */
typedef enum { false, true } bool;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*Finally, include our own libraries*/
#include "../includes/elements.h"
#include "../includes/GameObj.h"
#include "../includes/helpers.h"
#include "../includes/elementsFunctions.h"
#include "../includes/print.h"

#include "../includes/mainFunctions.h"