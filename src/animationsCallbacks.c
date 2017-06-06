#include "../includes/game.h"

/***************************/
/** Start game animations **/
/***************************/

void startAnimationA()
{
	free(gameObj.game.startAnimation.countDown->element.txt->text);
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "2");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, &startAnimationB);
}

void startAnimationB()
{
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationC);
}

void startAnimationC()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
	{
	    createFloatAnimation(&gameObj.game.startAnimation.playersCtrl[i]->element.pict->opacity, 1.0, 0.0, 1000, 0, QUAD, NULL);
	}

	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "1");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, &startAnimationD);
}

void startAnimationD()
{
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationE);
}

void startAnimationE()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
	{
		gameObj.game.startAnimation.playersCtrl[i]->display = false;
	}
	
	free(gameObj.game.startAnimation.playersCtrl);
	
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "GO");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, &startAnimationF);
}

void startAnimationF()
{
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationG);
}

void startAnimationG()
{
    gameObj.game.startAnimation.countDown->display = false;
    gameObj.game.startAnimation.countDown->element.txt->opacity = 0.0;
	gameObj.game.play = true;
	gameObj.game.starting = false;
}




void endAnimationA()
{
	gameObj.gameState = PLAYERSELECTION;
}
