#pragma once

typedef enum _GameState
{
    EMPTY,
    MAINMENU,
	THEMESELECTION,
    PLAYERSELECTION,
    STARTGAME,
    INGAME,
    EXITING
} GameState;



enum elType
{
	PLATEFORME,
	BRICK,
	BALL,
	BONUS,
	BUTTON,
	PICTURE
};

enum elState
{
	IDLE,
	SELECTED,
	FOCUS,
	BLUR
};

enum bonusType
{
	NONE,
	WIDE,
	NARROW,
	STICKY,
	BIG,
	SMALL,
	REVERSED
};

enum PlayerType
{
	HUMAN,
	AI
};

enum CollisionSide
{
	NO_COLLISION,
	UNKNOWN,
	TOP_SIDE,
	RIGHT_SIDE,
	BOTTOM_SIDE,
	LEFT_SIDE
};

enum brickType
{
	BASIC,			/*1*/
	STRONG,			/*2*/
	VERYSTRONG		/*3*/
	INVICIBLE,		/*4*/
	TRANSPARENT,	/*5*/
};

enum gameType
{
	LOCALE,
	ONLINE
};
