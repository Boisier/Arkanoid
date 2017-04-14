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
    cleanToPrint();
    freeTextures();
}

void doThings()
{
    switch(gameObj.gameState)
    {
        case MAINMENU:
            mainMenu();
        break;
        case PLAYERSELECTION:
            playerSelection();
        default:
            return;
    }
}

/*Handle main menu*/
void mainMenu()
{
    char callback;

    if(gameObj.printContent != MAINMENU)
    {
        cleanToPrint();
        createMainMenu();
        return;
    }

    callback = btnHandler();

    /*Do something with the callback*/
    if(callback == 's')
    {
        gameObj.gameState = PLAYERSELECTION;
    }
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

void playerSelection()
{
    char callback;

    if(gameObj.printContent != PLAYERSELECTION)
    {
        cleanToPrint();
        createPlayerSelection();
        return;
    }

    callback = btnHandler();

    if(callback == 's')
    {
        return;
    }
}

/*Create Mainmenu elements*/
void createPlayerSelection()
{
    Picture * background;
    Button * onePlayerBtn, * twoPlayerBtn, * backBtn;

    background = createPicture(-600, 400, "background.png");

    onePlayerBtn = createButton(-171, -78, 342, 52, 's');                /*'s' for start*/
    onePlayerBtn->idleTexture = getTexture("onePlayerBtn_idle.png");
    onePlayerBtn->selectedTexture = getTexture("onePlayerBtn_selected.png");
    onePlayerBtn->state = SELECTED;

    twoPlayerBtn = createButton(-171, -195, 342, 52, 'r');              /*'r' for rules*/
    twoPlayerBtn->idleTexture = getTexture("twoPlayerBtn_idle.png");
    twoPlayerBtn->selectedTexture = getTexture("twoPlayerBtn_selected.png");
    twoPlayerBtn->state = IDLE;

    backBtn = createButton(-171, -312, 342, 52, 'r');              /*'r' for rules*/
    backBtn->idleTexture = getTexture("backBtn_idle.png");
    backBtn->selectedTexture = getTexture("backBtn_selected.png");
    backBtn->state = IDLE;

    onePlayerBtn->bottomBtn = twoPlayerBtn;
    twoPlayerBtn->topBtn = onePlayerBtn;
    twoPlayerBtn->bottomBtn = backBtn;
    backBtn->topBtn = twoPlayerBtn;

    addToPrint(background, PICTURE);
    addToPrint(onePlayerBtn, BUTTON);
    addToPrint(twoPlayerBtn, BUTTON);
    addToPrint(backBtn, BUTTON);

    gameObj.printContent = PLAYERSELECTION;
    gameObj.currentlySelectedBtn = onePlayerBtn;
}