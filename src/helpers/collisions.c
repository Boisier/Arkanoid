#include "../../includes/game.h"

Collision circleRectCollision(Circle circle, Polygon * poly)
{
	Collision col;
	Vector2D vertex, nextVertex, edge, axis;
	float dot = 0, edgeLengthSquared = 0, radiusSquared, radiusSquaredCompensated, distance;
	int i, edgeID = -1;
	bool corner;

	col.side = NO_COLLISION;
	col.delta = 0;

	for(i = 0; i < poly->nbrPoints; ++i)
	{	
		vertex = poly->points[i];

		if(i + 1 == poly->nbrPoints)
			nextVertex = poly->points[poly->nbrPoints - 1];
		else
			nextVertex = poly->points[i+1];

		/**Check collision with the angle*/
		distance = normSquared(subVector(vertex, circle.position));
		radiusSquared = circle.radius * circle.radius;

		if(distance < radiusSquared)
		{
			edgeID = i;
			corner = true;
			break;
		}

		/*Check collision with the edge*/
		edge = subVector(nextVertex, vertex);
		axis = subVector(circle.position, vertex);

		dot = dotP(axis, edge);

		if(dot <= 0)
			continue;

		edgeLengthSquared = normSquared(edge);

		if(dot >= edgeLengthSquared)
			continue;
		
		radiusSquaredCompensated = normSquared(axis) - radiusSquared;
		distance = radiusSquaredCompensated * edgeLengthSquared;

		if(distance <= dot*dot)
		{
			/**Collision*/
			edgeID = i + 4;
			corner = false;
			break;
		}
	}

	if(edgeID == -1)
		return col;

	/*Enter collision informations*/
	if(corner)
	{
		col.point = vertex;
		col.delta = circle.radius - sqrt(distance);
	}
	else
	{
		col.point = addVector(vertex, multVector(edge, dot / edgeLengthSquared));
		col.delta = circle.radius - norm(subVector(col.point, circle.position));
	}

	/*Non quad polygon*/
	if(poly->nbrPoints > 4)
	{
		if(corner)
			col.side = CORNER;
		else
			col.side = UNKNOWN;
		
		return col;
	}

	/* EDGES ORDER : TOP RIGHT BOTTOM LEFT */
	switch(edgeID)
	{
		case 0: col.side = TOP_LEFT_CORNER;     break;
		case 1: col.side = TOP_RIGHT_CORNER;    break;
		case 2: col.side = BOTTOM_RIGHT_CORNER; break;
		case 3: col.side = BOTTOM_LEFT_CORNER;  break;
		case 4: col.side = TOP_SIDE;    		break;
		case 5: col.side = RIGHT_SIDE;  		break;
		case 6: col.side = BOTTOM_SIDE; 		break;
		case 7: col.side = LEFT_SIDE;   		break;
	}

	return col;
}
