#ifndef _VARS_H
#define _VARS_H

// Misc constants
const TRUE = 1, FALSE = 0;

// Four player's constants according to their colors

const   PLR_BLUE   = 0, /* Blue is in upper left corner */
		PLR_GREEN  = 1, /* Green is in upper right corner */
		PLR_RED    = 2, /* Red is in lower right corner */
		PLR_YELLOW = 3; /* Yellow is in lower left corner */

// Type of each player's constants

const TYPE_NONE = 0,
	  TYPE_HUMAN = 1,
	  TYPE_COMPUTER = 2;

// Each piece's status constants

const   ST_ATHOME = 0,
		ST_ONBOARD = 1,
		ST_FINISHED = 2;

/* Board object and player info structures */

struct point
{
	int x;
	int y;
};

struct Piece
{
	int status;
	point location;
	int canmove;
	point homelocation;
	point moveto;
};

struct PlayerInfo
{
	int type;
	Piece piece[4];
	char name[10];
};

struct Board
{
	int player;
	int pieces[4];
	int top;
};

/* board's graphical info */
struct boardgraf
{
	int left;
	int top;
	int blocksize;
};

/* Button updown store info */
struct button
{
	unsigned int coords[2][2];
	int* up;   // Image when button is not pressed
	int* down; // Image when button is pressed
};
#endif
