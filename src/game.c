#include "../includes/game.h"

/** Main loop of the app**/
void theLoop()
{
    Uint32 startTime, elapsedTime;
    
    while(gameObj.gameState != EXITING) 
    {
        startTime = SDL_GetTicks();

        /*Clear the screen*/
        glClear(GL_COLOR_BUFFER_BIT);

        /*Execute game actions*/
        doThings();

        /*Print everything on the screen*/
        printScreen();
        SDL_GL_SwapBuffers();

        /*Handle SDL events*/
        watcher();
        
        /*In case we're too fast ;)*/
        elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    
    /** Free all used memory **/
    cleanToPrint();
    freeTextures();
    freePlayers();
    freeFont(gameObj.defaultFont);
}


/** Call current game state function **/
void doThings()
{
    switch(gameObj.gameState)
    {
        case MAINMENU:        mainMenu();        break;
        case THEMESELECTION:  themeSelection();  break;
        case PLAYERSELECTION: playerSelection(); break;
        case LEVELSELECTION:  levelSelection(); break;
        case STARTGAME:       startGame();       break;
        case INGAME:          ingame();          break;
        case ENDGAME:         endgame();         break;
        default: return;
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
        /*jouer son ici*/
        gameObj.gameState = PLAYERSELECTION;
    }

    if(callback == 't')
    {
        gameObj.gameState = THEMESELECTION;
    }

    if(callback == 'q')
    {
        gameObj.gameState = EXITING;
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
    
    if(callback == 'p')
    {
        gameObj.game.humans = gameObj.playerSelection.humans->value;
        gameObj.game.computers = gameObj.playerSelection.computers->value;

        gameObj.gameState = LEVELSELECTION;
    }
}


/** Select the bricks layout **/
void levelSelection()
{
    int callback;

    if(gameObj.printContent != LEVELSELECTION)
    {
        cleanToPrint();
        createLevelSelection();
        return;
    } 

    callback = btnHandler();

    if(callback == 'b')
    {
        gameObj.gameState = PLAYERSELECTION;
        return;
    }
    else if(callback == -1 || callback > 0)
    {
        gameObj.game.levelID = callback;
        gameObj.gameState = STARTGAME;
    }

}


/** Start the game **/
void startGame()
{
    int i, p;
    char pos[11][7];

    if(gameObj.game.players != NULL)
        freePlayers(); /*Free previous players*/
        
    /*Make sure there is at least two players*/
    if(gameObj.game.humans == 1 && gameObj.game.computers == 0)
    {
        gameObj.game.computers = 1;
    }

    gameObj.game.nbrPlayers = gameObj.game.humans + gameObj.game.computers;

    /*Board layout for all the different configurations*/

    /*  HA     -     HA  */  pos[0][0] = 'H';  pos[0][1] = 'A';
    /*  HAA    -    HAA  */  pos[1][0] = 'H';  pos[1][1] = 'A';  pos[1][2] = 'A'; 
    /*  HAAA   -   HAAA  */  pos[2][0] = 'H';  pos[2][1] = 'A';  pos[2][2] = 'A';  pos[2][3] = 'A'; 
    /*  HH     -     HH  */  pos[3][0] = 'H';  pos[3][1] = 'H';   
    /*  HHA    -    AHH  */  pos[4][0] = 'A';  pos[4][1] = 'H';  pos[4][2] = 'H';  
    /*  HHAA   -   HAHA  */  pos[5][0] = 'H';  pos[5][1] = 'A';  pos[5][2] = 'H';  pos[5][3] = 'A'; 
    /*  HHAAA  -  AHAAH  */  pos[6][0] = 'A';  pos[6][1] = 'H';  pos[6][2] = 'A';  pos[6][3] = 'A';  pos[6][4] = 'H'; 
    /*  HHH    -    HHH  */  pos[7][0] = 'H';  pos[7][1] = 'H';  pos[7][2] = 'H';  
    /*  HHHA   -   HHAH  */  pos[8][0] = 'H';  pos[8][1] = 'H';  pos[8][2] = 'A';  pos[8][3] = 'H'; 
    /*  HHHAA  -  HAHHA  */  pos[9][0] = 'H';  pos[9][1] = 'A';  pos[9][2] = 'H';  pos[9][3] = 'H';  pos[9][4] = 'A'; 
    /*  HHHAAA - HAHAHA  */ pos[10][0] = 'H'; pos[10][1] = 'A'; pos[10][2] = 'H'; pos[10][3] = 'A'; pos[10][4] = 'H'; pos[10][5] = 'A'; 

    if(gameObj.game.humans == 1 && gameObj.game.computers == 1)
        p = 0;
    else if(gameObj.game.humans == 1 && gameObj.game.computers == 2)
        p = 1;
    else if(gameObj.game.humans == 1 && gameObj.game.computers == 3)
        p = 2;
    else if(gameObj.game.humans == 2 && gameObj.game.computers == 0)
        p = 3;
    else if(gameObj.game.humans == 2 && gameObj.game.computers == 1)
        p = 4;
    else if(gameObj.game.humans == 2 && gameObj.game.computers == 2)
        p = 5;
    else if(gameObj.game.humans == 2 && gameObj.game.computers == 3)
        p = 6;
    else if(gameObj.game.humans == 3 && gameObj.game.computers == 0)
        p = 7;
    else if(gameObj.game.humans == 3 && gameObj.game.computers == 1)
        p = 8;
    else if(gameObj.game.humans == 3 && gameObj.game.computers == 2)
        p = 9;
    else /*if(gameObj.game.humans == 3 && gameObj.game.computers == 3)*/
        p = 10;


    /*Create bounding box*/
    defineBoundingBox();

    /*Create players*/
    gameObj.game.players = allocate(sizeof(Player *) * gameObj.game.nbrPlayers);

    gameObj.game.humans = 0;

    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        if(pos[p][i] == 'H')
        {
            /*Insert Human*/
            createPlayer(HUMAN, i);
            gameObj.game.players[i]->controls = gameObj.game.humans;
            gameObj.game.humans++;

            /*Set bbox as reversed to ensure controls direction logic*/
            if(bboxIsReversed(i))
                gameObj.game.players[i]->reversed = true;
        }
        else
        {
            /*Insert computer*/
            createPlayer(AI, i);
        }
    }

    gameObj.gameState = INGAME;
}


/** Create a new player **/
void createPlayer(enum PlayerType type, int playerNbr)
{   
    /*Define plateforme position*/
    int platX = 0 - gameObj.defVal.plateforme.size * .5;
    int platY = gameObj.game.bb.height - gameObj.defVal.plateforme.level;

    /*Create the player*/
    Player * player = allocate(sizeof(Player));
    gameObj.game.players[playerNbr] = player;

    player->type = type;
    player->life = gameObj.defVal.lifeNbr;

    /*Create the player's plateforme*/
    player->plateforme = createPlateforme(platX, platY);
    player->plateforme->BBox = playerNbr;

    player->reversed = false;
}


/** Handle ingame events **/
void ingame()
{
    int callback = 0;

    if(gameObj.printContent != INGAME)
    {
        cleanToPrint();
        createGameBoard();

        gameObj.game.pause = false;
        return;
    }

    /*Can we execute the game ?*/
    if(!gameObj.game.play)
    {
        if(!gameObj.game.starting)
        {
            createStartGameAnimation();
        }

        return;
    }

    /*Shall we toggle the pauseMenu ?*/
    if(gameObj.keys.esc)
    {
        if(gameObj.game.pause)
            hidePause(); /*Pause the game*/
        else
            enterPause(); /*Unpause the game*/

        gameObj.keys.esc = false;
    }

    /*Is the game paused*/
    if(gameObj.game.pause)
    {   
        /*Wait for user actions*/
        callback = btnHandler();

        if(callback == 'p')   
            hidePause(); /*HideMenu*/
        else if(callback == 'q')
            gameObj.gameState = PLAYERSELECTION;

        return;
    }

    /*Animate the gmae*/
    playerMovements();  /*Players/Plateformes*/

    ballMovements();    /*Balls*/

    bonusMovements();   /*Bonus*/
}


/*pause the game*/
void enterPause()
{
    gameObj.game.pause = true; /*pause the game*/

    createPauseMenu();
}


/*Hide the game*/
void hidePause()
{
    createFloatAnimation(&gameObj.game.pauseMenu.background->element.pict->opacity, .8, .0, 500, 0, QUAD, &quitPause);
    createFloatAnimation(&gameObj.game.pauseMenu.playBtn->element.btn->opacity, 1.0, .0, 500, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.pauseMenu.quitBtn->element.btn->opacity, 1.0, .0, 500, 0, QUAD, NULL);
}


/*restarte the game*/
void quitPause()
{
    gameObj.game.pauseMenu.background->display = false;
    gameObj.game.pauseMenu.playBtn->display = false;
    gameObj.game.pauseMenu.quitBtn->display = false;
    gameObj.game.pause = false; /*Unpause the game*/
}

/* Displauy end game sequence and free memory*/
void endgame()
{
    static bool displayed = false;
    int i;
    char caption[50], * temp;

    if(gameObj.printContent != ENDGAME)
    {
        addToPrint(createText("FIN", 0, 0, gameObj.defaultFont), TEXT);

        gameObj.printContent = ENDGAME;
        return;
    }

    SDL_Delay(2000);

    if(!displayed)
    {  
        displayed = true;

        if(gameObj.game.computers > 0)
        {
            strcpy(caption, "L ordinateur remporte la partie");
            addToPrint(createText(caption, 0, -60, gameObj.defaultFont), TEXT);
            return;
        }

        for(i = 0; i < gameObj.game.nbrPlayers; ++i)
        {
            if(gameObj.game.players[i]->life != 0)
            {
                strcpy(caption, "Joueur ");
                temp = itoa(i+1);
                strcat(caption, temp);
                free(temp);
                strcat(caption, " gagne la partie");
                addToPrint(createText(caption, 0, -60, gameObj.defaultFont), TEXT);
            }
        }

        return;
    }

    SDL_Delay(2000);

    gameObj.gameState = PLAYERSELECTION;
}
