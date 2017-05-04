#include "../includes/game.h"

/** Watch for user events **/
void watcher()
{
    SDL_Event e;
    bool newKeyValue = false;

    while(SDL_PollEvent(&e)) 
    {
        if(e.type == SDL_QUIT) 
        {
            gameObj.gameState = EXITING;
            break;
        }

        switch(e.type) 
        {
            case SDL_KEYDOWN:
                newKeyValue = true;
            break;
            case SDL_KEYUP:
                newKeyValue = false;
            break;
        }

        if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_UP      : gameObj.keys.up    = newKeyValue; break;
                case SDLK_DOWN    : gameObj.keys.down  = newKeyValue; break;
                case SDLK_LEFT    : gameObj.keys.left  = newKeyValue; break;
                case SDLK_RIGHT   : gameObj.keys.right = newKeyValue; break;
                case SDLK_RETURN  :
                case SDLK_KP_ENTER:   /*Let's not forget the keypad*/
                                    gameObj.keys.enter = newKeyValue; break;
                case SDLK_ESCAPE  : gameObj.keys.esc   = newKeyValue; break;
                case SDLK_a       : gameObj.keys.a     = newKeyValue; break;
                case SDLK_z       : gameObj.keys.z     = newKeyValue; break;
                case SDLK_e       : gameObj.keys.e     = newKeyValue; break;
                case SDLK_b       : gameObj.keys.b     = newKeyValue; break;
                case SDLK_n       : gameObj.keys.n     = newKeyValue; break;
                case SDLK_v       : gameObj.keys.v     = newKeyValue; break;
                default: /*Do nothing for other keys*/ break;
            }
        }
    }
}

/** Handle menu interactions **/
char btnHandler()
{
	Button * btn;

	if(!gameObj.keys.up && !gameObj.keys.down && !gameObj.keys.left && !gameObj.keys.right && !gameObj.keys.enter)
    {
        return 0;                             /*None of the keys we need are pressed, no need to go further*/
    }

    btn = gameObj.currentlySelectedBtn;

    if(gameObj.keys.enter)
    {
        /*Deactivate the key*/
        gameObj.keys.enter = false;

        /*Handle action*/
        if(btn->functionBtn)
        {
            (*btn->functionCallback)(btn->callbackArgument);
        }
        else
        {
		    return btn->callback;
        }
    }

    if(gameObj.keys.up && btn->topBtn != NULL)
    {
        btn->state = IDLE;
        btn->topBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->topBtn;

        if(btn->isNumberBox)
            btn->text->color = gameObj.defautlTextColor;

        if(btn->topBtn->isNumberBox)
            btn->topBtn->text->color = gameObj.selectedTextColor;

        /*Deactivate the key*/
        gameObj.keys.up = false;

		return 0;
    }
    
	if(gameObj.keys.down && btn->bottomBtn != NULL)
    {
        btn->state = IDLE;
        btn->bottomBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->bottomBtn;

        if(btn->isNumberBox)
            btn->text->color = gameObj.defautlTextColor;

        if(btn->bottomBtn->isNumberBox)
            btn->bottomBtn->text->color = gameObj.selectedTextColor;
        
        /*Deactivate the key*/
        gameObj.keys.down = false;

		return 0;
    }

    if(gameObj.keys.right && btn->isNumberBox)
    {
        /*Increment numberBox*/
        incrementNumberBox(btn->callbackArgument);

        /*Deactivate the key*/
        gameObj.keys.right = false;

        return 0;
    }

    if(gameObj.keys.left && btn->isNumberBox)
    {
        /*Increment numberBox*/
        decrementNumberBox(btn->callbackArgument);

        /*Deactivate the key*/
        gameObj.keys.left = false;

        return 0;
    }

	return 0;
}

