#include "../../includes/game.h"


void getCoordinatesAngle(float angle, float distance, Vector2D * coord)
{
    coord->x = sin(angle / DEGTORAD) * distance;
    coord->y = cos(angle / DEGTORAD) * distance;
}

float norm(Vector2D A, Vector2D B)
{
    return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

float bbWidthAt(float dist)
{
	return ((gameObj.game.bb.height - dist) / gameObj.game.bb.height) * gameObj.game.bb.width;
}

float bbAngle(int BBox)
{
	return gameObj.game.bb.startAngle + gameObj.game.bb.angle * BBox;
}

Vector2D rotateVector(Vector2D vec, float angle)
{
    Vector2D rotated;
    rotated.x = vec.x * cos(angle / DEGTORAD) - vec.y * sin(angle / DEGTORAD);
    rotated.y = vec.x * sin(angle / DEGTORAD) + vec.y * cos(angle / DEGTORAD);

    return rotated;
}

bool inBBox(Ball * ball)
{
    float u, v, denom, dotAA, dotAC, dotAB, dotCC, dotCB;
    Vector2D A, C, B;

    if(gameObj.game.bb.squared)
    {
        /*Simple square technique*/
        return ball->y >= 0 && ball->y <= gameObj.game.bb.height;
    }
    /*Barycentric Technique*/

    A.x = gameObj.game.bb.width / 2;
    A.y = gameObj.game.bb.height;

    C.x = - gameObj.game.bb.width/2;
    C.y = gameObj.game.bb.height;

    B.x = ball->x;
    B.y = ball->y;

    dotAA = dotP(A, A);
    dotAC = dotP(A, C);
    dotAB = dotP(A, B);
    dotCC = dotP(C, C);
    dotCB = dotP(C, B);

    denom = 1 / (dotAA * dotCC - dotAC * dotAC);
    
    u = (dotCC * dotAB - dotAC * dotCB) * denom;
    v = (dotAA * dotCB - dotAC * dotAB) * denom;

    return u >= 0 && v >= 0 && u + v < 1;
}

float dotP(Vector2D A, Vector2D B)
{
    return A.x * B.x + A.y * B.y;
}
