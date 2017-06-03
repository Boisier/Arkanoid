#include "../includes/game.h"

/** All awaited properties **/
enum Properties
{
	PP_WINDOW_WIDTH,
	PP_WINDOW_HEIGHT,
	PP_DEFAULT_THEME,
	
	PP_PLATEFORME_SIZE,
	PP_PLATEFORME_HEIGHT,
	PP_PLATEFORME_MAXSPEED,
	PP_PLATEFORME_ACCELERATION,
	PP_PLATEFORME_LEVEL,
	PP_PLATEFORME_NARROWSIZE,
	PP_PLATEFORME_WIDESIZE,
	
	PP_BRICK_STARTLEVEL,
	PP_BRICK_HEIGHT,
	
	PP_BALL_SIZE,
	PP_BALL_BIGSIZE,
	PP_BALL_SMALLSIZE,
	PP_BALL_MINSPEED,
	PP_BALL_MAXSPEED,
	PP_BALL_MAXANGLE,
	PP_BALL_GLUEDMAXDURATION,

	PP_BONUS_SIZE,
	PP_BONUS_SPEED,
	PP_BONUS_DURATION,

	PP_LIFENUMBER,

	PP_GUIDELINES,

	PP_UNKNOWN
};

typedef struct _pp_symTable
{
	enum Properties pp;
	char * key;
} PP_SymTable;


/** Parse setting.json and insert values in the gameObj **/
static bool loadDynProps();

/** Set other values in the gameObj **/
static void loadStaticProps();

/** Return correct property for the given string*/
static enum Properties propKey(char * prop);





/** Init game properties and dependancies with default values **/
void initGame()
{   
    /*Init the gameObj*/
    if(!initGameObj())
        throwCriticalError();

    /*Open the window*/
    if(!initSDL())
        throwCriticalError();

    loadSDLDependants();
}





/** Inite the game obj with default values*/
bool initGameObj()
{
	if(!loadDynProps())
		return false;

	loadStaticProps();

	return true;
}




/** Return correct property for the given string*/
enum Properties propKey(char * prop)
{
	PP_SymTable lookupTable[] = {
		{PP_WINDOW_WIDTH, "window_width"},
		{PP_WINDOW_HEIGHT, "window_height"},
		{PP_DEFAULT_THEME, "default_theme"},
		{PP_PLATEFORME_SIZE, "plateforme_size"},
		{PP_PLATEFORME_HEIGHT, "plateforme_height"},
		{PP_PLATEFORME_MAXSPEED, "plateforme_maxspeed"},
		{PP_PLATEFORME_ACCELERATION, "plateforme_acceleration"},
		{PP_PLATEFORME_LEVEL, "plateforme_level"},
		{PP_PLATEFORME_NARROWSIZE, "plateforme_narrowsize"},
		{PP_PLATEFORME_WIDESIZE, "plateforme_widesize"},
		{PP_BRICK_STARTLEVEL, "brick_startLevel"},
		{PP_BRICK_HEIGHT, "brick_height"},
		{PP_BALL_SIZE, "ball_size"},
		{PP_BALL_BIGSIZE, "ball_bigsize"},
		{PP_BALL_SMALLSIZE, "ball_smallsize"},
		{PP_BALL_MINSPEED, "ball_minspeed"},
		{PP_BALL_MAXSPEED, "ball_maxspeed"},
		{PP_BALL_MAXANGLE, "ball_maxangle"},
		{PP_BALL_GLUEDMAXDURATION, "ball_gluedmaxduration"},
		{PP_BONUS_SIZE, "bonus_size"},
		{PP_BONUS_SPEED, "bonus_speed"},
		{PP_BONUS_DURATION, "bonus_duration"},
		{PP_LIFENUMBER, "life_number"},
		{PP_GUIDELINES, "guidelines"}
	};

	int i;
	int nbrKeys = sizeof(lookupTable) / sizeof(PP_SymTable);

	for(i = 0; i < nbrKeys; ++i)
	{
		if(strcmp(prop, lookupTable[i].key) == 0)
			return lookupTable[i].pp;
	}

	return PP_UNKNOWN;
}




