#pragma once

Plateforme * createPlatforme(float posX, float posY);
void printPlateforme(Plateforme * plateforme);
void freePlateforme(Plateforme * plateforme);

Picture * createPicture(float posX, float posY, char * picturePath);
void printPicture(Picture * pict);
void freePicture(Picture * pict);