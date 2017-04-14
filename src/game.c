#include "../includes/game.h"

/**
 * Init gameObj with default values
 */
void initGame()
{
    gameObj.WINDOW_WIDTH = 1200;
    gameObj.WINDOW_HEIGHT = 800;

    strcpy(gameObj.theme, "themes/default/");

    gameObj.platformeHeight = 5;
    gameObj.platMaxSpeed = 25;
    gameObj.platSpeedUpFactor = 2.5;

    gameObj.brickWidth = 50;
    gameObj.brickHeight = 20;
    
    gameObj.ballSize = 15;
    gameObj.defaultSpeed = 5; 

    gameObj.bonusSize = 10;
    gameObj.bonusSpeed = 10;

    gameObj.gameState = MAINMENU;

    gameObj.textures = NULL;
    gameObj.nbrTextures = 0;

    gameObj.toPrint = NULL;
    gameObj.nbrToPrint = 0;
    gameObj.printContent = EMPTY;

    gameObj.keys.up = false;
    gameObj.keys.down = false;
    gameObj.keys.left = false;
    gameObj.keys.right = false;
    gameObj.keys.space = false;
    gameObj.keys.enter = false;
    gameObj.keys.a = false;
    gameObj.keys.z = false;
    gameObj.keys.esc = false;

    gameObj.currentlySelectedBtn = NULL;
}

void theLoop()
{
    Uint32 startTime, elapsedTime;
    
    while(gameObj.gameState != EXITING) 
    {
        startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        doThings();

        printScreen();

        SDL_GL_SwapBuffers();

        watcher();
        
        elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    
    /** TODO : Clean up app **/
}

void doThings()
{
    switch(gameObj.gameState)
    {
        case MAINMENU:
            mainMenu();
        break;
        default:
            return;
    }
}

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

/*Handle main menu*/
void mainMenu()
{
    Button * btn;
    char callback = 0;

    if(gameObj.printContent != MAINMENU)
    {
        createMainMenu();
        return;
    }

    if(!gameObj.keys.up && !gameObj.keys.down && !gameObj.keys.enter)
    {
        return;                             /*None of the keys we need are pressed, no need to go further*/
    }

    btn = gameObj.currentlySelectedBtn;

    if(gameObj.keys.enter)
    {
        callback = btn->callback;
    }
    else if(gameObj.keys.up && btn->topBtn != NULL)
    {
        btn->state = IDLE;
        btn->topBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->topBtn;
    }
    else if(gameObj.keys.down && btn->bottomBtn != NULL)
    {
        btn->state = IDLE;
        btn->bottomBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = btn->bottomBtn;
    }

    /*Do something with the callback*/
    printf("%d", callback);
}

/*Create Mainmenu elements*/
void createMainMenu()
{
    Picture * background;
    Button * playBtn, * rulesBtn;

    background = createPicture(-600, 400, "background.png");

    playBtn = createButton(-171, -78, 342, 52, 's');                /*'s' for start*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");
    playBtn->state = SELECTED;

    rulesBtn = createButton(-171, -195, 342, 52, 'r');              /*'r' for rules*/
    rulesBtn->idleTexture = getTexture("rulesBtn_idle.png");
    rulesBtn->selectedTexture = getTexture("rulesBtn_selected.png");
    rulesBtn->state = IDLE;

    playBtn->bottomBtn = rulesBtn;
    rulesBtn->topBtn = playBtn;

    addToPrint(background, PICTURE);
    addToPrint(playBtn, BUTTON);
    addToPrint(rulesBtn, BUTTON);

    gameObj.printContent = MAINMENU;
    gameObj.currentlySelectedBtn = playBtn;
}
