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

typedef struct _platforme
{
	float x;
	float y;
	int size;

	float speed;

	enum bonusType bonus;
	int bonusEnd;
} Plateforme;

enum brickType
{
	BASIC,
	INVICIBLE,
	TRANSPARENT,
	STRONG,
	VERYSTRONG
};

typedef struct _brick
{
	int x;
	int y;

	enum brickType type;
	int strength;
} Brick;

typedef struct _vector2D
{
	float x;
	float y;
} Vector2D;

typedef struct _ball
{
	float x;
	float y;
	float size;

	bool glued;
	Plateforme * gluedPlat;
	float glueOffsetX;

	Vector2D direction;
	float speed;
	
	enum bonusType bonus;
	int bonusEnd;
} Ball;

typedef struct _bonus
{
	float x;
	float y;
	
	enum bonusType type;

	Vector2D direction;

} Bonus;

typedef struct _button
{
	int x;
	int y;

	int width;
	int height;

	GLuint idleTexture;
	GLuint selectedTexture;

	int callBack;
} Button;

typedef struct _picture
{
	int x;
	int y;
	int width;
	int height;

	GLuint texture;
} Picture;








typedef struct _printElement
{
	union 
	{
		Plateforme * plateforme;
		Brick * brick;
		Ball * ball;
		Bonus * bonus;
		Button * btn;
		Picture * pict;
	} element;
	enum elType type;
	enum elState state;

} PrintElement;
