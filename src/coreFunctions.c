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
                case SDLK_SPACE   : gameObj.keys.space = newKeyValue; break;
                case SDLK_RETURN  :
                case SDLK_KP_ENTER:   /*Let's not forget the keypad*/
                                    gameObj.keys.enter = newKeyValue; break;
                case SDLK_a       : gameObj.keys.a     = newKeyValue; break;
                case SDLK_z       : gameObj.keys.z     = newKeyValue; break;
                case SDLK_ESCAPE  : gameObj.keys.esc   = newKeyValue; break;
                default: /*Do nothing for other keys*/ break;
            }
        }
    }
}

/** Handle menu interactions **/
char btnHandler()
{
	char callback = 0;
	Button * btn;

	if(!gameObj.keys.up && !gameObj.keys.down && !gameObj.keys.left && !gameObj.keys.right && !gameObj.keys.enter)
    {
        return 0;                             /*None of the keys we need are pressed, no need to go further*/
    }

    btn = gameObj.currentlySelectedBtn;

    if(gameObj.keys.enter)
    {
        callback = btn->callback;
        
        /*Deactivate the key*/
        gameObj.keys.enter = false;

		return callback;
    }

    if(gameObj.keys.up && btn->topBtn != NULL)
    {
        btn->state = IDLE;
        btn->topBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->topBtn;

        /*Deactivate the key*/
        gameObj.keys.up = false;

		return 0;
    }
    
	if(gameObj.keys.down && btn->bottomBtn != NULL)
    {
        btn->state = IDLE;
        btn->bottomBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->bottomBtn;
        
        /*Deactivate the key*/
        gameObj.keys.down = false;

		return 0;
    }

	return callback;
}

/** Handle movement of the players **/
void playerMovements()
{
    int i;
    float speedFactor, speedBonus;
    Player * player;
    Plateforme * plateforme;
    bool leftKey, rightKey;

    for(i = 0; i < gameObj.nbrPlayers; ++i)
    {
        player = gameObj.players[i];

        if(player->type == AI)
        {
            /*Handle AI*/
            continue;
        }

        /*Human Player*/
        if(player->playerPos == TOP)
        {
            leftKey = gameObj.keys.a;
            rightKey = gameObj.keys.z;
        }
        else
        {
            leftKey = gameObj.keys.left;
            rightKey = gameObj.keys.right;
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

        if(plateforme->speed > gameObj.defVal.plateforme.maxSpeed)
            plateforme->speed = gameObj.defVal.plateforme.maxSpeed;
        
        if(leftKey)
        {
            /*Move top plateforme to the left*/
            plateforme->x -= plateforme->speed; 

            plateforme->dirFactor = -1;

            /*Are we still in the game area ?*/
            if(plateforme->x < 0)
            {
                plateforme->x = 0;
                plateforme->speed = 0;
            }

        }
        else if(rightKey)
        {
            /*Move top plateforme to the right*/
            plateforme->x += plateforme->speed; 

            plateforme->dirFactor = 1;

            /*Are we still in the game area ?*/
            if(plateforme->x + plateforme->size > gameObj.wWidth)
            {
                plateforme->x = gameObj.wWidth - plateforme->size;
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
            /*Deactivate the ball*/
            gameObj.toPrint[i].display = false;
            gameObj.players[player]->life--;
            
            /*Create a new ball for the ball owner*/
            ball = createBall(0, 0, ball->playerID);
            ball->gluedPlat = gameObj.players[ball->playerID]->plateforme;
            ball->glueOffsetX = gameObj.players[ball->playerID]->plateforme->size / 2;
            addToPrint(ball, BALL);

            continue;
        }

        /*Check collisions*/
        ballCollisions(ball);
    }
}