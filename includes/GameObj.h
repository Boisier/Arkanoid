#pragma once

typedef struct _GameObj
{
    /** Window Properties **/
    int wWidth;
    int wHeight;

    /** Theme **/
    char theme[256];

    /** Default values **/
    struct
    {
        struct
        {
            float size;
            float height;
            float maxSpeed;
            float acceleration;
            float level;
            float narrowSize;
            float wideSize;
        } plateforme;

        struct
        {
            int startLevel;
            int height;
        } brick;

        struct
        {
            float size;
            float bigSize;
            float smallSize;
            float minSpeed;
            float maxSpeed;
            float maxAngle;
            float gluedMaxDuration;
        } ball;

        struct
        {
            int size;
            float speed;
            int duration;
        } bonus;

        struct
        {
            int height;
        } wall;

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
    struct
    {
        enum gameType type;

        bool play;
        bool starting;

        Player ** players;
        int nbrPlayers;

        int humans;
        int computers;

        int levelID;

        /*Bounding box informations*/
        struct
        {
            float angle;
            float startAngle;
            float radius;
            
            bool squared;

            float height;
            float width;

            float platMinPos;
            float platMaxPos;

            int gridH;
            int gridW;
        } bb; /* bb : short for Bounding Box*/

        bool guidelines;

        bool pause;

        struct
        {
            PrintElement * background;
            PrintElement * playBtn;
            PrintElement * quitBtn;
        } pauseMenu;

        struct
        {
            PrintElement ** playersCtrl;
            PrintElement * countDown;
        } startAnimation;

    } game;

    /* Player selection holder*/
    struct 
    {
        NumberBox * humans;
        NumberBox * computers;
    } playerSelection;

    /* Font */
    Font * defaultFont;
    Vector3D defautlTextColor;
    Vector3D selectedTextColor;

} GameObj;

GameObj gameObj;
