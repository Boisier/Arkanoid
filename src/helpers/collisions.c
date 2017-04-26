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


typedef struct _circle
{
	float radius;
	Vector2D position;
} Circle;

typedef struct _polygon
{
	Vector2D * points;
	int nbrPoints;
} Polygon;


bool circleRectCollision(Circle circle, Polygon poly)
{
	Vector2D vertex, circleCenter, axis, origin, nextVertex, edge, projection;
	float radiusSquared, nearestDistance, distance, edgeLengthSquared, dot;
	int i, nearestVertex;

	origin.x = 0; origin.y = 0;
	
	radiusSquared = circle.radius * circle.radius;

	vertex = poly.points[poly.nbrPoints - 1];

	circleCenter = circle.position;

	nearestDistance = gameObj.wWidth;
    nearestVertex = -1;

	/* For each points of the polygon*/
	for(i = 0; i < poly.nbrPoints; i++)
    {
		axis = subVector(circleCenter, poly.points[i]);

		distance = normSquared(axis, origin) - radiusSquared;

		/*Collision ?*/
		if (distance <= 0)
		{
			/*Yes*/
			return true;
		}
		else if (distance < nearestDistance)
		{	
			/*No for this point*/
			nearestVertex = i;
			nearestDistance = distance;
		}
    }

	if(nearestVertex == 0)
		vertex = poly.points[poly.nbrPoints - 1];
	else
		vertex = poly.points[nearestVertex - 1];
	
	
	/*For vertex area before and after the closest point*/
    for(i = 0; i < 2; i++)
    {
      	nextVertex = poly.points[nearestVertex + i];
		
		edge = subVector(nextVertex, vertex);
		edgeLengthSquared = normSquared(edge, origin);

		/*norm = 0 ? same point, jump to the next*/
      	if(edgeLengthSquared == 0)
      	{
      		vertex = nextVertex;
			continue;
		}

		axis = subVector(circleCenter, vertex);
		dot = dotP(edge, axis);

		if(dot > 0 || dot > edgeLengthSquared)
      	{
      		vertex = nextVertex;
			continue;
		}

		projection = addVector(vertex, multVector(edge, (dot / edgeLengthSquared)));
		axis = subVector(projection, circleCenter);

		if(normSquared(axis, origin) <= radiusSquared)
			return true;
		
		if(edge.x > 0 && axis.y > 0)
			return false;

		if(edge.x < 0 && axis.y < 0)
			return false;
		
		if(edge.y > 0 && axis.x < 0)
			return false;
		
		if (axis.x > 0)
			return false;

      	vertex = nextVertex;
    }

    return true;
}
