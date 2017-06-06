#include "../../includes/game.h"

/** Create a new bonus element width the defaultplateforme texture**/
Bonus * createBonus(float posX, float posY, int BBox, enum bonusType type)
{
	Bonus * bonus = allocate(sizeof(Bonus));

	bonus->BBox = BBox;

	bonus->x = posX;
	bonus->y = posY;

	bonus->type = type;

	switch(bonus->type)
	{
		case WIDE_PLATEFORME:
		case STICKY_PLATEFORME:
		case BIG_BALL:
		case ADD_LIFE:
			bonus->texture = getTexture("bonus.png");
		break;
		case NARROW_PLATEFORME:
		case SMALL_BALL:
		case REMOVE_LIFE:
			bonus->texture = getTexture("malus.png");
		break;
		default: bonus->texture = 0; break;
	}

	return bonus;
}


/** Print the bonus on the screen**/
void printBonus(Bonus * bonus)
{	
	float x, y, size, angle;

	size = gameObj.defVal.bonus.size;

	x = bonus->x - size * .5;
	y = bonus->y - size * .5;

	angle = bbAngle(bonus->BBox);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bonus->texture);
	
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(   x    ,    y    );
		glTexCoord2f(1, 0); glVertex2f(x + size,    y    );
		glTexCoord2f(1, 1); glVertex2f(x + size, y + size);
		glTexCoord2f(0, 1); glVertex2f(   x    , y + size);
	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


/** Free the bonus, the texture is not free-ed **/
void freeBonus(Bonus * bonus)
{
	free(bonus);
	bonus = NULL;
}


/** Build a circle for the given bonus**/
Circle getBonusCircle(Bonus * bonus)
{
	Circle circ;

	circ.BBox = bonus->BBox;
	circ.position.x = bonus->x;
	circ.position.y = bonus->y;
	circ.radius = gameObj.defVal.bonus.size * .5;

	return circ;
}


/** Generate a bonus at the given brick's position **/
void generateBonus(Brick * brick)
{
	int random;
	Vector2D startPos;
	enum bonusType bType;

	/*Select a bonus*/
	random = rand() % 100;

	if(random < 20)
		bType = WIDE_PLATEFORME;
	else if(random < 40)
		bType = STICKY_PLATEFORME;
	else if(random < 60)
		bType = BIG_BALL;
	else if(random < 70)
		bType = ADD_LIFE;
	else if(random < 80)
		bType = REMOVE_LIFE;
	else if(random < 90)
		bType = NARROW_PLATEFORME;
	else
		bType = SMALL_BALL;

	/*Center of the brick*/
	startPos = addVector(brick->topLeft, multVector(subVector(brick->bottomRight, brick->topLeft), .5));

	/*Create and print the brick*/
	addToPrint(createBonus(startPos.x, startPos.y, brick->BBox, bType), BONUS);
}


/** Check for ball collisions **/
bool bonusCollisions(Bonus * bonus)
{
	Circle bonusCirc;
	Collision col;
	Polygon * poly;

	bonusCirc = getBonusCircle(bonus);

	/*Select the plateforme on the same BBox*/
	if(gameObj.game.players[bonus->BBox]->life == 0) 
		return false; /*This player has lost, not collision here*/

	poly = getPlateformePolygon(gameObj.game.players[bonus->BBox]->plateforme);

	/*Check collisions with the plateforme*/
	col = circleRectCollision(bonusCirc, poly);
			
	if(col.side == NO_COLLISION)
	{
		freePolygon(poly);
		return false; /*No collision, let's move on*/
	}

	/*There is a collision, apply bonus or malus in consequence*/
	applyBonus(bonus);

	freePolygon(poly);

	return true;
}


