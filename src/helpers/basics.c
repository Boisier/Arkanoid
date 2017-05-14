#include "../../includes/game.h"

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


/** return a percentage based on the window size **/
float percent(float percent, char direction)
{
    if(direction == 'w')
        return (gameObj.wWidth / 100.0) * percent;
    else if(direction == 'h')
        return (gameObj.wHeight / 100.0) * percent;
    else
        return 0;
}

/** Return the higher value between a and b **/
float fmax(float a, float b)
{
	return a > b ? a : b;
}

/** Return the lower value between a and b **/
float fmin(float a, float b)
{
	return a < b ? a : b;
}

/** Clamp value between min and max **/
float clamp(float value, float min, float max)
{
	return fmax(min, fmin(value, max));
}

/** Convert an int to a string **/
char * itoa(int val)
{
	char * caption = allocate(sizeof(char) * 10);

	sprintf(caption, "%d", val);
    return caption;
}

void freePolygon(Polygon * poly)
{
    free(poly->points);
    free(poly);
    poly = NULL;
}

/** Free all players **/
void freePlayers()
{
    int i;

    if(gameObj.game.nbrPlayers == 0)
        return; /*Nothing to free*/

    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
        free(gameObj.game.players[i]);

    free(gameObj.game.players);
    
    gameObj.game.players = NULL;
    gameObj.game.nbrPlayers = 0;
}

