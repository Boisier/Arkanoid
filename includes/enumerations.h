#pragma once

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

enum PlayerPos
{
	TOP,
	BOTTOM
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
	BASIC,
	INVICIBLE,
	TRANSPARENT,
	STRONG,
	VERYSTRONG
};

typedef enum _GameState
{
    EMPTY,
    MAINMENU,
    PLAYERSELECTION,
    STARTGAME,
    INGAME,
    EXITING
} GameState;
