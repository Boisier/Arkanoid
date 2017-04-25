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
	TOP_RIGHT_CORNER,
	RIGHT_SIDE,
	BOTTOM_RIGHT_CORNER,
	BOTTOM_SIDE,
	BOTTOM_LEFT_CORNER,
	LEFT_SIDE,
	LEFT_TOP_CORNER
};

typedef struct _collisionData
{
	enum CollisionSide side;
	float x;
	float y;

	float deltaTop;
	float deltaLeft;
	float deltaRight;
	float deltaBottom;

} Collision;




typedef struct _vector2D
{
	float x;
	float y;
} Vector2D;

typedef struct _basicRectangle
{
	float topLeftX;
	float topLeftY;

	float topRightX;
	float topRightY;

	float bottomRightX;
	float bottomRightY;

	float bottomLeftX;
	float bottomLeftY;
} BaseRect;

typedef struct _basicSphere
{
	float x;
	float y;
	float radius;
} BaseSphere;





typedef struct _platforme
{
	float x;
	float y;
	int size;
	int dirFactor;

	enum PlayerPos pos;
	float speed;

	bool sticky;

	GLuint texture;

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

typedef struct _ball
{
	float x;
	float y;
	float size;

	int playerID;

	bool glued;
	Plateforme * gluedPlat;
	float glueOffsetX;

	GLuint texture;

	Vector2D direction;
	float speed;
	
	enum bonusType bonusType;
	int bonusEnd;
} Ball;

typedef struct _bonus
{
	float x;
	float y;
	
	enum bonusType type;

	Vector2D direction;

} Bonus;

typedef struct _button Button;

struct _button {
	int x;
	int y;
	int width;
	int height;

	GLuint idleTexture;
	GLuint selectedTexture;

	enum elState state;
	char callback;

	Button * leftBtn;
	Button * topBtn;
	Button * rightBtn;
	Button * bottomBtn;
};

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
	bool display;

} PrintElement;






typedef struct _player
{
	enum PlayerType type;

	int life;
	int score; /* ~ */

	enum PlayerPos playerPos;

	Plateforme * plateforme;
} Player;