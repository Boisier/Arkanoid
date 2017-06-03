#include "../../includes/game.h"

static void fillAnimation(Animation * anim, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)());

/** Create an animation for the specified target value **/
void createIntAnimation(int * target, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)())
{
	Animation * anim = allocate(sizeof(Animation));

	anim->target.i = target;
	anim->isInt = true;

	fillAnimation(anim, startVal, endVal, duration, delay, easing, callback);	
}


/** Create an animation for the specified target value **/
void createFloatAnimation(float * target, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)())
{
	Animation * anim = allocate(sizeof(Animation));

	anim->target.f = target;
	anim->isInt = false;

	fillAnimation(anim, startVal, endVal, duration, delay, easing, callback);
}


/** Fill informations in an animation for the specified target value **/
void fillAnimation(Animation * anim, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)())
{
	anim->easing = easing;

	anim->startVal = startVal;
	anim->endVal = endVal;
	anim->changeValue = endVal - startVal;
	anim->duration = duration;
	anim->startTime = SDL_GetTicks() + delay;

	anim->callback = callback;

	anim->holder = addToPrint(anim, ANIMATION);	
}


/** Execute the animation **/
void animate(Animation * anim)
{
	float newVal = 0, c, b, t;
	int d;

	if(SDL_GetTicks() < anim->startTime)
		return; /*We are before the animation time frame*/
		
	if(SDL_GetTicks() > anim->startTime + anim->duration)
	{	/*We are after the animation time frame*/
		anim->holder->display = false; /*Stop executing the animation every frame*/

		/*Update the value to its dest value**/
		if(anim->isInt)
			* anim->target.i = anim->endVal;
		else
			* anim->target.f = anim->endVal;

		/*Execute callback if needed*/
		if(anim->callback != NULL)
			anim->callback();

		return; 
	}

	t = SDL_GetTicks() - anim->startTime;
	b = anim->startVal;
	c = anim->changeValue;
	d = anim->duration;

	/*Use choosen easing function*/
	/*Sending kisses to http://gizma.com/easing/ <3 */
	switch(anim->easing)
	{
		case LINEAR:
			newVal = c * t / d + b;
		break;
		case QUAD:
			t /= d * .5;
			if(t < 1)
			{
				newVal = c * .5 * t * t + b;
				break;
			}

			t--;
			newVal = -c * .5 * (t * (t - 2) - 1) + b;
		break;
		case CUBIC:
			t /= d * .5;

			if (t < 1) 
			{
				newVal = c * .5 * t * t * t + b;
				break;
			}

			t -= 2;
			newVal = c * .5 * (t * t * t + 2) + b;
		break;
		case QUARTIC:
			t /= d * .5;

			if (t < 1) 
			{
				newVal = c * .5 * t * t * t * t + b;
				break;
			}

			t -= 2;
			newVal = -c * .5 * (t * t * t * t - 2) + b;
		break;
		case QUINTIC:
			t /= d * .5;

			if (t < 1) 
			{
				newVal = c * .5 * t * t * t * t * t + b;
				break;
			}

			t -= 2;
			newVal = c * .5 * ( t * t * t * t * t + 2) + b;
		break;
	}
	
	/*Update the value**/
	if(anim->isInt)
		* anim->target.i = newVal;
	else
		* anim->target.f = newVal;
}


/** Free the given animation **/
void freeAnimation(Animation * anim)
{
	free(anim);
}
