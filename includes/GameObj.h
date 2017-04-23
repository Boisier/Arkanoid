#pragma once

typedef enum _GameState
{
    EMPTY,
    MAINMENU,
    PLAYERSELECTION,
    STARTGAME,
    INGAME,
    EXITING
} GameState;


typedef struct _keyPressed
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool space;
    bool enter;
    bool a;
    bool z;
    bool esc;
} KeyPressed;

typedef struct _GameObj
{
    /** Window Properties **/
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    /** Theme **/
    char theme[256];

    /** Default values **/
    struct
    {
        struct
        {
            int size;
            int height;
            float maxSpeed;
            float acceleration;
        } plateforme;

        struct
        {
            int width;
            int height;
        } brick;

        struct
        {
            int size;
            float speed;
        } ball;

        struct
        {
            int size;
            float speed;
        } bonus;

        int lifeNbr;

    } defVal;

    /** Loop storage **/
    GameState gameState;

	/** Textures **/
	GLuint * textures;
	char ** texturesPath;
	int nbrTextures;

    /** Printing**/
    PrintElement * toPrint;
    int nbrToPrint;
    GameState printContent;

    /** key watched **/
    KeyPressed keys;

    /** Interactions holders **/
    Button * currentlySelectedBtn;

    /** The Game **/
    int nbrHumanPlayers;
    Player ** players;

} GameObj;

GameObj gameObj;