#include "board.h"
#include "grafix.h"
#include "vars.h"
#include <graphics.h>
	
extern boardgraf boardgraf;
extern Board board[15][15];
extern PlayerInfo player[4];

void circblock(int, int, int, int);

void drawboard(int startx, int starty, int blocksize)
{
	//Integers declaration
	int iter,iterx,itery;
	//Constant integers decalaration
	int scale = blocksize  * 15;

	setcolor(BLACK);


	//Draw boxes for ludo board:-
	//~~~~~~~~~~~~~~~~~~~~~~~~~
	{
		int locationfromx, locationfromy,
			 locationtox, locationtoy;
		for (iter = 1; iter <=4; iter++)
		{
			switch(iter)
			{
			case 1:
				// For the upper portion
				locationfromx = 6;
				locationfromy = 0;
				locationtox   = 8;
				locationtoy   = 5;
				break;
			case 2:
				// For the left portion
				locationfromx = 0;
				locationfromy = 6;
				locationtox   = 5;
				locationtoy   = 8;
				break;
			case 3:
				// For the right
				locationfromx = 9;
				locationfromy = 6;
				locationtox   = 14;
				locationtoy   = 8;
				break;
			case 4:
				// For the lower portion
				locationfromx = 6;
				locationfromy = 9;
				locationtox   = 8;
				locationtoy   = 14;
				break;
			}

			for(itery = (blocksize*locationfromy) + starty; itery <= (blocksize * locationtoy) + starty; itery = itery + blocksize)
				for(iterx = (blocksize*locationfromx)+startx; iterx <= (blocksize * locationtox) + startx; iterx = iterx + blocksize)
				{
					rectangle (iterx, itery, iterx + blocksize, itery + blocksize);
				}
		}

		// Draw home in middle
		line(startx + blocksize * 6, starty + blocksize * 6, startx + blocksize * 9, starty + blocksize * 9);
		line(startx + blocksize * 9, starty + blocksize * 6, startx + blocksize * 6, starty + blocksize * 9);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill( startx + blocksize * 6.5, starty + blocksize * 7.5, BLACK);
		setfillstyle(SOLID_FILL, GREEN);
		floodfill( startx + blocksize * 7.5, starty + blocksize * 6.5, BLACK);
		setfillstyle(SOLID_FILL, RED);
		floodfill( startx + blocksize * 8.5, starty + blocksize * 7.5, BLACK);
		setfillstyle(SOLID_FILL, YELLOW);
		floodfill( startx + blocksize * 7.5, starty + blocksize * 8.5, BLACK);
		setcolor(BLACK);
		setfillstyle(SOLID_FILL, WHITE);
		fillellipse(startx + blocksize * 7.5, starty + blocksize * 7.5, blocksize, blocksize);
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		outtextxy(startx + blocksize * 7.5, starty + blocksize * 7.5, "Home");
	} //End draw boxes
	
	//Draw blue player's home area
	setcolor(BLUE);
	setfillstyle(CLOSE_DOT_FILL, BLUE);
	bar(startx, starty, startx + blocksize * 6 - 1, starty + blocksize * 6 - 1);
	setfillstyle(SOLID_FILL, WHITE);
	bar(startx + blocksize * 0.5, starty + blocksize * 0.5, startx + blocksize * 5.5,  starty + blocksize * 5.5);
	setfillstyle(SOLID_FILL, BLUE);
	circle(startx + blocksize     + blocksize / 2 , starty + blocksize     + blocksize / 2 , blocksize / 2 );
	circle(startx + blocksize * 4 + blocksize / 2 , starty + blocksize     + blocksize / 2 , blocksize / 2 );
	circle(startx + blocksize     + blocksize / 2 , starty + blocksize * 4 + blocksize / 2 , blocksize / 2 );
	circle(startx + blocksize * 4 + blocksize / 2 , starty + blocksize * 4 + blocksize / 2 , blocksize / 2 );
	line(startx + blocksize * 2, starty + blocksize * 1.5, startx + blocksize * 2  , starty + blocksize * 4.5);
	line(startx + blocksize * 4, starty + blocksize * 1.5, startx + blocksize * 4  , starty + blocksize * 4.5);
	line(startx + blocksize * 1.5, starty + blocksize * 2, startx + blocksize * 4.5, starty + blocksize * 2);
	line(startx + blocksize * 1.5, starty + blocksize * 4, startx + blocksize * 4.5, starty + blocksize * 4);
	floodfill(startx + blocksize * 2.5, starty + blocksize * 2.5, BLUE);

	//Draw green player's home area
	setcolor(GREEN);
	setfillstyle(CLOSE_DOT_FILL, GREEN);
	bar(startx + blocksize * 9 + 1, starty, startx + blocksize * 15, starty + blocksize * 6 - 1);
	setfillstyle(SOLID_FILL, WHITE);
	bar(startx + blocksize * 9.5, starty + blocksize * 0.5, startx + blocksize * 14.5,  starty + blocksize * 5.5);
	setfillstyle(SOLID_FILL, GREEN);
	circle(startx + blocksize * 10.5, starty + blocksize * 1.5, blocksize / 2 );
	circle(startx + blocksize * 13.5, starty + blocksize * 1.5, blocksize / 2 );
	circle(startx + blocksize * 10.5, starty + blocksize * 4.5, blocksize / 2 );
	circle(startx + blocksize * 13.5, starty + blocksize * 4.5, blocksize / 2 );
	line(startx + blocksize * 11, starty + blocksize * 1.5, startx + blocksize * 11  , starty + blocksize * 4.5);
	line(startx + blocksize * 13, starty + blocksize * 1.5, startx + blocksize * 13  , starty + blocksize * 4.5);
	line(startx + blocksize * 10.5, starty + blocksize * 2, startx + blocksize * 13.5, starty + blocksize * 2);
	line(startx + blocksize * 10.5, starty + blocksize * 4, startx + blocksize * 13.5, starty + blocksize * 4);
	floodfill(startx + blocksize * 12, starty + blocksize * 2.5, GREEN);

	//Draw yellow player's home area
	setcolor(BLACK);
	setfillstyle(CLOSE_DOT_FILL, YELLOW);
	bar(startx, starty + blocksize * 9 + 1, startx + blocksize * 6 - 1, starty + blocksize * 15);
	setfillstyle(SOLID_FILL, WHITE);
	bar(startx + blocksize * 0.5, starty + blocksize * 9.5, startx + blocksize * 5.5,  starty + blocksize * 14.5);
	setfillstyle(SOLID_FILL, BLACK);
	circle(startx + blocksize * 1.5, starty + blocksize * 10.5, blocksize / 2 );
	circle(startx + blocksize * 4.5, starty + blocksize * 10.5, blocksize / 2 );
	circle(startx + blocksize * 1.5, starty + blocksize * 13.5 , blocksize / 2 );
	circle(startx + blocksize * 4.5, starty + blocksize * 13.5 , blocksize / 2 );
	line(startx + blocksize * 2, starty + blocksize * 10.5, startx + blocksize * 2  , starty + blocksize * 13.5);
	line(startx + blocksize * 4, starty + blocksize * 10.5, startx + blocksize * 4  , starty + blocksize * 13.5);
	line(startx + blocksize * 1.5, starty + blocksize * 11, startx + blocksize * 4.5, starty + blocksize * 11);
	line(startx + blocksize * 1.5, starty + blocksize * 13, startx + blocksize * 4.5, starty + blocksize * 13);
	floodfill(startx + blocksize * 2.5, starty + blocksize * 11.5, BLACK);

	//Draw red player's home area
	setcolor(RED);
	setfillstyle(CLOSE_DOT_FILL, RED);
	bar(startx + blocksize * 9 + 1, starty + blocksize * 9 + 1, startx + blocksize * 15, starty + blocksize * 15);
	setfillstyle(SOLID_FILL, WHITE);
	bar(startx + blocksize * 9.5, starty + blocksize * 9.5, startx + blocksize * 14.5,  starty + blocksize * 14.5);
	setfillstyle(SOLID_FILL, RED);
	circle(startx + blocksize * 10.5, starty + blocksize * 10.5, blocksize / 2 );
	circle(startx + blocksize * 13.5, starty + blocksize * 10.5, blocksize / 2 );
	circle(startx + blocksize * 10.5, starty + blocksize * 13.5, blocksize / 2 );
	circle(startx + blocksize * 13.5, starty + blocksize * 13.5, blocksize / 2 );
	line(startx + blocksize * 11, starty + blocksize * 10.5, startx + blocksize * 11  , starty + blocksize * 13.5);
	line(startx + blocksize * 13, starty + blocksize * 10.5, startx + blocksize * 13  , starty + blocksize * 13.5);
	line(startx + blocksize * 10.5, starty + blocksize * 11, startx + blocksize * 13.5, starty + blocksize * 11);
	line(startx + blocksize * 10.5, starty + blocksize * 13, startx + blocksize * 13.5, starty + blocksize * 13);
	floodfill(startx + blocksize * 12, starty + blocksize * 11.5, RED);
	
	//Draw stops and home paths on board
	for(iterx = 0; iterx <= 5; iterx++)
		for(itery = 6; itery <= 8; itery++)
			drawblock(iterx,itery);
	for(iterx = 6; iterx <= 8; iterx++)
		for(itery = 0; itery <= 5; itery++)
			drawblock(iterx,itery);
	for(iterx = 9; iterx <= 14; iterx++)
		for(itery = 6; itery <= 8; itery++)
			drawblock(iterx,itery);
	for(iterx = 6; iterx <= 8; iterx++)
		for(itery = 9; itery <= 14; itery++)
			drawblock(iterx,itery);

}

