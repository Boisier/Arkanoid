#include "../../includes/game.h"

static void fillAnimation(Animation * anim, float startVal, float endVal, float duration, float delay);

Animation * createIntAnimation(int * target, float startVal, float endVal, float duration, float delay)
{
	Animation * anim = allocate(sizeof(Animation));

	anim->target.i = target;
	anim->isInt = true;

	fillAnimation(anim, startVal, endVal, duration, delay);

	return anim;	
}

Animation * createFloatAnimation(float * target, float startVal, float endVal, float duration, float delay)
{
	Animation * anim = allocate(sizeof(Animation));

	anim->target.f = target;
	anim->isInt = false;

	fillAnimation(anim, startVal, endVal, duration, delay);

	return anim;	
}

void fillAnimation(Animation * anim, float startVal, float endVal, float duration, float delay)
{
	anim->startVal = startVal;
	anim->endVal = endVal;
	anim->duration = duration;
	anim->startTime = SDL_GetTicks() + delay;

	anim->holder = addToPrint(anim, ANIMATION);	
}

void animate(Animation * anim)
{
	float newVal;

	if(SDL_GetTicks() < anim->startTime)
		return; /*We are before the animation time frame*/
		
	if(SDL_GetTicks() > anim->startTime + anim->duration)
	{	/*We are after the animation time frame*/
		anim->holder->display = false; /*Stop executing the animation every frame*/
		return; 
	}

	/*Linear easing*/
	newVal = anim->endVal * SDL_GetTicks() / anim->duration + anim->startVal;

	if(anim->isInt)
		* anim->target.i = newVal;
	else
		* anim->target.f = newVal;
}

void freeAnimation(Animation * anim)
{
	free(anim);
}