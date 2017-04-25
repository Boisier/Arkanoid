#include "../../includes/game.h"

Collision SphereRectCollision(BaseSphere sphere, BaseRect rect, bool details)
{
	float deltaX, deltaY;
	Collision col;

	/*At start, assume no collision*/
	col.side = NO_COLLISION;

	/*Fast collision detection*/
	deltaX = sphere.x - clamp(sphere.x, rect.topLeftX, rect.topRightX);
	deltaY = sphere.y - clamp(sphere.y, rect.topLeftY, rect.bottomLeftY);

	if((deltaX * deltaX + deltaY * deltaY) >= (sphere.radius * sphere.radius))
		return col;				/*No collision*/
	
	/*There is a collision*/

	if(!details) 				/*Do we want more details ?*/
	{
		/*No need for details*/
		col.side = UNKNOWN;
		return col;
	}

	/*We need details*/
	return SphereRectCollisionDetails(sphere, rect);
}

/** Detect a collision between a sphere and a rect and get details for it **/
Collision SphereRectCollisionDetails(BaseSphere sphere, BaseRect rect)
{
	Collision col;

	/*At start, assume no collision*/
	col.side = NO_COLLISION;

	/*Get collision side*/
	col.deltaTop = sphere.y - rect.topLeftY;
	col.deltaBottom = sphere.y - rect.bottomLeftY;
	col.deltaRight = sphere.x - rect.topRightX;
	col.deltaLeft = sphere.x - rect.topLeftX;

	/*printf("> TOP: %f > LEFT: %f > RIGHT: %f > BOTTOM : %f > RADIUS : %f\n", col.deltaTop, col.deltaLeft, col.deltaRight, col.deltaBottom, sphere.radius);*/

	if((col.deltaTop <= sphere.radius || col.deltaBottom <= sphere.radius) && col.deltaLeft >= -sphere.radius && col.deltaRight <= sphere.radius)
	{
		/*Collision on top or bottom*/
		col.x = sphere.x;

		if(col.deltaTop <= col.deltaBottom)
		{
			/*Sphere closer to top, assume coming from top*/
			col.y = sphere.y - col.deltaTop;
			col.side = TOP_SIDE;
		}
		else
		{
			/*Sphere closer to bottom or at equal distance*/
			/*Assume coming from bottom*/
			col.y = sphere.y + col.deltaBottom;
			col.side = BOTTOM_SIDE;
		}
	}
	else if((col.deltaLeft <= sphere.radius || col.deltaRight <= sphere.radius) && col.deltaLeft >= -sphere.radius && col.deltaRight <= sphere.radius)
	{
		/*Collision on left or right*/
		col.y = sphere.y;

		if(col.deltaLeft <= col.deltaRight)
		{
			/*Sphere closer to the left, assume coming from the left*/
			col.x = sphere.x - col.deltaLeft;
			col.side = LEFT_SIDE;
		}
		else
		{
			/*Sphere closer to the right or at equal distance*/
			/*Assume coming from thr right*/
			col.x = sphere.x + col.deltaRight;
			col.side = RIGHT_SIDE;
		}
	}
	else
	{
		col.side = UNKNOWN;
	}

	return col;
}
