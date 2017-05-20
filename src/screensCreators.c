# include "../includes/game.h"

/*Create Mainmenu elements*/
void createMainMenu()
{
    Picture * background;
    Button * playBtn, * rulesBtn, * themeBtn;

    background = createPicture(0, 0, "background.png");

    playBtn = createButton(0, -78, 342, 52, 's');                /*'s' for start*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");
    playBtn->state = SELECTED;

    rulesBtn = createButton(-370, -165, 342, 52, 'r');              /*'r' for rules*/
    rulesBtn->idleTexture = getTexture("rulesBtn_idle.png");
    rulesBtn->selectedTexture = getTexture("rulesBtn_selected.png");
    rulesBtn->state = IDLE;

    themeBtn = createButton(0, -311, 342, 52, 't');              /*'t' for themes*/
    themeBtn->idleTexture = getTexture("themeBtn_idle.png");
    themeBtn->selectedTexture = getTexture("themeBtn_selected.png");
    themeBtn->state = IDLE;

    playBtn->bottomBtn = rulesBtn;
    rulesBtn->topBtn = playBtn;
    rulesBtn->bottomBtn = themeBtn;
    themeBtn->topBtn = rulesBtn;

    addToPrint(background, PICTURE);
    addToPrint(playBtn, BUTTON);
    addToPrint(rulesBtn, BUTTON);
    addToPrint(themeBtn, BUTTON);

    gameObj.printContent = MAINMENU;
    gameObj.currentlySelectedBtn = playBtn;
}


/** Create theme selection screen **/
void createThemeSelection()
{
    Picture * background = NULL;
    Button * tempBtn = NULL, * lastBtn = NULL, * backBtn = NULL;
    char currentTheme[256], themePath[256];
    int i = 0;

    FolderContent * folder = NULL;

    strcpy(currentTheme, gameObj.theme);

    /** First, the background*/
    background = createPicture(0, 0, "background.png");
    addToPrint(background, PICTURE);

    /*List available themes*/
    folder = parseFolder("./themes");

    /*For each folder*/
    for(i = 0; i < folder->nbrElements && i < 5; ++i)
    {
        /*Build path*/
        strcpy(themePath, "./themes/");
        strcat(themePath, folder->elements[i]->d_name);
        strcat(themePath, "/");

        /*Set as current theme*/
        strcpy(gameObj.theme, themePath);

        /*Create the button*/
        tempBtn = createButton(0, 300 - 111 * i, 342, 52, i + 1);
        tempBtn->idleTexture = getTexture("themeSelectorBtn_idle.png");
        tempBtn->selectedTexture = getTexture("themeSelectorBtn_selected.png");
        
        if(i == 0)
        {
            tempBtn->state = SELECTED;
            gameObj.currentlySelectedBtn = tempBtn;
        }

        /*Add interactions*/
        if(lastBtn != NULL)
        {
            tempBtn->topBtn = lastBtn;
            lastBtn->bottomBtn = tempBtn;
        }

        addToPrint(tempBtn, BUTTON);

        lastBtn = tempBtn;
        tempBtn = NULL;

        /*Revert to normal*/
        themePath[0] = '\0';
        strcpy(gameObj.theme, currentTheme);
    }

    closeFolder(folder);

    backBtn = createButton(0, -312, 342, 52, 'b');              /*'b' for back*/
    backBtn->idleTexture = getTexture("backBtn_idle.png");
    backBtn->selectedTexture = getTexture("backBtn_selected.png");

    if(i == 0)
    {
        backBtn->state = SELECTED;
        gameObj.currentlySelectedBtn = backBtn;

        addToPrint(createText("Une erreur est survenue", 0, 100, gameObj.defaultFont), TEXT);
    }
    else
    {
        backBtn->topBtn = lastBtn;
        lastBtn->bottomBtn = backBtn;
    }

    addToPrint(backBtn, BUTTON);

    gameObj.printContent = THEMESELECTION;
}