/** Parse setting.json and insert values in the gameObj **/
bool loadDynProps()
{
  	char * props = 0, key[50], value[50];
	long length;
	FILE * file = fopen ("settings.json", "rb");
	jsmn_parser p;
	jsmntok_t t[128]; /* Suppose no more than 128 properties */
	int nbrProps, i;
	float fValue = 0;
	enum Properties pp;

	if(file)
	{
		/*Get file size*/
		fseek (file, 0, SEEK_END);
		length = ftell (file);
		rewind(file);
		
		/*Allocate memory and read file*/
		props = allocate(sizeof(char) * (int)(length + 1));
		if(fread (props, 1, length, file) == 0)
			return false;

		props[length] = '\0';
		
		fclose (file);
	}

	if (!props)
		return false; /*error while reading file*/
	
	/*Init jsmn*/
	jsmn_init(&p);

	/*Parse JSON*/
	nbrProps = jsmn_parse(&p, props, strlen(props), t, sizeof(t)/sizeof(t[0]));
	
	/*printf("> parsing %d properties\n", (nbrProps - 1) * .5);*/

	if(nbrProps < 0)
		return false; /*Error in json*/

	for(i = 1; i < nbrProps; i += 2)
	{
		/*Get key and value as strings*/
		strncpy(key, props + t[i].start, t[i].end - t[i].start);
		key[t[i].end - t[i].start] = '\0';

		strncpy(value, props + t[i+1].start, t[i+1].end - t[i+1].start);
		value[t[i+1].end - t[i+1].start] = '\0';

		/*printf("> %s %s <\n", key, value);*/
		
		pp = propKey(key);

		/*In most case, we need the value as a number*/
		if(pp != PP_DEFAULT_THEME)
			fValue = atof(value);
		
		/*Assign value to correct property*/
		switch(pp)
		{
			case PP_WINDOW_WIDTH:            gameObj.wWidth                         = fValue; break;
			case PP_WINDOW_HEIGHT:           gameObj.wHeight                        = fValue; break;
			case PP_PLATEFORME_SIZE:         gameObj.defVal.plateforme.size         = fValue; break;
			case PP_PLATEFORME_HEIGHT:       gameObj.defVal.plateforme.height       = fValue; break;
			case PP_PLATEFORME_MAXSPEED:     gameObj.defVal.plateforme.maxSpeed     = fValue; break;
			case PP_PLATEFORME_ACCELERATION: gameObj.defVal.plateforme.acceleration = fValue; break;
			case PP_PLATEFORME_LEVEL:        gameObj.defVal.plateforme.level        = fValue; break;
			case PP_PLATEFORME_NARROWSIZE:   gameObj.defVal.plateforme.narrowSize   = fValue; break;
			case PP_PLATEFORME_WIDESIZE:     gameObj.defVal.plateforme.wideSize     = fValue; break;
			case PP_BRICK_STARTLEVEL:        gameObj.defVal.brick.startLevel        = fValue; break;
			case PP_BRICK_HEIGHT:            gameObj.defVal.brick.height            = fValue; break;
			case PP_BALL_SIZE:               gameObj.defVal.ball.size               = fValue; break;
			case PP_BALL_BIGSIZE:            gameObj.defVal.ball.bigSize            = fValue; break;
			case PP_BALL_SMALLSIZE:          gameObj.defVal.ball.smallSize          = fValue; break;
			case PP_BALL_MINSPEED:           gameObj.defVal.ball.minSpeed           = fValue; break;
			case PP_BALL_MAXSPEED:           gameObj.defVal.ball.maxSpeed           = fValue; break;
			case PP_BALL_MAXANGLE:           gameObj.defVal.ball.maxAngle           = fValue; break;
			case PP_BALL_GLUEDMAXDURATION:   gameObj.defVal.ball.gluedMaxDuration   = fValue; break;
			case PP_BONUS_SIZE:              gameObj.defVal.bonus.size              = fValue; break;
			case PP_BONUS_SPEED:             gameObj.defVal.bonus.speed             = fValue; break;
			case PP_BONUS_DURATION:          gameObj.defVal.bonus.duration          = fValue; break;
			case PP_LIFENUMBER:              gameObj.defVal.lifeNbr                 = fValue; break;
			case PP_GUIDELINES:              gameObj.game.guidelines                = (fValue == 1 ? true : false); break;
			case PP_DEFAULT_THEME: 
				strcpy(gameObj.theme, "themes/");
				strcat(gameObj.theme, value);
				strcat(gameObj.theme, "/");
			break;
			default: break;
		}
	}

	free(props);

	return true;
}





/** Set other values in the gameObj **/
void loadStaticProps()
{
    gameObj.defVal.wall.height = gameObj.defVal.plateforme.level;

    gameObj.gameState = MAINMENU;

	/*Texture and toPrint array*/
    gameObj.textures = NULL;
    gameObj.nbrTextures = 0;

    gameObj.toPrint = NULL;
    gameObj.nbrToPrint = 0;
    gameObj.printContent = EMPTY;

    /*Set state of keys*/
    gameObj.keys.up = false;
    gameObj.keys.down = false;
    gameObj.keys.left = false;
    gameObj.keys.right = false;
    gameObj.keys.enter = false;
    gameObj.keys.esc = false;
    gameObj.keys.a = false;
    gameObj.keys.z = false;
    gameObj.keys.e = false;
    gameObj.keys.b = false;
    gameObj.keys.n = false;
    gameObj.keys.v = false;

    gameObj.currentlySelectedBtn = NULL;

    gameObj.game.play = false;
    gameObj.game.starting = false;

    gameObj.game.nbrPlayers = 0;
    gameObj.game.players = NULL;

    gameObj.defaultTextColor = vec3(1, 1, 1);
    gameObj.selectedTextColor = vec3(0, 61.0/255.0, 81.0/255.0);

    gameObj.game.play = false;
    gameObj.game.pause = false;
    gameObj.game.pauseMenu.background = NULL;
    gameObj.game.pauseMenu.playBtn = NULL;
    gameObj.game.pauseMenu.quitBtn = NULL;

    gameObj.game.startAnimation.playersCtrl = NULL;
    gameObj.game.startAnimation.countDown = NULL;

    srand(time(NULL));   /*Init rand*/
}



/** Init the SDL **/
bool initSDL()
{
    if(-1 == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
    {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return false;
    }
	
    setVideoMode(gameObj.wWidth, gameObj.wHeight);

    SDL_WM_SetCaption("Arkanopong", NULL);

    /**Init AUdio**/
    /*audio_init();*/

	return true;
}



/** Load elements depending on the SDL library **/
void loadSDLDependants()
{
    gameObj.defaultFont = loadFont("BebasNeue.otf", 40);
}
