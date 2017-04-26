#pragma once

/** Collision is used to store data about a collision event **/
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

/** Holds a vector used for direction **/
typedef struct _vector2D
{
	float x;
	float y;
} Vector2D;

/** Rectangles coordinates for easy manipulation **/
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

/** Spheres coordinates for easy manipulation **/
typedef struct _basicSphere
{
	float x;
	float y;
	float radius;
} BaseSphere;





/********************/
/** PRINT ELEMENTS **/
/********************/

typedef struct _platforme
{
	int BBox;

	float x;
	float y;
	int size;
	int dirFactor;

	float speed;

	bool sticky;

	GLuint texture;

	enum bonusType bonus;
	int bonusEnd;
} Plateforme;

typedef struct _brick
{
	int BBox;

	Vector2D topLeft;
	Vector2D topRight;
	Vector2D bottomRight;
	Vector2D bottomLeft;

	enum brickType type;

	int strength;

	GLuint texture;
	GLuint texture1;
	GLuint texture2;
} Brick;

typedef struct _ball
{
	int BBox;

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
	int BBox;
	
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

	Plateforme * plateforme;
} Player;
