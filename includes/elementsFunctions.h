#pragma once

Plateforme * createPlatforme(float posX, float posY);
void printPlateforme(Plateforme * plateforme);
void freePlateforme(Plateforme * plateforme);

Picture * createPicture(float posX, float posY, char * picturePath);
void printPicture(Picture * pict);
void freePicture(Picture * pict);

Button * createButton(float posX, float posY, float width, float height, char callback);
void printButton(Button * btn);
void freeButton(Button * btn);