#include "../includes/game.h"

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
		return callback;
    }

    if(gameObj.keys.up && btn->topBtn != NULL)
    {
        btn->state = IDLE;
        btn->topBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->topBtn;
		return 0;
    }
    
	if(gameObj.keys.down && btn->bottomBtn != NULL)
    {
        btn->state = IDLE;
        btn->bottomBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->bottomBtn;
		return 0;
    }

	return callback;
}