#include "../includes/game.h"

/** Init gameObj with default values **/
void initGame()
{
    gameObj.wWidth = 1200;
    gameObj.wHeight = 800;

    strcpy(gameObj.theme, "themes/default/");

    gameObj.defVal.plateforme.size = 100;
    gameObj.defVal.plateforme.height = 15;
    gameObj.defVal.plateforme.maxSpeed = 25;
    gameObj.defVal.plateforme.acceleration = .5;
    gameObj.defVal.plateforme.level = 35;

    gameObj.defVal.brick.width = 50;
    gameObj.defVal.brick.height = 20;
    
    gameObj.defVal.ball.size = 15;
    gameObj.defVal.ball.minSpeed = 5; 
    gameObj.defVal.ball.maxSpeed = 20; /*= gameObj.defVal.plateforme.height*/
    gameObj.defVal.ball.maxAngle = 90;

    gameObj.defVal.bonus.size = 10;
    gameObj.defVal.bonus.speed = 10;

    gameObj.defVal.lifeNbr = 3;

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

    gameObj.game.nbrPlayers = 0;
    gameObj.game.players = NULL;
}

/** Main loop of the app**/
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

/** Call current game state function **/
void doThings()
{
    switch(gameObj.gameState)
    {
        case MAINMENU:
            mainMenu();
        break;
        case THEMESELECTION:
            themeSelection();
        break;
        case PLAYERSELECTION:
            playerSelection();
        break;
        case STARTGAME:
            startGame();
        break;
        case INGAME:
            ingame();
        break;
        default:
            return;
    }
}

/** Handle the main menu **/
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

    if(callback == 't')
    {
        gameObj.gameState = THEMESELECTION;
    }
}



/** Theme switch screen **/
void themeSelection()
{
    char callback;

    if(gameObj.printContent != THEMESELECTION)
    {
        cleanToPrint();
        createThemeSelection();
    }

    callback = btnHandler();

    if(callback == 'b')
    {
        gameObj.gameState = MAINMENU;
        return;
    }
    
    if(callback != 0)
    {
        /*Apply new theme*/
        setTheme(callback);
        gameObj.gameState = MAINMENU;
    }
}


/** Handle nbr of player selection **/
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

    if(callback == 'b')
    {
        gameObj.gameState = MAINMENU;
        return;
    }
    
    if(callback != 0)
    {
        gameObj.game.nbrPlayers = callback;
        gameObj.gameState = STARTGAME;
    }
}

/** Start the game **/
void startGame()
{
    int i;
    bool addAI = false;

    if(gameObj.game.players != NULL)
    {
        /*Free players*/
    }
    
    /*Handle only one human player*/
    if(gameObj.game.nbrPlayers == 1)
    {
        gameObj.game.nbrPlayers = 2;
        addAI = true;
    }

    
    
    /********************************/
    gameObj.game.nbrPlayers = 8;
    /********************************/


    /*Create bounding box*/
    defineBoundingBox();

    printf("> %d\n", gameObj.game.nbrPlayers);
    gameObj.game.players = allocate(sizeof(Player *) * gameObj.game.nbrPlayers);

    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        if(addAI == true && i+1 == gameObj.game.nbrPlayers)
        {
            /*If it's the last insert and we need an AI -> add an AI*/
            createPlayer(AI, i);
            break;
        }

        createPlayer(HUMAN, i);
    }

    gameObj.gameState = INGAME;
}

/** Create a new player **/
void createPlayer(enum PlayerType type, int playerNbr)
{   
    /**Define plateforme position **/
    int platX = 0 - gameObj.defVal.plateforme.size / 2;
    int platY = gameObj.game.bb.height - gameObj.defVal.plateforme.level;

    Player * player = allocate(sizeof(Player));

    player->type = type;
    player->life = gameObj.defVal.lifeNbr;

    player->plateforme = createPlateforme(platX, platY);
    player->plateforme->BBox = playerNbr;

    gameObj.game.players[playerNbr] = player;
}

/** Handle ingame events **/
void ingame()
{
    if(gameObj.printContent != INGAME)
    {
        cleanToPrint();
        createGameBoard();
        return;
    }

    playerMovements();

    ballMovements();
}
