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

/* Function prototypes */
void initgraphics();

/* Main Function Definition */

void main()
{

	initgraphics();
	showlogo();
	while(!mainoptions());
	playgame();
	getch();
	getch();

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