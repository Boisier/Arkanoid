#pragma once

/** The different stats of the game*/
typedef enum _GameState
{
    EMPTY,
    MAINMENU,			
	THEMESELECTION,
    PLAYERSELECTION,
	LEVELSELECTION,
    STARTGAME,
    INGAME,
	ENDGAME,
    EXITING
} GameState;


/** The differents elements **/
enum elType
{
	PLATEFORME,
	BRICK,
	WALL,
	BALL,
	BONUS,
	BUTTON,
	PICTURE,
	NUMBERBOX,
	TEXT,
	ANIMATION
};


/** The state an element can take **/
enum elState
{
	IDLE,
	SELECTED,
	FOCUS,
	BLUR
};


/** The different bonuses **/
enum bonusType
{
	NONE,
	WIDE_PLATEFORME,
	NARROW_PLATEFORME,
	STICKY_PLATEFORME,
	BIG_BALL,
	SMALL_BALL,
	ADD_LIFE,
	REMOVE_LIFE
};


/** The two possibles player types **/
enum PlayerType
{
	HUMAN,
	AI
};


/** Collision places handled **/
enum CollisionSide
{
	NO_COLLISION,
	UNKNOWN,			/*There's a collision but we don't know where*/
	CORNER,
	TOP_LEFT_CORNER,
	TOP_SIDE,
	TOP_RIGHT_CORNER,
	RIGHT_SIDE,
	BOTTOM_RIGHT_CORNER,
	BOTTOM_SIDE,
	BOTTOM_LEFT_CORNER,
	LEFT_SIDE
};


/** The different types of brick **/
enum brickType
{
	BASIC,			/*1*/
	STRONG,			/*2*/
	VERYSTRONG,		/*3*/
	INVINCIBLE,		/*4*/
	TRANSPARENT   	/*5*/
};


/** Type of game **/
enum gameType
{
	LOCALE,
	ONLINE
};


/** Alignement of a text element **/
enum textAlign
{
	ALIGN_CENTER,
	ALIGN_LEFT,
	ALIGN_RIGHT
};

/** Supported animations easings **/
enum animationType
{
	LINEAR,
	QUAD,
	CUBIC,
	QUARTIC,
	QUINTIC
};