/**Define the playable area for each player**/
void defineBoundingBox()
{
    Vector2D A, B, middle;

    /*Size of a bounding box*/
    gameObj.game.bb.radius = gameObj.wHeight / 2;

    /*The display style is different if there is only 2 players*/
    if(gameObj.game.nbrPlayers == 2)
    {
        /*For 2 player, the bb is handled manually*/
        gameObj.game.bb.squared = true;
        gameObj.game.bb.angle = 180;
        gameObj.game.bb.startAngle = 180;
        gameObj.game.bb.width = gameObj.wWidth;
        gameObj.game.bb.height = gameObj.game.bb.radius;
        gameObj.game.bb.platMinPos = - gameObj.wWidth / 2;
        gameObj.game.bb.platMaxPos = gameObj.wWidth / 2;
        return;
    }

    gameObj.game.bb.squared = false;

    /*Get angles*/
    gameObj.game.bb.angle = 360.0 / gameObj.game.nbrPlayers;
    gameObj.game.bb.startAngle = 180;

    /*Get two extremities*/
    getCoordinatesAngle(0, gameObj.game.bb.radius, &A);
    getCoordinatesAngle(gameObj.game.bb.angle, gameObj.game.bb.radius, &B);

    /*Width of the base of the bounding box*/
    gameObj.game.bb.width = norm(subVector(B, A));

    /*Get the height of the bounding box*/
    middle.x = (A.x + B.x) / 2;
    middle.y = (A.y + B.y) / 2;

    gameObj.game.bb.height = sqrt((middle.x * middle.x) + (middle.y * middle.y));

    /**Store more data that will be used frequently**/
    gameObj.game.bb.platMinPos = - bbWidthAt(gameObj.defVal.plateforme.level) / 2;
    gameObj.game.bb.platMaxPos = bbWidthAt(gameObj.defVal.plateforme.level) / 2;
    
    /*printf(">> %f <> %f \n", bbWidthAt(gameObj.defVal.plateforme.level), gameObj.game.bb.width);*/
}







/** Handle movement of the players **/
void playerMovements()
{
    int i;
    float speedFactor, speedBonus, plateCenterX;
    Player * player;
    Plateforme * plateforme;
    Circle closest;
    bool leftKey, rightKey;


    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        player = gameObj.game.players[i];

        if(player->plateforme == NULL)
            continue; /*This player is out*/

        /*Check and update bonus on this player's plateforme*/
        updatePlateformeBonus(player->plateforme);

        if(player->type == AI)
        {
            /*AI*/
            closest = closestBall(player->plateforme);

            plateCenterX = player->plateforme->x + player->plateforme->size / 2;

            if(closest.position.x > plateCenterX + 25)
            {
                leftKey = false;
                rightKey = true;
            }
            else if(closest.position.x < plateCenterX - 25)
            {
                leftKey = true;
                rightKey = false;
            }
            else
            {
                leftKey = false;
                rightKey = false;
            }
        }
        else
        {
            /*Human Player*/

            /*Keys are inversed if the player in on the top half on the screen*/
            if(player->controls == 0)
            {
                if(player->reversed)
                {
                    leftKey = gameObj.keys.right;
                    rightKey = gameObj.keys.left;
                }
                else
                {
                    leftKey = gameObj.keys.left;
                    rightKey = gameObj.keys.right;
                }
            }
            else if(player->controls == 1)
            {
                if(player->reversed)
                {
                    leftKey = gameObj.keys.e;
                    rightKey = gameObj.keys.a;
                }
                else
                {
                    leftKey = gameObj.keys.a;
                    rightKey = gameObj.keys.e;
                }
            }
            else /*if(player->controls == 2)*/
            {
                if(player->reversed)
                {
                    leftKey = gameObj.keys.n;
                    rightKey = gameObj.keys.v;
                }
                else
                {
                    leftKey = gameObj.keys.v;
                    rightKey = gameObj.keys.n;
                }
            }
        }

        plateforme = player->plateforme;

        if(!leftKey && !rightKey)
        {
            /*No movement here*/
            plateforme->speed = 0;
            continue;
        }
        
        /*This player plateforme is moving*/
        
        /*Update plateforme speed*/
        speedFactor = 1 - (plateforme->speed / gameObj.defVal.plateforme.maxSpeed);
        speedBonus = speedFactor * gameObj.defVal.plateforme.acceleration;

        plateforme->speed += gameObj.defVal.plateforme.acceleration + speedBonus;

        if(plateforme->speed > gameObj.defVal.plateforme.maxSpeed && player->type == HUMAN)
            plateforme->speed = gameObj.defVal.plateforme.maxSpeed;
        
        if(leftKey)
        {
            /*Move top plateforme to the left*/
            plateforme->x -= plateforme->speed; 

            plateforme->dirFactor = -1;

            /*Are we still in the game area ?*/
            if(plateforme->x < gameObj.game.bb.platMinPos)
            {
                plateforme->x = gameObj.game.bb.platMinPos;
                plateforme->speed = 0;
            }

        }
        else if(rightKey)
        {
            /*Move top plateforme to the right*/
            plateforme->x += plateforme->speed; 

            plateforme->dirFactor = 1;

            /*Are we still in the game area ?*/
            if(plateforme->x + plateforme->size > gameObj.game.bb.platMaxPos)
            {
                plateforme->x = gameObj.game.bb.platMaxPos - plateforme->size;
                plateforme->speed = 0;
            }
        }
    }
}

