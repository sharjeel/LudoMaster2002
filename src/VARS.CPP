// Four player's constants according to their colors

const PLR_BLUE   = 0, /* Blue is in upper left corner */
		PLR_RED    = 1, /* Red is in lower right corner */
		PLR_GREEN  = 2, /* Green is in upper right corner */
		PLR_YELLOW = 3; /* Yellow is in lower left corner */

// Type of each player's constants

const TYPE_NONE = 0,
		TYPE_HUMAN = 1,
		TYPE_COMPUTER = 2;

// Each piece's status constants

const ST_HOME = 0,
		ST_BOARD = 1,
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
};

struct PlayerInfo
{
	int type;
	Piece piece[4];
	char name[10];
};

struct Board
{
	int occupied;
	int player;
	stack pieces;
};

/* Board object and player info variable declaration */

Board board;
PlayerInfo player[4];

