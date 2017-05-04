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

    rulesBtn = createButton(0, -195, 342, 52, 'r');              /*'r' for rules*/
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
    Picture * background;
    Button * tempBtn, * lastBtn = NULL, * backBtn;
    char currentTheme[256], themePath[256];
    int nbrThemes = 0;

    DIR * themesFolder;
    struct dirent * tFold;

    strcpy(currentTheme, gameObj.theme);

    /** First, the background*/
    background = createPicture(0, 0, "background.png");
    addToPrint(background, PICTURE);

    /*List available themes*/
    themesFolder = opendir("./themes");
    
    do
    {
        tFold = readdir(themesFolder);

        if(tFold != NULL)
        {
            /*Ignore dot folders*/
            if(tFold->d_name[0] == '.')
                continue;

            nbrThemes++;

            /*Build path*/
            strcpy(themePath, "./themes/");
            strcat(themePath, tFold->d_name);
            strcat(themePath, "/");

            /*Set as current theme*/
            strcpy(gameObj.theme, themePath);

            /*Create the button*/
            tempBtn = createButton(0, 300 - 111*(nbrThemes - 1), 342, 52, nbrThemes);
            tempBtn->idleTexture = getTexture("themeSelectorBtn_idle.png");
            tempBtn->selectedTexture = getTexture("themeSelectorBtn_selected.png");
            
            if(nbrThemes == 1)
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

            /*Limit to 5 themes*/
            if(nbrThemes == 5)
                break;
        }
    } while(tFold != NULL);

    (void) closedir(themesFolder);

    backBtn = createButton(0, 300 - 111*(nbrThemes + 1), 342, 52, 'b');              /*'b' for back*/
    backBtn->idleTexture = getTexture("backBtn_idle.png");
    backBtn->selectedTexture = getTexture("backBtn_selected.png");

    backBtn->topBtn = lastBtn;
    lastBtn->bottomBtn = backBtn;

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


/*Create the game board*/
void createGameBoard()
{
    Picture * background, * life;
    Ball * ball;
    Text * txt;
    int i;

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
        addToPrint(life, PICTURE);

        txt = createText(itoa(gameObj.game.players[i]->life), gameObj.game.bb.width / 2 - 75, gameObj.game.bb.height + 10, gameObj.defaultFont);
        txt->BBox = i;
        gameObj.game.players[i]->lifeText = txt;
        addToPrint(txt, TEXT);

        /*Print it's plateforme*/
        addToPrint(gameObj.game.players[i]->plateforme, PLATEFORME);

        /*Give him a ball*/
        ball = createGluedBall(0, 0, i);
        addToPrint(ball, BALL);
    }

    /**Create the bricks for the level*/
    createBricks("full.level");

    gameObj.printContent = INGAME;
}
