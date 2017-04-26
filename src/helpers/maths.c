#include "../../includes/game.h"

/** Substract two vectors **/
Vector2D addVector(Vector2D A, Vector2D B)
{
    Vector2D vec;

    vec.x = A.x + B.x;
    vec.y = A.y + B.y;

    return vec;
}

Vector2D subVector(Vector2D A, Vector2D B)
{
    Vector2D vec;

    vec.x = A.x - B.x;
    vec.y = A.y - B.y;

    return vec;
}


/** Substract two vectors **/
Vector2D multVector(Vector2D A, float factor)
{
    Vector2D vec;

    vec.x = A.x * factor;
    vec.y = A.y * factor;

    return vec;
}

/** Vector Norm **/
float norm(Vector2D A, Vector2D B)
{
    return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

/** Vector Norm **/
float normSquared(Vector2D A, Vector2D B)
{
    float n = norm(A, B);
    return n * n;
}

/** Dot Product **/
float dotP(Vector2D A, Vector2D B)
{
    return A.x * B.x + A.y * B.y;
}

/** Rotate vector **/
Vector2D rotateVector(Vector2D vec, float angle)
{
    Vector2D rotated;
    rotated.x = vec.x * cos(angle / DEGTORAD) - vec.y * sin(angle / DEGTORAD);
    rotated.y = vec.x * sin(angle / DEGTORAD) + vec.y * cos(angle / DEGTORAD);

    return rotated;
}

/** Get point coordinate at the given angle and distance from the origin **/
void getCoordinatesAngle(float angle, float distance, Vector2D * vec)
{
    vec->x = sin(angle / DEGTORAD) * distance;
    vec->y = cos(angle / DEGTORAD) * distance;
}





/****************************/
/** BBOX RELATED FUNCTIONS **/
/****************************/

float bbWidthAt(float dist)
{
    if(gameObj.game.bb.squared)
        return gameObj.game.bb.width;
	    
    return ((gameObj.game.bb.height - dist) / gameObj.game.bb.height) * gameObj.game.bb.width;
}

float bbAngle(int BBox)
{
	return gameObj.game.bb.startAngle + gameObj.game.bb.angle * BBox;
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