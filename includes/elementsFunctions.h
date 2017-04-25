#pragma once

Plateforme * createPlateforme(float posX, float posY, enum PlayerPos pos);
void printPlateforme(Plateforme * plateforme);
void freePlateforme(Plateforme * plateforme);
BaseRect getPlateformeBaseRect(Plateforme * plat);

Picture * createPicture(float posX, float posY, char * picturePath);
void printPicture(Picture * pict);
void freePicture(Picture * pict);

Button * createButton(float posX, float posY, float width, float height, char callback);
void printButton(Button * btn);
void freeButton(Button * btn);

Ball * createBall(float posX, float posY, int playerID);
void printBall(Ball * ball);
void freeBall(Ball * ball);
void unglueBall(Ball * ball);
void moveBall(Ball * ball);
bool ballLost(Ball * ball, int * player);
void ballCollisions(Ball * ball);
void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col);