void draw_block_pieces(int cordx, int cordy)
{
	int color;
	int nopieces = board[cordx][cordy].top + 1;
	int x,y,radius;

	if (nopieces == 0)
		return;

	switch (board[cordx][cordy].player)
	{
		case PLR_BLUE:	
			color = BLUE;
			break;
		case PLR_GREEN:
			color = GREEN;
			break;
		case PLR_RED:
			color = RED;
			break;
		case PLR_YELLOW:
			color = YELLOW;
			break;
	}
	
	switch (nopieces)
	{
		case 1:
			radius = boardgraf.blocksize /3 - 2;
			x = boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize/2;
			y = boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize/2;
			piecedraw(x, y, radius, color);
			break;
		case 2:
			radius = boardgraf.blocksize /4 - 2;
			x = boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize/2;
			y = boardgraf.top + boardgraf.blocksize * cordy + radius + 2;
			piecedraw(x, y, radius, color);
			y = boardgraf.top + boardgraf.blocksize * (cordy + 1) - radius - 2;
			piecedraw(x, y, radius, color);
			break;
		case 3:
			radius = boardgraf.blocksize /4 - 2;
			x = boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize/2;
			y = boardgraf.top + boardgraf.blocksize * cordy + radius + 2;
			piecedraw(x, y, radius, color);
			y = boardgraf.top + boardgraf.blocksize * (cordy + 1) - radius - 2;
			piecedraw(x, y, radius, color);
			x = boardgraf.left + boardgraf.blocksize * cordx + radius + 2;
			y = boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize / 2;
			piecedraw(x, y, radius, color);
			break;
		case 4:	
			radius = boardgraf.blocksize /4 - 2;
			x = boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize/2;
			y = boardgraf.top + boardgraf.blocksize * cordy + radius + 2;
			piecedraw(x, y, radius, color);
			y = boardgraf.top + boardgraf.blocksize * (cordy + 1) - radius - 2;
			piecedraw(x, y, radius, color);
			x = boardgraf.left + boardgraf.blocksize * cordx + radius + 2;
			y = boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize / 2;
			piecedraw(x, y, radius, color);
			x = boardgraf.left + boardgraf.blocksize * (cordx + 1) - radius - 2;
			y = boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize / 2;
			piecedraw(x, y, radius, color);
			break;
	}
}

