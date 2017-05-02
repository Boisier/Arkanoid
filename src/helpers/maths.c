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
float norm(Vector2D A)
{
    return sqrt(normSquared(A));
}

/** Vector Norm **/
float normSquared(Vector2D A)
{
    return A.x * A.x + A.y * A.y;
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

float vectorAngleOrigin(Vector2D vec)
{
    return atan2(vec.y, vec.x) * DEGTORAD;
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

/** Tell if the given Ball is inside it's BBox **/
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

/** Rotate the given polygon to the new BBox*/
void changePolyBBox(Polygon * poly, int BBox)
{
    float angle;
    int i;

    if(poly->BBox == BBox)
        return;

    angle = (poly->BBox - BBox) * -gameObj.game.bb.angle;
			
    for(i = 0; i < poly->nbrPoints; ++i)
	    poly->points[i] = rotateVector(poly->points[i], angle);

    poly->BBox = BBox;
}

/** Rotate the given circle to the new BBox*/
void changeCircleBBox(Circle * circle, int BBox)
{
    float angle;

    if(circle->BBox == BBox)
        return;

    angle = (circle->BBox - BBox) * -gameObj.game.bb.angle;
			
    circle->position = rotateVector(circle->position, angle);

    circle->BBox = BBox;
}
