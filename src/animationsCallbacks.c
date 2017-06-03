#include "../includes/game.h"

/***************************/
/** Start game animations **/
/***************************/

void startAnimationTwo()
{
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "2");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationOne);
}

void startAnimationOne()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
	    createFloatAnimation(&gameObj.game.startAnimation.playersCtrl[i]->element.pict->opacity, 1.0, 0.0, 1000, 0, QUAD, NULL);
			
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "1");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationStart);
}

void startAnimationStart()
{
	int i;

	for(i = 0; i < gameObj.game.humans; ++i)
		gameObj.game.startAnimation.playersCtrl[i]->display = false;

	free(gameObj.game.startAnimation.playersCtrl);
	
	strcpy(gameObj.game.startAnimation.countDown->element.txt->text, "GO");
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 0.0, 1.0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.startAnimation.countDown->element.txt->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationEnd);
}

void startAnimationEnd()
{
    gameObj.game.startAnimation.countDown->display = false;
    gameObj.game.startAnimation.countDown->element.txt->opacity = 0.0;
	gameObj.game.play = true;
	gameObj.game.starting = false;
}
