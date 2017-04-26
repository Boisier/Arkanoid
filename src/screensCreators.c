# include "../includes/game.h"

/*Create Mainmenu elements*/
void createMainMenu()
{
    Picture * background;
    Button * playBtn, * rulesBtn, * themeBtn;

    background = createPicture(percent(-50, 'w'), percent(50, 'h'), "background.png");

    playBtn = createButton(-171, -78, 342, 52, 's');                /*'s' for start*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");
    playBtn->state = SELECTED;

    rulesBtn = createButton(-171, -195, 342, 52, 'r');              /*'r' for rules*/
    rulesBtn->idleTexture = getTexture("rulesBtn_idle.png");
    rulesBtn->selectedTexture = getTexture("rulesBtn_selected.png");
    rulesBtn->state = IDLE;

    themeBtn = createButton(-171, -311, 342, 52, 't');              /*'t' for themes*/
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
    background = createPicture(percent(-50, 'w'), percent(50, 'h'), "background.png");
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
            tempBtn = createButton(-171, 300 - 111*(nbrThemes - 1), 342, 52, nbrThemes);
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

    backBtn = createButton(-171, 300 - 111*(nbrThemes + 1), 342, 52, 'b');              /*'b' for back*/
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
    Button * onePlayerBtn, * twoPlayerBtn, * backBtn;

    background = createPicture(percent(-50, 'w'), percent(50, 'h'), "background.png");

    onePlayerBtn = createButton(-171, -78, 342, 52, 1);                /*'s' for start*/
    onePlayerBtn->idleTexture = getTexture("onePlayerBtn_idle.png");
    onePlayerBtn->selectedTexture = getTexture("onePlayerBtn_selected.png");
    onePlayerBtn->state = SELECTED;

    twoPlayerBtn = createButton(-171, -195, 342, 52, 2);              /*'r' for rules*/
    twoPlayerBtn->idleTexture = getTexture("twoPlayerBtn_idle.png");
    twoPlayerBtn->selectedTexture = getTexture("twoPlayerBtn_selected.png");
    twoPlayerBtn->state = IDLE;

    backBtn = createButton(-171, -312, 342, 52, 'b');              /*'b' for back*/
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


/*Create the game board*/
void createGameBoard()
{
    Picture * background;
    Ball * ball;
    int i;

    /*Background Image*/
    background = createPicture(percent(-50, 'w'), percent(50, 'h'), "background.png");
    addToPrint(background, PICTURE);

    /*For each player*/
    for(i = 0; i < gameObj.game.nbrPlayers; ++i)
    {
        /*Print it's plateforme*/
        addToPrint(gameObj.game.players[i]->plateforme, PLATEFORME);

        /*Give him a ball*/
        ball = createGluedBall(0, 0, i);
        addToPrint(ball, BALL);
    }

    /**Create the bricks for the level*/
    createBricks("starter.level");

    gameObj.printContent = INGAME;
}
