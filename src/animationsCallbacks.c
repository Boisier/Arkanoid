#include "../includes/game.h"

/***************************/
/** Start game animations **/
/***************************/

void startAnimationA()
{
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "2");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 1000, QUAD, &startAnimationB);
}

void startAnimationB()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
	{
	    createFloatAnimation(&gameObj.game.startAnimation.playersCtrl[i]->element.pict->opacity, 1.0, 0.0, 1000, 0, QUAD, NULL);
	}

	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "1");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 1000, QUAD, &startAnimationC);
}

void startAnimationC()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
	{
		gameObj.game.startAnimation.playersCtrl[i]->display = false;
	}
	
	free(gameObj.game.startAnimation.playersCtrl);
	
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "GO");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 1000, QUAD, &startAnimationD);
}

void startAnimationD()
{
    gameObj.game.startAnimation.countDown->display = false;
    gameObj.game.startAnimation.countDown->element.txt->opacity = 0.0;
	gameObj.game.play = true;
	gameObj.game.starting = false;
}


void endAnimationA()
{
	createFloatAnimation(&gameObj.toPrint[gameObj.nbrToPrint-1]->element.txt->opacity, 1.0, 1.0, 1, 1300, QUAD, &endAnimationB);
}

void endAnimationB()
{
	gameObj.gameState = PLAYERSELECTION;
}