/*Create Mainmenu elements*/
void createPlayerSelection()
{
    Picture * background;
    Button * playBtn, * backBtn;
    NumberBox * humans, * computers;
    Text * humansText, * computersText;

    background = createPicture(0, 0, "background.png");

    /*Humans*/
    humansText = createText("Joueurs", 0, 220, gameObj.defaultFont);

    humans = createNumberBox(0, 175, 342, 52, 2, 1, 3);
    humans->btn->state = SELECTED;
    humans->display->color = gameObj.selectedTextColor;

    /*Computers*/
    computersText = createText("ordinateurs", 0, 20, gameObj.defaultFont);

    computers = createNumberBox(0, -25, 342, 52, 0, 0, 3);

    /*Buttons*/
    playBtn = createButton(0, -195, 342, 52, 'p');               /*'p' for play*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");

    backBtn = createButton(0, -312, 342, 52, 'b');              /*'b' for back*/
    backBtn->idleTexture = getTexture("backBtn_idle.png");
    backBtn->selectedTexture = getTexture("backBtn_selected.png");

    /*Interactions*/
    humans->btn->bottomBtn = computers->btn;
    computers->btn->topBtn = humans->btn;
    computers->btn->bottomBtn = playBtn;
    playBtn->topBtn = computers->btn;
    playBtn->bottomBtn = backBtn;
    backBtn->topBtn = playBtn;

    /*Printing*/
    addToPrint(background, PICTURE);
    addToPrint(humansText, TEXT);
    addToPrint(humans, NUMBERBOX);
    addToPrint(computersText, TEXT);
    addToPrint(computers, NUMBERBOX);
    addToPrint(playBtn, BUTTON);
    addToPrint(backBtn, BUTTON);

    gameObj.printContent = PLAYERSELECTION;
    gameObj.currentlySelectedBtn = humans->btn;

    /*Store in gameObj for later retreiving*/
    gameObj.playerSelection.humans = humans;
    gameObj.playerSelection.computers = computers;
}


/** Create the brick layout selection **/
void createLevelSelection()
{
    Picture * background;
    Button * backBtn, * lvlArrow = NULL, * lastArrow = NULL;
    Text * lvlName;
    FolderContent * levelsFolder = parseFolder("./levels");
    int i;

    /*Background*/
    background = createPicture(0, 0, "background.png");
    addToPrint(background, PICTURE);

    /*Text Action*/
    addToPrint(createText("Choix du niveau", 0, 240, gameObj.defaultFont), TEXT);

    /*Parse levels - 8 levels max on screen*/
    for(i = 0; i < levelsFolder->nbrElements && i < 8; ++i)
    {
        /*Arrow*/
        lvlArrow = createButton(-250, 200 - 50 * i, 52, 52, i + 1);
        lvlArrow->idleTexture = getTexture("empty.png");
        lvlArrow->selectedTexture = getTexture("rightArrow.png");

        /*Select if first*/
        if(i == 0)
        {
            lvlArrow->state = SELECTED;
            gameObj.currentlySelectedBtn = lvlArrow;
        }
        
        /*Level name*/
        lvlName = createText(levelsFolder->elements[i]->d_name, -190, 180 - 50 * i, gameObj.defaultFont);
        lvlName->align = ALIGN_LEFT;

        /*Interactions if not the first*/
        if(lastArrow != NULL)
        {
            lastArrow->bottomBtn = lvlArrow;
            lvlArrow->topBtn = lastArrow;
        }

        /*Add to print*/
        addToPrint(lvlArrow, BUTTON);
        addToPrint(lvlName, TEXT);

        lastArrow = lvlArrow;
        lvlArrow = NULL;
    }

    closeFolder(levelsFolder);

    /*empty level button*/
    lvlArrow = createButton(-250, 200 - 50 * i, 52, 52, -1);            /*-1 implies no level*/
    lvlArrow->idleTexture = getTexture("empty.png");
    lvlArrow->selectedTexture = getTexture("rightArrow.png");

    /*Empty level text*/
    lvlName = createText("Aucun", -190, 180 - 50 * i, gameObj.defaultFont);
    lvlName->align = ALIGN_LEFT;

    /*Add to print*/
    addToPrint(lvlArrow, BUTTON);
    addToPrint(lvlName, TEXT);

    /*Interactions*/
    lastArrow->bottomBtn = lvlArrow;
    lvlArrow->topBtn = lastArrow;

    /*Back btn*/
    backBtn = createButton(0, -312, 342, 52, 'b');           /*'b' for back*/
    backBtn->idleTexture = getTexture("backBtn_idle.png");
    backBtn->selectedTexture = getTexture("backBtn_selected.png");
    addToPrint(backBtn, BUTTON);

    /*Interactions*/
    lvlArrow->bottomBtn = backBtn;
    backBtn->topBtn = lvlArrow;

    gameObj.printContent = LEVELSELECTION;
}