void drawblock(int cordx, int cordy)
{
	/* Check the phase of the block to be drawn */
	
	//Block is in blue's playing area
	if (cordx >= 0 && cordx <= 5 && cordy >= 6 && cordy <= 8)
	{
		if ((cordx == 1 && cordy == 6) || (cordx == 2 && cordy == 8))
		{
			//Draw stops
 			circblock(cordx, cordy, BLUE, BLACK);
 			return;
 		}
 		
 		if (cordx >= 1 && cordx <= 5 && cordy == 7)
		{
			//Draw path to home of Blue
 			circblock(cordx, cordy, BLUE, BLACK);
 			return;
 		}
 		
 		//Otherwise just vanish the area
 		circblock(cordx, cordy, WHITE, WHITE);
 		return;
	}
	
	//Block is in green's playing area
	if (cordx >= 6 && cordx <= 8 && cordy >= 0 && cordy <= 5)
	{
		if ((cordx == 6 && cordy == 2) || (cordx == 8 && cordy == 1))
		{
			//Draw stops
 			circblock(cordx, cordy, GREEN, BLACK);
 			return;
 		}
 		
 		if (cordx == 7 && cordy >= 1 && cordy <= 5)
		{
			//Draw path to home of Green
 			circblock(cordx, cordy, GREEN, BLACK);
 			return;
 		}
 		
 		//Otherwise just vanish the area
 		circblock(cordx, cordy, GREEN, WHITE);
 		return;
	}
	
	//Block is in red's playing area
	if (cordx >= 9 && cordx <= 14 && cordy >= 6 && cordy <= 8)
	{
		if ((cordx == 12 && cordy == 6) || (cordx == 13 && cordy == 8))
		{
			//Draw stops
 			circblock(cordx, cordy, RED, BLACK);
 			return;
 		}
 		
 		if (cordx >= 9 && cordx <= 13 && cordy == 7)
		{
			//Draw path to home of Red
 			circblock(cordx, cordy, RED, BLACK);
 			return;
 		}
 		
 		//Otherwise just vanish the area
 		circblock(cordx, cordy, RED, WHITE);
 		return;
	}
	
	//Block is in yellow's playing area
	if (cordx >= 6 && cordx <= 8 && cordy >= 9 && cordy <= 14)
	{
		if ((cordx == 6 && cordy == 13) || (cordx == 8 && cordy == 12))
		{
			//Draw stops
 			circblock(cordx, cordy, YELLOW, BLACK);
 			return;
 		}
 		
 		if (cordx == 7 && cordy >= 9 && cordy <= 13)
		{
			//Draw path to home of Blue
 			circblock(cordx, cordy, YELLOW, BLACK);
 			return;
 		}
 		
 		//Otherwise just vanish the area
 		circblock(cordx, cordy, YELLOW, WHITE);
 		return;
	}
	
	//When block is of home just vanish the portion of piece
	setcolor(WHITE);
	for(int i = 0; i < boardgraf.blocksize /2; i++)
		circle( boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize /2,
				boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize /2,
				i);
}

void circblock(int cordx, int cordy, int color, int boundcol)
{
	int i;
	int radius = boardgraf.blocksize /2 - 2;
	int x = boardgraf.left + boardgraf.blocksize * cordx + boardgraf.blocksize /2;
	int y = boardgraf.top + boardgraf.blocksize * cordy + boardgraf.blocksize /2; 

	//Vanish the area first
	setfillstyle(SOLID_FILL, WHITE);
	bar(boardgraf.left + boardgraf.blocksize * cordx + 1, boardgraf.top + boardgraf.blocksize * cordy + 1,
		boardgraf.left + boardgraf.blocksize * (cordx + 1) - 1, boardgraf.top + boardgraf.blocksize * (cordy +1) - 1);
		
	//Draw circle of the specified color alongwith boundry
	setcolor(boundcol);
	circle(x,y,radius);
	setfillstyle(LINE_FILL, color);
	floodfill(x,y, boundcol);
}
