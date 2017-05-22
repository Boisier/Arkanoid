#pragma once

/** The game object (GameObj) is a global structure holding informations.
    These are informations used by multiple functions throughout the application.
    Using one big global structure prevent having to pass it to each function, lightening up the code **/
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

    /** Current state of the application **/
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

    /** Menu interactions holder **/
    Button * currentlySelectedBtn;

    /** The Game **/
    struct
    {
        /** ONLINE or LOCAL game ? *TODO* */
        enum gameType type;

        bool play;
        bool starting;
        bool pause;

        /*Players*/
        Player ** players;
        int nbrPlayers;

        int humans;
        int computers;

        /*The level*/
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
    Vector3D defaultTextColor;
    Vector3D selectedTextColor;

} GameObj;

GameObj gameObj;