/*Create the game board*/
void createGameBoard()
{
    Picture * background, * life;
    Ball * ball;
    Text * txt;
    int i;
    FolderContent * levelFolder;

    /*Background Image*/
    background = createPicture(0, 0, "background.png");
    addToPrint(background, PICTURE);

    /*For each player*/
    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        /*Add a life counter*/
        life = createPicture(gameObj.game.bb.width / 2 - 25, gameObj.game.bb.height + 25, "life.png");
        life->height = 40;
        life->width = 40;
        life->BBox = i;
        gameObj.game.players[i]->lifePicture = life;

        if(gameObj.game.bb.squared)
        {
            life->x = -gameObj.game.bb.width / 2 + 35;
            life->y = gameObj.game.bb.height - 105;
        }
        
        addToPrint(life, PICTURE);

        txt = createText(itoa(gameObj.game.players[i]->life), gameObj.game.bb.width / 2 - 75, gameObj.game.bb.height + 10, gameObj.defaultFont);
        txt->BBox = i;
        gameObj.game.players[i]->lifeText = txt;

        if(gameObj.game.bb.squared)
        {
            txt->x = -gameObj.game.bb.width / 2 + 35;
            txt->y = gameObj.game.bb.height - 75;
        }

        addToPrint(txt, TEXT);

        /*Print it's plateforme*/
        addToPrint(gameObj.game.players[i]->plateforme, PLATEFORME);
        gameObj.game.players[i]->plateforme->elementID = gameObj.nbrToPrint - 1;

        /*Give him a ball*/
        ball = createGluedBall(0, 0, i);
        ball->elementID = gameObj.nbrToPrint;
        addToPrint(ball, BALL);

    }

    gameObj.printContent = INGAME;

    /*Is this an empty level ?*/
    if(gameObj.game.levelID == -1)
        return;
    
    /*Get level file path*/
    levelFolder = parseFolder("./levels");

    if(gameObj.game.levelID > levelFolder->nbrElements)
        return; /*Bad folder ID*/

    /**Create the bricks for the level*/
    printf("LEVEL > %s", levelFolder->elements[gameObj.game.levelID - 1]->d_name);
    createBricks(levelFolder->elements[gameObj.game.levelID - 1]->d_name);

    closeFolder(levelFolder);
}

void createPauseMenu()
{
    Picture * pauseBackground;
    Button * playBtn, * quitBtn;

    pauseBackground = createPicture(0, 0, "background.1.png");

    playBtn = createButton(0, 100, 342, 52, 'p');                /*'p' for play*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");
    playBtn->state = SELECTED;

    quitBtn = createButton(0, -78, 342, 52, 'q');                /*'q' for quit*/
    quitBtn->idleTexture = getTexture("quitBtn_idle.png");
    quitBtn->selectedTexture = getTexture("quitBtn_selected.png");

    playBtn->bottomBtn = quitBtn;
    quitBtn->topBtn = playBtn;

    gameObj.currentlySelectedBtn = playBtn;

    gameObj.game.pauseMenu.background = addToPrint(pauseBackground, PICTURE);
    gameObj.game.pauseMenu.playBtn = addToPrint(playBtn, BUTTON);
    gameObj.game.pauseMenu.quitBtn = addToPrint(quitBtn, BUTTON);

    createFloatAnimation(&pauseBackground->opacity, 0.0, .8, 500, 0, QUAD, NULL);
    createFloatAnimation(&playBtn->opacity, 0.0, 1.0, 500, 0, QUAD, NULL);
    createFloatAnimation(&quitBtn->opacity, 0.0, 1.0, 500, 0, QUAD, NULL);
}

void createStartGameAnimation()
{
    Picture * playerCtrl;
    Text * countDown;
    char path[256];
    int i, nbr = 0;

    gameObj.game.startAnimation.playersCtrl = allocate(sizeof(Picture *) * gameObj.game.humans);

    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        if(gameObj.game.players[i]->type == AI)
            continue; /*Ignore AI*/

        strcpy(path, "playerControls");
        strcat(path, itoa(nbr+1));
        strcat(path, ".png");

        playerCtrl = createPicture(0, 250, path);
        playerCtrl->BBox = i;

        gameObj.game.startAnimation.playersCtrl[nbr] = addToPrint(playerCtrl, PICTURE);
        
        ++nbr;
    }

    countDown = createText("3", 0, 0, gameObj.defaultFont);
    gameObj.game.startAnimation.countDown = addToPrint(countDown, TEXT);

    createFloatAnimation(&countDown->opacity, 1.0, 0.0, 200, 800, QUAD, &startAnimationTwo);
    
    gameObj.game.starting = true;
}