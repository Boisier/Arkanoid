# include "../includes/game.h"

/*Create Mainmenu elements*/
void createMainMenu()
{
    Picture * background;
    Button * playBtn, * rulesBtn;

    background = createPicture(0, 0, "background.png");

    playBtn = createButton(429, 478, 342, 52, 's');                /*'s' for start*/
    playBtn->idleTexture = getTexture("playBtn_idle.png");
    playBtn->selectedTexture = getTexture("playBtn_selected.png");
    playBtn->state = SELECTED;

    rulesBtn = createButton(429, 595, 342, 52, 'r');              /*'r' for rules*/
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

/*Create Mainmenu elements*/
void createPlayerSelection()
{
    Picture * background;
    Button * onePlayerBtn, * twoPlayerBtn, * backBtn;

    background = createPicture(0, 0, "background.png");

    onePlayerBtn = createButton(429, 478, 342, 52, 1);                /*'s' for start*/
    onePlayerBtn->idleTexture = getTexture("onePlayerBtn_idle.png");
    onePlayerBtn->selectedTexture = getTexture("onePlayerBtn_selected.png");
    onePlayerBtn->state = SELECTED;

    twoPlayerBtn = createButton(429, 595, 342, 52, 2);              /*'r' for rules*/
    twoPlayerBtn->idleTexture = getTexture("twoPlayerBtn_idle.png");
    twoPlayerBtn->selectedTexture = getTexture("twoPlayerBtn_selected.png");
    twoPlayerBtn->state = IDLE;

    backBtn = createButton(429, 712, 342, 52, 'b');              /*'b' for back*/
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

      background = createPicture(0, 0, "background.png");

      addToPrint(background, PICTURE);

      for(i = 0; i < gameObj.nbrPlayers; ++i)
      {
          addToPrint(gameObj.players[i]->plateforme, PLATEFORME);

          ball = createBall(0, 0, i);
          ball->gluedPlat = gameObj.players[i]->plateforme;
          ball->glueOffsetX = gameObj.players[i]->plateforme->size / 2;
          addToPrint(ball, BALL);
      }

      gameObj.printContent = INGAME;
}