#include "../includes/game.h"

/* Music */
Mix_Music * menuMusic = NULL;
char menuMusicPath[256];
Mix_Music * battleMusic = NULL;
char battleMusicPath[256];

/* Sounds */
Mix_Chunk * okSound = NULL;
Mix_Chunk * backSound = NULL;
Mix_Chunk * moveSound = NULL;
Mix_Chunk * pauseSound = NULL;
Mix_Chunk * unpauseSound = NULL;

/** Main loop of the app**/
void theLoop()
{
    Uint32 startTime, elapsedTime;

    /*Load musics*/
    strcpy(menuMusicPath, gameObj.theme);
    strcat(menuMusicPath, "sounds/menu.mp3");
    menuMusic = Mix_LoadMUS(menuMusicPath);
    strcpy(battleMusicPath, gameObj.theme);
    strcat(battleMusicPath, "sounds/battle.mp3");
    battleMusic = Mix_LoadMUS(battleMusicPath);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    /*Load general sounds*/
    okSound = Mix_LoadWAV("./themes/default/sounds/ok.wav");
    backSound = Mix_LoadWAV("./themes/default/sounds/back.wav");
    moveSound = Mix_LoadWAV("./themes/default/sounds/move.wav");
    pauseSound = Mix_LoadWAV("./themes/default/sounds/pause.wav");
    unpauseSound = Mix_LoadWAV("./themes/default/sounds/unpause.wav");

    Mix_PlayMusic(menuMusic, -1);

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
    Mix_FreeChunk(okSound);
    Mix_FreeChunk(backSound);
    Mix_FreeChunk(moveSound);
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(battleMusic);
    freeFont(gameObj.defaultFont);
    Mix_CloseAudio();
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

    callback = btnHandler(moveSound);

    /*Do something with the callback*/
    if(callback == 's')
    {
        Mix_PlayChannel( -1, okSound, 0 );
        gameObj.gameState = PLAYERSELECTION;
    }

    if(callback == 't')
    {
        Mix_PlayChannel( -1, okSound, 0 );
        gameObj.gameState = THEMESELECTION;
    }

    if(callback == 'q')
    {
        Mix_PlayChannel( -1, backSound, 0 );
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

    callback = btnHandler(moveSound);

    if(callback == 'b')
    {
        Mix_PlayChannel( -1, backSound, 0 );
        gameObj.gameState = MAINMENU;
        return;
    }
    
    if(callback != 0)
    {
        /*Apply new theme*/
        setTheme(callback);
        Mix_FreeMusic(menuMusic);
        Mix_FreeMusic(battleMusic);
        freeFont(gameObj.defaultFont);
        loadSDLDependants();

        /*Load new musics*/
        strcpy(menuMusicPath, gameObj.theme);
        strcat(menuMusicPath, "sounds/menu.mp3");
        menuMusic = Mix_LoadMUS(menuMusicPath);
        strcpy(battleMusicPath, gameObj.theme);
        strcat(battleMusicPath, "sounds/battle.mp3");
        battleMusic = Mix_LoadMUS(battleMusicPath);

        Mix_PlayChannel( -1, okSound, 0 );
        gameObj.gameState = MAINMENU;
        Mix_PlayMusic(menuMusic, -1);
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

    callback = btnHandler(moveSound);

    if(callback == 'b')
    {
        Mix_PlayChannel( -1, backSound, 0 );
        gameObj.gameState = MAINMENU;
        return;
    }
    
    if(callback == 'p')
    {
        gameObj.game.humans = gameObj.playerSelection.humans->value;
        gameObj.game.computers = gameObj.playerSelection.computers->value;

        Mix_PlayChannel( -1, okSound, 0 );
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

    callback = btnHandler(moveSound);

    if(callback == 'b')
    {
        Mix_PlayChannel( -1, backSound, 0 );
        gameObj.gameState = PLAYERSELECTION;
        return;
    }
    else if(callback == -1 || callback > 0)
    {
        gameObj.game.levelID = callback;
        Mix_PlayChannel( -1, okSound, 0 );
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

    /*Switch to battle music*/
    Mix_HaltMusic();
    Mix_PlayMusic(battleMusic, -1);

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
    player->bonusText = NULL;
    player->currentBonusAnim = NULL;

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
        gameObj.keys.esc = false;
        
        if(gameObj.game.pause)
        {
            Mix_PlayChannel( -1, unpauseSound, 0 );
            hidePause(); /*Pause the game*/
        }
        else
        {
            Mix_PlayChannel( -1, pauseSound, 0 );
            enterPause(); /*Unpause the game*/
        }
    }

    /*Is the game paused*/
    if(gameObj.game.pause)
    {   
        /*Wait for user actions*/
        callback = btnHandler(moveSound);

        if(callback == 'p')
        {
            Mix_PlayChannel( -1, unpauseSound, 0 );   
            hidePause(); /*HideMenu*/
        }
        else if(callback == 'q')
        {
            Mix_PlayChannel( -1, backSound, 0 );
            gameObj.gameState = PLAYERSELECTION;

            /*Switch to menu music*/
            Mix_HaltMusic();
            Mix_PlayMusic(menuMusic, -1);
        }

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
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    createPauseMenu();
}


/*Hide the game*/
void hidePause()
{
    createFloatAnimation(&gameObj.game.pauseMenu.background->element.pict->opacity, .8, .0, 200, 0, QUAD, &quitPause);
    createFloatAnimation(&gameObj.game.pauseMenu.playBtn->element.btn->opacity, 1.0, .0, 200, 0, QUAD, NULL);
    createFloatAnimation(&gameObj.game.pauseMenu.quitBtn->element.btn->opacity, 1.0, .0, 200, 0, QUAD, NULL);
}


/*restarte the game*/
void quitPause()
{
    gameObj.game.pauseMenu.background->display = false;
    gameObj.game.pauseMenu.playBtn->display = false;
    gameObj.game.pauseMenu.quitBtn->display = false;
    gameObj.game.pause = false; /*Unpause the game*/
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}

/* Displauy end game sequence and free memory*/
void endgame()
{
    int i;
    char caption[50], * temp;
    Text * endText = NULL;

    if(gameObj.printContent != ENDGAME)
    {
        addToPrint(createText("FIN", 0, 0, gameObj.defaultFont), TEXT);

        if(gameObj.game.computers > 0)
        {
            strcpy(caption, "L ordinateur remporte la partie");
            endText = createText(caption, 0, -60, gameObj.defaultFont);
        }
        else
        {
            for(i = 0; i < gameObj.game.nbrPlayers; ++i)
            {
                if(gameObj.game.players[i]->life != 0)
                {
                    strcpy(caption, "Joueur ");
                    temp = itoa(i+1);
                    strcat(caption, temp);
                    free(temp);
                    strcat(caption, " gagne la partie");
                    endText = createText(caption, 0, -60, gameObj.defaultFont);
                }
            }
        }
        
        addToPrint(endText, TEXT);

        endText->opacity = 0.0;
    	createFloatAnimation(&endText->opacity, 0.0, 1.0, 200, 1000, QUAD, &endAnimationA);

        return;
    }

    /*Switch to menu music*/
    Mix_HaltMusic();
    Mix_PlayMusic(menuMusic, -1);

    gameObj.gameState = PLAYERSELECTION;
}
