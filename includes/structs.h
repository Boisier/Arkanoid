#pragma once

/** Holds a vector used for direction **/
typedef struct _vector2D
{
	float x;
	float y;
} Vector2D;

/** Polygon coordinates for easy manipulation **/
typedef struct _polygon
{
	int BBox;
	Vector2D * points;
	int nbrPoints;
} Polygon;

/** Circle coordinates for easy manipulation **/
typedef struct _circle
{
	int BBox;
	float radius;
	Vector2D position;
} Circle;

/** Collision is used to store data about a collision event **/
typedef struct _collisionData
{
	enum CollisionSide side;
	Vector2D point;

	float delta;

} Collision;





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

	GLuint texture;

	bool sticky;
	int stickyEnd;

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
	int gluedAt;

	GLuint texture;

	Vector2D direction;
	float speed;
	
	enum bonusType bonus;
	int bonusEnd;
} Ball;

typedef struct _bonus
{
	int BBox;
	
	float x;
	float y;
	
	enum bonusType type;

	GLuint texture;

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
	bool toDelete;

} PrintElement;

typedef struct _player
{
	enum PlayerType type;

	int life;
	int score; /* ~ */

	Plateforme * plateforme;
} Player;
