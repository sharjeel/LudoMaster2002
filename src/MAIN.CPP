#include "vars.h"
#include "board.h"
#include "options.h"
#include "game.h"
#include "grafix.h"
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

/* Global Vars */
Board board[15][15];
PlayerInfo player[4];
int turn;

/* Function prototypes */
void initgraphics();
void initvars();

/* Main Function Definition */

void main()
{
	initvars();
	initgraphics();
	showlogo();
	
	do
	{
		while(!mainoptions());
		playgame();
	}while(1); //Infinite loop
}

void initgraphics()
{
	/* request auto detection */
	int gdriver = DETECT, gmode, errorcode;

	/* initialize graphics and local variables */
	initgraph(&gdriver, &gmode, "");

	/* read result of initialization */
	errorcode = graphresult();
	/* an error occurred */
	if (errorcode != grOk)
	{
		cout << "Graphics error: %s\n" << grapherrormsg(errorcode) << endl;
		cout << "Press any key to halt:";
		getch();
		exit(1);
	}
}

void initvars()
{
	//Set home location constants for every player's each piece
	player[PLR_BLUE].piece[0].homelocation.x = 1;
	player[PLR_BLUE].piece[0].homelocation.y = 1;
	player[PLR_BLUE].piece[1].homelocation.x = 4;
	player[PLR_BLUE].piece[1].homelocation.y = 1;
	player[PLR_BLUE].piece[2].homelocation.x = 1;
	player[PLR_BLUE].piece[2].homelocation.y = 4;
	player[PLR_BLUE].piece[3].homelocation.x = 4;
	player[PLR_BLUE].piece[3].homelocation.y = 4;

	player[PLR_GREEN].piece[0].homelocation.x = 10;
	player[PLR_GREEN].piece[0].homelocation.y = 1;
	player[PLR_GREEN].piece[1].homelocation.x = 13;
	player[PLR_GREEN].piece[1].homelocation.y = 1;
	player[PLR_GREEN].piece[2].homelocation.x = 10;
	player[PLR_GREEN].piece[2].homelocation.y = 4;
	player[PLR_GREEN].piece[3].homelocation.x = 13;
	player[PLR_GREEN].piece[3].homelocation.y = 4;

	player[PLR_RED].piece[0].homelocation.x = 10;
	player[PLR_RED].piece[0].homelocation.y = 10;
	player[PLR_RED].piece[1].homelocation.x = 13;
	player[PLR_RED].piece[1].homelocation.y = 10;
	player[PLR_RED].piece[2].homelocation.x = 10;
	player[PLR_RED].piece[2].homelocation.y = 13;
	player[PLR_RED].piece[3].homelocation.x = 13;
	player[PLR_RED].piece[3].homelocation.y = 13;

	player[PLR_YELLOW].piece[0].homelocation.x = 1;
	player[PLR_YELLOW].piece[0].homelocation.y = 10;
	player[PLR_YELLOW].piece[1].homelocation.x = 4;
	player[PLR_YELLOW].piece[1].homelocation.y = 10;
	player[PLR_YELLOW].piece[2].homelocation.x = 1;
	player[PLR_YELLOW].piece[2].homelocation.y = 13;
	player[PLR_YELLOW].piece[3].homelocation.x = 4;
	player[PLR_YELLOW].piece[3].homelocation.y = 13;

}