/** Apply the effect of the given bonus **/
void applyBonus(Bonus * bonus)
{
	int i;
	float offset;

	switch(bonus->type)
	{
		case ADD_LIFE:
			addLifePlayer(gameObj.game.players[bonus->BBox]);
		break;
		case REMOVE_LIFE:
			removeLifePlayer(gameObj.game.players[bonus->BBox]);
		break;
		case WIDE_PLATEFORME:
		case NARROW_PLATEFORME:

			gameObj.game.players[bonus->BBox]->plateforme->bonus = bonus->type;
			gameObj.game.players[bonus->BBox]->plateforme->bonusEnd = (int)SDL_GetTicks() + gameObj.defVal.bonus.duration;
			
			if(bonus->type == WIDE_PLATEFORME)
			{
				offset = (gameObj.defVal.plateforme.wideSize - gameObj.game.players[bonus->BBox]->plateforme->size) * .5;
				gameObj.game.players[bonus->BBox]->plateforme->size = gameObj.defVal.plateforme.wideSize;
			}
			else
			{
				offset = (gameObj.defVal.plateforme.narrowSize - gameObj.game.players[bonus->BBox]->plateforme->size) * .5;
				gameObj.game.players[bonus->BBox]->plateforme->size = gameObj.defVal.plateforme.narrowSize;
			}

			gameObj.game.players[bonus->BBox]->plateforme->x -= offset;
		break;
		case STICKY_PLATEFORME:

			gameObj.game.players[bonus->BBox]->plateforme->sticky = true;
			gameObj.game.players[bonus->BBox]->plateforme->stickyEnd = (int)SDL_GetTicks() + gameObj.defVal.bonus.duration;
		
		break;
		case BIG_BALL:
		case SMALL_BALL:

			for(i = 0; i < gameObj.nbrToPrint; ++i)
			{
				if(!gameObj.toPrint[i].display || gameObj.toPrint[i].type != BALL)
					continue;
				
				if(gameObj.toPrint[i].element.ball->playerID != bonus->BBox)
					continue;

				gameObj.toPrint[i].element.ball->bonus = bonus->type;
				gameObj.toPrint[i].element.ball->bonusEnd = (int)SDL_GetTicks() + gameObj.defVal.bonus.duration;

				if(bonus->type == BIG_BALL)
					gameObj.toPrint[i].element.ball->size = gameObj.defVal.ball.bigSize;
				else
					gameObj.toPrint[i].element.ball->size = gameObj.defVal.ball.smallSize;
			}

		break;
		default: break;
	}

	displayBonusText(bonus);
}

void displayBonusText(Bonus * bonus)
{
	Text * txt = gameObj.game.players[bonus->BBox]->bonusText;
	char caption[256];

	switch(bonus->type)
	{
		case ADD_LIFE:          strcpy(caption, "+1 vie"); 			    break;
		case REMOVE_LIFE:       strcpy(caption, "-1 vie"); 			    break;
		case WIDE_PLATEFORME:   strcpy(caption, "Grande plateforme");   break;
		case NARROW_PLATEFORME: strcpy(caption, "Petite plateforme");   break;
		case STICKY_PLATEFORME: strcpy(caption, "Plateforme collante"); break;
		case BIG_BALL: 			strcpy(caption, "Grosse balle");        break;
		case SMALL_BALL: 		strcpy(caption, "Petite balle");        break;
		default: break;
	}

	if(txt == NULL)
	{
		txt = createText(caption, 0, gameObj.game.bb.height + 10, gameObj.defaultFont);
		txt->BBox = bonus->BBox;

		addToPrint(txt, TEXT);
	}
	else
		strcpy(txt->text, caption);
	
	if(gameObj.game.players[bonus->BBox]->currentBonusAnim != NULL)
		gameObj.game.players[bonus->BBox]->currentBonusAnim->startVal = -1000; /*Terminate ongoing animation*/
		
	createFloatAnimation(&txt->opacity, 1.0, 0.0, 500, 1000, QUAD, NULL);
	gameObj.game.players[bonus->BBox]->currentBonusAnim = gameObj.toPrint[gameObj.nbrToPrint - 1].element.animation;
}
