#pragma once

/****************/
/** PLATEFORME **/
/****************/

/** Create a new plateforme element width the defaultplateforme texture**/
Plateforme * createPlateforme(float posX, float posY);

/** Print the plateforme on the screen**/
void printPlateforme(Plateforme * plateforme);

/** Free the plateforme, the texture is not free-ed **/
void freePlateforme(Plateforme * plateforme);

/** Generate a polygon for the given plateforme **/
Polygon * getPlateformePolygon(Plateforme * plat);

/** Return the plateforme speed ratio to the max speed */
float platSpeedFactor(Plateforme * plat);

/** Update bonus for the given plateforme **/
void updatePlateformeBonus(Plateforme * plate);

/** Return the closest ball circle to the plateforme**/
Circle closestBall(Plateforme * plate);



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
Ball * createBall(float posX, float posY, int playerID, int BBox);

/** Create a new ball already glued to a plat*/
Ball * createGluedBall(float posX, float posY, int playerID);

/** Reset the bll position and state **/
void resetBall(Ball * ball);

/** Print the ball on the screen**/
void printBall(Ball * ball);

/** Free the ball, the texture is not free-ed **/
void freeBall(Ball * ball);

/** Build a circle for the given ball**/
Circle getBallCircle(Ball * ball);

/** Free the ball, the texture is not free-ed **/
void unglueBall(Ball * ball);

/** Move the ball along its direction. Bounce on screen sides*/
void moveBall(Ball * ball);

/** Update the ball BBox to it's new one **/
void updateBallBBox(Ball * ball);

/** Check if the ball is lost, return playerID who lost it*/
bool ballLost(Ball * ball, int * player);

/** CHeck for ball collisions **/
void ballCollisions(Ball * ball);

/**Handle ball collision with plateforme **/
void ballPlateformeCollision(Ball * ball, Plateforme * plat, Collision col);

/** Handle collision between a ball and a wall **/
void ballWallCollision(Ball * ball, Wall * wall, Collision col);

/**Handle collision between a ball and a brick**/
void ballBrickCollision(Ball * ball, Brick * brick, Polygon * brickPoly, Collision col, int brickID);

/** Set ball direction from a given angle **/
void setBallDirection(Ball * ball, float angle);

/** Update bonus for the given ball **/
void updateBallBonus(Ball * ball);





/***********/
/** BRICK **/
/***********/

/** Create a new brick element **/
Brick * createBrick(float Xpos, float level, int type, int BBox);

/** Print the brick on the screen on the correct BBox*/
void printBrick(Brick * brick);

/** Free the brick element, the texture(s) is(are) NOT free-ed **/
void freeBrick(Brick * brick);

/** Generate a Polygon for the given brick **/
Polygon * getBrickPolygon(Brick * brick);

/** Handle a hit made to a brick*/
void brickHit(Brick * brick, int brickID);





/**********/
/** WALL **/
/**********/

/** Create a new wall element **/
Wall * createWall(int BBox);

/** Print the wall on the screen on the correct BBox*/
void printWall(Wall * wall);

/** Free the wall element, the texture is NOT free-ed **/
void freeWall(Wall * wall);

/** Generate a polygon for the given wall **/
Polygon * getWallPolygon(Wall * wall);




/***********/
/** BONUS **/
/***********/

/** Create a new bonus element width the defaultplateforme texture**/
Bonus * createBonus(float posX, float posY, int BBox, enum bonusType type);

/** Print the bonus on the screen**/
void printBonus(Bonus * bonus);

/** Free the bonus, the texture is not free-ed **/
void freeBonus(Bonus * bonus);

/** Build a circle for the given bonus**/
Circle getBonusCircle(Bonus * bonus);

/** Generate a bonus at the given brick's position **/
void generateBonus(Brick * brick);

/** Check for ball collisions **/
bool bonusCollisions(Bonus * bonus);

/** Apply the effect of the given bonus **/
void applyBonus(Bonus * bonus);




/****************/
/** NUMBER BOX **/
/****************/

/*Create one button with the numberbox texture, add a text element on top, and link interactions between the two*/
NumberBox * createNumberBox(int x, int y, int width, int height, int value, int min, int max);

/*Free the number box, the button and the text are not free-ed*/
void freeNumberBox(NumberBox * nBox);

/*Increment by one the number box, and update it's text*/
void incrementNumberBox(NumberBox * nBox);

/*Decrement by one the number box, and update it's text*/
void decrementNumberBox(NumberBox * nBox);



/**********/
/** TEXT **/
/**********/

/** Create a new Text element **/
Text * createText(char * caption, float x, float y, Font * font);

/** Print the given Text at proper location **/
void printText(Text * txt);

/** Free the given Text element. The font is NOT free-ed **/
void freeText(Text * text);



/**********/
/** TEXT **/
/**********/

/** Create an animation for the specified target **/
void createIntAnimation(int * target, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)());
void createFloatAnimation(float * target, float startVal, float endVal, float duration, float delay, enum animationType easing, void (*callback)());

/** Execute the animation **/
void animate(Animation * anim);

/** Free the given animation **/
void freeAnimation(Animation * anim);
