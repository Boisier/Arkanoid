#include "../includes/game.h"

/**
 * Init gameObj with default values
 */
void initGame()
{
    gameObj.WINDOW_WIDTH = 1200;
    gameObj.WINDOW_HEIGHT = 800;

    strcpy(gameObj.theme, "themes/default/");

    gameObj.defVal.plateforme.size = 100;
    gameObj.defVal.plateforme.height = 20;
    gameObj.defVal.plateforme.maxSpeed = 25;
    gameObj.defVal.plateforme.acceleration = .5;

    gameObj.defVal.brick.width = 50;
    gameObj.defVal.brick.height = 20;
    
    gameObj.defVal.ball.size = 20;
    gameObj.defVal.ball.minSpeed = 5; 
    gameObj.defVal.ball.maxSpeed = 20; /*= gameObj.defVal.plateforme.height*/
    gameObj.defVal.ball.maxStartAngle = 90;

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

    gameObj.nbrHumanPlayers = 0;
    gameObj.players = NULL;
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
    }
    else if(callback == 1 || callback == 2)
    {
        gameObj.nbrHumanPlayers = callback;
        gameObj.gameState = STARTGAME;
    }
}

void startGame()
{
    if(gameObj.players != NULL)
    {
        /*Free players*/
    }
    
    gameObj.nbrPlayers = 2;
    gameObj.players = allocate(sizeof(Player *) * gameObj.nbrPlayers);

    createPlayer(HUMAN, 1);

    if(gameObj.nbrHumanPlayers == 1)
        createPlayer(AI, 2);
    else
        createPlayer(HUMAN, 2);

    gameObj.gameState = INGAME;
}

void createPlayer(enum PlayerType type, int playerNbr)
{   
    int platX = (gameObj.WINDOW_WIDTH / 2 - gameObj.defVal.plateforme.size / 2);
    int platY;

    Player * player = allocate(sizeof(Player));

    if(playerNbr == 1)
    {
        player->playerPos = BOTTOM;
        platY = gameObj.WINDOW_HEIGHT - 50;
    }
    else
    {
        player->playerPos = TOP;
        platY = 50;

    }
    player->type = type;
    player->life = gameObj.defVal.lifeNbr;

    player->plateforme = createPlateforme(platX, platY, player->playerPos);

    gameObj.players[playerNbr - 1] = player;
}

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