#pragma once

/****************/
/** PLATEFORME **/
/****************/

/** Create a new plateforme element width the defaultplateforme texture**/
Plateforme * createPlateforme(float posX, float posY, enum PlayerPos pos);

/** Print the plateforme on the screen**/
void printPlateforme(Plateforme * plateforme);

/** Free the plateforme, the texture is not free-ed **/
void freePlateforme(Plateforme * plateforme);

/** Generate a baseRect for the given plateforme* */
BaseRect getPlateformeBaseRect(Plateforme * plat);

/** Return the plateforme speed ratio to the max speed */
float platSpeedFactor(Plateforme * plat);



/*************/
/** PICTURE **/
/*************/

/** Create a new picture element with the given texture **/
Picture * createPicture(float posX, float posY, char * picturePath);

/** Print the given picture to the screen */
void printPicture(Picture * pict);

/** Free the picture element, the texture is NOT free-ed **/
void freePicture(Picture * pict);




/************/
/** BUTTON **/
/************/

/** Create a new button element without any texture **/
Button * createButton(float posX, float posY, float width, float height, char callback);

/** Print the given button to the screen **/
void printButton(Button * btn);

/** Free the button element, the textures are NOT free-ed **/
void freeButton(Button * btn);




/**********/
/** BALL **/
/**********/

/** Create a new ball element width the defaultplateforme texture**/
Ball * createBall(float posX, float posY, int playerID);

/** Print the ball on the screen**/
void printBall(Ball * ball);

/** Free the ball, the texture is not free-ed **/
void freeBall(Ball * ball);

/** Free the ball, the texture is not free-ed **/
void unglueBall(Ball * ball);

/** Move the ball along its direction. Bounce on screen sides*/
void moveBall(Ball * ball);

/** Check if the ball is lost, return playerID who lost it*/
bool ballLost(Ball * ball, int * player);

/** CHeck for ball collisions **/
void ballCollisions(Ball * ball);

/**Handle ball collision with plateforme **/
void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col);

/** Set ball direction from a given angle **/
void setBallDirection(Ball * ball, float angle);