/** Handle movement of the balls **/
void ballMovements()
{
    int i, player;
    Ball * ball;

    for(i = 0; i < gameObj.nbrToPrint; ++i)
    {
        /*Only care about active balls here*/
        if(gameObj.toPrint[i].type != BALL || !gameObj.toPrint[i].display)
            continue;

        ball = gameObj.toPrint[i].element.ball;

        /*Update bonus*/
        updateBallBonus(ball);

        /*See if ball is glued and try to unglue it*/
        unglueBall(ball);

        if(ball->glued)
        {
            /*Ball glued, no movements here*/
            continue;
        }

        /*Ball not glued, let's move*/
        moveBall(ball);

        /*Lost ball ?*/
        if(ballLost(ball, &player))
        {
            /*Remove a life to the player who lost it*/
            gameObj.game.players[player]->life--;
            strcpy(gameObj.game.players[player]->lifeText->text, itoa(gameObj.game.players[player]->life));

            /*Reset the ball to it's starting position*/
            resetBall(ball);

            continue;
        }

        /*Check collisions*/
        ballCollisions(ball);
    }
}

/**Create the bricks for the level*/
void createBricks(char * levelName)
{
    FILE * level;
    char path[256] = "./levels/", line[256];
    int i, j, k, type;

    strcat(path, levelName);

    level = fopen(path, "r");

    /*Retrieve grid dimensions*/
    if(fgets(line, 255, level) == NULL)
        throwCriticalError();

    gameObj.game.bb.gridW = atoi(strtok(line, " "));
    gameObj.game.bb.gridH = atoi(strtok(NULL, " "));

    /*For every line of brick*/
    for(i = 0; i < gameObj.game.bb.gridH; ++i)
    {
        /*Get the line infos*/
        if(fgets(line, 255, level) == NULL)
            throwCriticalError();
        
        /*printf("%s\n", line);*/
        
        /*For every brick on the line*/
        for(j = 0; j < gameObj.game.bb.gridW; ++j)
        {
            /*Get the brick type*/
            if(j == 0)
                type = atoi(strtok(line, " "));
            else
                type = atoi(strtok(NULL, " "));
            
            if(type == 0) /*No brick on this one*/
                continue;

            /*For every player*/
            for(k = 0; k < gameObj.game.nbrPlayers; ++k)
            {   
                /*Add the brick*/
                addToPrint(createBrick(gameObj.game.bb.gridW - j-1, gameObj.defVal.brick.startLevel + i * gameObj.defVal.brick.height, type, k), BRICK);
            }
        }
    }

    fclose(level);
}


/** Handle movement of the bonus **/
void bonusMovements()
{
    int i;
    Bonus * bonus;

    for(i = 0; i < gameObj.nbrToPrint; ++i)
    {
        /*Only care about active bonus here*/
        if(gameObj.toPrint[i].type != BONUS || !gameObj.toPrint[i].display)
            continue;

        bonus = gameObj.toPrint[i].element.bonus;

        /*Move bonus*/
        bonus->y += gameObj.defVal.bonus.speed;

        /*Lost ball ?*/
        if(bonus->y > gameObj.game.bb.height)
        {
            /*Deactivate the bonus*/
            gameObj.toPrint[i].display = false;

            continue;
        }

        /*Check collisions*/
        if(bonusCollisions(bonus))
        {
            /*Deactivate the bonus*/
            gameObj.toPrint[i].display = false;
        }
    }

}
