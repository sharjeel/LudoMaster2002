#include "vars.h"
#include "board.h"
#include "mouse.h"
#include "grafix.h"
#include "board.h"
#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <fstream.h>
#include <stdlib.h>
#include <time.h>

//Global Variables
extern Board board[15][15];
extern PlayerInfo player[4];

extern int turn; 

//Private structures
struct undonode
{
	int playerturn;
	int piece;
	int fromx;
	int fromy;
	int tox;
	int toy;
	int boolkilled;
	int playerkilled;
	int piecekilled;	
	undonode *prev;
};

//Local global variables
boardgraf boardgraf;
undonode *undotail = NULL;

//Variables for buttons
button 	roll,
		quit,
		help,
		undo,
		save;

//Private Constants
const int ROLLDIE = 0, UNDO = 1, SAVE = 2, HELP = 3, QUIT = 4;

//Private Funciton prototypes
void drawscreenelements();
int getuseroption(int);
void movepiece(int, int, int, int);
void sendhome(int, int);
int validmove(int, int, int, int, int&, int&);
int rolldie(int);
void savegame();
point nextblock(int, int, int);
point getuserinput();
int brd_pop(Board&);
void brd_push(Board&, int, int);
void addundo(int,int,int,int,int,int,int,int);
void doundo();

void playgame()
{
	int xmax = getmaxx(),
		ymax = getmaxy();
	int i, dice, turnagain, finished = FALSE, option, piece_to_move, boolkilledpiece, playerkilled, piecekilled;
	point usermove, moveto, homeaddress;
 

	/* Initialise Board's coordinates and its unit size */
	boardgraf.blocksize = (ymax - 30) / 15;
	if (boardgraf.blocksize%2 != 0)
		boardgraf.blocksize = boardgraf.blocksize - 1;
	boardgraf.left = xmax / 2 - boardgraf.blocksize * 7.5;
	boardgraf.top  = 5;

	mouse_data mouse;
	hide_mouse();

	drawscreenelements();
	
	show_mouse();

	do
	{
		turnagain = FALSE;
		//Clear the info bar
		setfillstyle(SOLID_FILL, WHITE);
		bar(0,ymax - 20, undo.coords[0][0] - 1, ymax);
		setcolor(BLACK);

		//Check if the player is playing whose turn it is
		if ( player[turn].type != TYPE_NONE )
		{
			//Check if its a Human Player or a Computer Player
			if (player[turn].type == TYPE_HUMAN)
			{
				//Write in info bar
				writestatus("Roll the die or choose other options");

				//Get user's opiniom if he/she wants to roll the die or save/quit etc
				option = getuseroption(turn);
				switch (option)
				{
					case ROLLDIE:
						//User has rolled die
						dice = rolldie(turn);

						//Check which pieces can move and which can't
						for(i = 0; i <= 3; i++)
						{
							player[turn].piece[i].canmove = FALSE;
							if (validmove( player[turn].piece[i].location.x, player[turn].piece[i].location.y, dice, turn, player[turn].piece[i].moveto.x, player[turn].piece[i].moveto.y))
							{
								player[turn].piece[i].canmove = TRUE;
							}
						}

						//If none of the move of the user can move then skip his/her turn
						if( player[turn].piece[0].canmove == TRUE ||
							player[turn].piece[1].canmove == TRUE ||
							player[turn].piece[2].canmove == TRUE ||
							player[turn].piece[3].canmove == TRUE 	)
						{

							writestatus("Please click on a piece on the board");
							
							if (dice == 6)
								turnagain = TRUE;


							//Get user's input for a location on board using mouse
							//If that location is occupied, and is occupied by his on piece and that piece can move then
							//move that piece
							do
							{
								usermove = getuserinput();
								piece_to_move = board[usermove.x][usermove.y].pieces[board[usermove.x][usermove.y].top];
							}while(	!(board[usermove.x][usermove.y].top >= 0 &&
									  board[usermove.x][usermove.y].player == turn &&
									  player[turn].piece[piece_to_move].canmove == TRUE));

							//Move that piece
							{		  
								//If there is already a piece sitting on that part, send it to home first (kill it)
								boolkilledpiece = FALSE;
								if(board[player[turn].piece[piece_to_move].moveto.x][player[turn].piece[piece_to_move].moveto.y].top >= 0 &&
									board[player[turn].piece[piece_to_move].moveto.x][player[turn].piece[piece_to_move].moveto.y].player != turn)
								{	
									playerkilled = board[player[turn].piece[piece_to_move].moveto.x][player[turn].piece[piece_to_move].moveto.y].player;
									piecekilled = board[player[turn].piece[piece_to_move].moveto.x][player[turn].piece[piece_to_move].moveto.y].pieces[0];
									homeaddress.x = player[playerkilled].piece[piecekilled].homelocation.x;
									homeaddress.y = player[playerkilled].piece[piecekilled].homelocation.y;
									sendhome(player[playerkilled].piece[piecekilled].location.x, player[playerkilled].piece[piecekilled].location.y);
									drawblock(homeaddress.x, homeaddress.y);
									draw_block_pieces(homeaddress.x, homeaddress.y);
									turnagain = TRUE;
									boolkilledpiece = TRUE;
								}	
								
								//Keep track for undopurpose
								addundo(turn, usermove.x, usermove.y, player[turn].piece[piece_to_move].moveto.x, player[turn].piece[piece_to_move].moveto.y,
										boolkilledpiece, playerkilled, piecekilled);
								
								//Change board coordinates of piece and show on screen
								movepiece(usermove.x, usermove.y, player[turn].piece[piece_to_move].moveto.x, player[turn].piece[piece_to_move].moveto.y);
								hide_mouse();
								drawblock(usermove.x, usermove.y);
								draw_block_pieces(usermove.x, usermove.y);
								show_mouse();


								//If piece lands on finished then turnagain = TRUE, also check if all pieces have finished. If yes then end game saying who has won
								//Otherwise just draw that piece on the baord
								if (player[turn].piece[piece_to_move].location.x >= 6 && player[turn].piece[piece_to_move].location.x <= 8 &&
									player[turn].piece[piece_to_move].location.y >= 6 && player[turn].piece[piece_to_move].location.y <= 8)
								{
									turnagain = TRUE;
									player[turn].piece[piece_to_move].status = ST_FINISHED;
									
									finished = TRUE;
									for(i = 0; i<= 3; i++)
									{
										if(player[turn].piece[i].status != ST_FINISHED)
											finished = FALSE;																					
									}
								}
								else
								{
									//Draw the piece on its new location
									hide_mouse();
									drawblock(player[turn].piece[piece_to_move].moveto.x, player[turn].piece[piece_to_move].moveto.y);
									draw_block_pieces(player[turn].piece[piece_to_move].moveto.x, player[turn].piece[piece_to_move].moveto.y);
									show_mouse();							
								}
							}														
						}
						break;
					case UNDO:
						doundo();
						turnagain = TRUE;					
						break;
						
					case SAVE:
						savegame();
						turnagain = TRUE;
						writestatus("Game Has been saved");
						break;
											
					case HELP:
						turnagain = TRUE;
						break;					
					case QUIT:
						//Deallocate the memory
						free ( (void*) roll.up);
						free ( (void*) roll.down);
						free ( (void*) quit.up);
						free ( (void*) quit.down);
						free ( (void*) help.up);
						free ( (void*) help.down);
						free ( (void*) undo.up);
						free ( (void*) undo.down);
						free ( (void*) save.up);
						free ( (void*) save.down);
						return;
						break;
				}
			}

			if (player[turn].type == TYPE_COMPUTER)
			{

			}
		}
	//If player isn't supposed to roll die again, next player's turn comes
	if (!turnagain)
		turn++;

	//After turn of the 4th player, 1st player's turn should come
	if (turn > 3)
		turn = 0;
	if (turn < 0)
		turn = 3;

	}while(!finished);
}

int validmove(int cordx, int cordy, int dice, int playernum, int &movetox, int &movetoy)
{
	point nextcords;

	int i, valid;

	// if not user's cords has any piece or not has its own piece
	if (board[cordx][cordy].top == -1)
		return FALSE;
	if (board[cordx][cordy].player != playernum)
		return FALSE;
	
	// if piece has finished then it can't move
	if (cordx >= 6 && cordx <= 	8 && cordy >= 6 && cordy <= 8)
		return FALSE;
	
	// Check if that piece can move

		//if piece is at home and dice is not six then that piece can't move
		if( ( (cordx > 0 && cordx < 6 && cordy > 0 && cordy < 6)  ||
			  (cordx > 8 && cordx < 14 && cordy > 0 && cordy < 6) ||
			  (cordx > 0 && cordx < 6 && cordy > 8 && cordy < 14)  || 
			  (cordx > 8 && cordx < 14 && cordy > 8 && cordy < 14) ) &&
			  dice != 6)
			return FALSE;

		//if piece is at home and dice IS six then that piece can move providced there is not doublee at his home
		if( ( (cordx > 0 && cordx < 6 && cordy > 0 && cordy < 6)  ||
			  (cordx > 8 && cordx < 14 && cordy > 0 && cordy < 6) ||
			  (cordx > 0 && cordx < 6 && cordy > 8 && cordy < 14) || 
			  (cordx > 8 && cordx < 14 && cordy > 8 && cordy < 14) ) &&
			  dice == 6)
			{
				nextcords = nextblock(cordx,cordy,playernum);
				if (board[nextcords.x][nextcords.y].top > 0 && board[nextcords.x][nextcords.y].player != playernum)
					return FALSE;
				else
				{
					nextcords = nextblock(cordx, cordy, playernum);
					movetox = nextcords.x;
					movetoy = nextcords.y;
					return TRUE;
				}
			}

		nextcords.x = cordx;
		nextcords.y = cordy;

		for (i = 1; i <= dice; i++)
		{
			nextcords = nextblock(nextcords.x, nextcords.y, playernum);

			// Check if there is double piece of any other player ahead
			if (board[nextcords.x][nextcords.y].top >= 1 && board[nextcords.x][nextcords.y].player != playernum)
				return FALSE;

			//Check if finish comes before number of moves shown on die
			if ( i != dice && ( (nextcords.x == 6 && nextcords.y == 7) ||
								(nextcords.x == 7 && nextcords.y == 6) ||
								(nextcords.x == 7 && nextcords.y == 8) ||
								(nextcords.x == 8 && nextcords.y == 7) ))
				return FALSE;
		}

		//At this spot we've got the coordinates where the piece may come to rest
		//Check if there is star and is occupied by someone else
		if (board[nextcords.x][nextcords.y].top >= 0 && board[nextcords.x][nextcords.y].player != playernum &&
			((nextcords.x == 2  && nextcords.y == 8 ) ||
			 (nextcords.x == 6  && nextcords.y == 2 ) ||
			 (nextcords.x == 12 && nextcords.y == 6 ) ||
			 (nextcords.x == 8  && nextcords.y == 12))
			)
		return FALSE;


		//Check if there is star and its not player's personal star
		if (board[nextcords.x][nextcords.y].top == 0 && board[nextcords.x][nextcords.y].player != playernum &&
			((nextcords.x == 1  && nextcords.y == 6 && playernum != PLR_BLUE ) ||
			 (nextcords.x == 8  && nextcords.y == 1 && playernum != PLR_GREEN) ||
			 (nextcords.x == 13 && nextcords.y == 8 && playernum != PLR_RED) ||
			 (nextcords.x == 6  && nextcords.y == 13 && playernum != PLR_YELLOW))
			)
			return FALSE;

		movetox = nextcords.x;
		movetoy = nextcords.y;
		return TRUE;
}

point nextblock(int cordx, int cordy, int playernum)
{
	/* This function will return the coordinates of a block's next block on which a piece is residing */

	point output;

	/*First set the next position for the four kind of blocks of homes*/

	//Set starting point for blue's home
	if(cordx >= 0 && cordx <= 5 && cordy >= 0 && cordy <= 5)
	{
		output.x = 1;
		output.y = 6;
		return output;
	}

	//Set starting point for green's home
	if(cordx >= 9 && cordx <= 14 && cordy >= 0 && cordy <= 5)
	{
		output.x = 8;
		output.y = 1;
		return output;
	}

	//Set starting point for red's home
	if(cordx >= 9 && cordx <= 14 && cordy >= 9 && cordy <= 14)
	{
		output.x = 13;
		output.y = 8;
		return output;
	}

	//Set starting point for yellow's home
	if(cordx >= 0 && cordx <= 5 && cordy >= 9 && cordy <= 14)
	{
		output.x = 6;
		output.y = 13;
		return output;
	}

	//Set next coords in blue's area
	if (cordx >= 1 && cordx <= 5 && cordy == 8)
	{
		output.x = cordx - 1;
		output.y = cordy;
		return output;
	}
	if (cordx == 0 && cordy == 8)
	{
		output.x = cordx;
		output.y = cordy - 1;
		return output;
	}
	if (cordx == 0 && cordy == 7)
	{
		if (playernum == PLR_BLUE)
		{
			//Send the piece towards finished
			output.x = cordx + 1;
			output.y = cordy;		
			return output;
		}
		else
		{
			output.x = cordx;
			output.y = cordy - 1;
			return output;
		}
	}
	if (cordy == 7 && cordx >= 1 && cordx <= 5)
	{
		output.x = cordx + 1;
		output.y = cordy;		
		return output;
	}
	if (cordy == 6 && cordx >= 0 && cordx <= 4)
	{
		output.x = cordx + 1;
		output.y = cordy;		
		return output;

	}
	if (cordy == 6 && cordx == 5)
	{
		output.x = cordx + 1;
		output.y = cordy - 1;
		return output;
	}
	
	//Set next coords in green's area
	if (cordy >= 1 && cordy <= 5 && cordx == 6)
	{
		output.x = cordx;
		output.y = cordy - 1;
		return output;
	}
	if (cordx == 6 && cordy == 0)
	{
		output.x = cordx + 1;
		output.y = cordy;
		return output;
	}
	if (cordx == 7 && cordy == 0)
	{
		if (playernum == PLR_GREEN)
		{
			//Send the piece towards finished
			output.x = cordx;
			output.y = cordy + 1;
			return output;
		}
		else
		{
			output.x = cordx + 1;
			output.y = cordy;
			return output;
		}
	}
	if (cordy >= 1 && cordy <= 5 && cordx == 7)
	{
		output.x = cordx;
		output.y = cordy + 1;
		return output;
	}
	if (cordx == 8 && cordy >= 0 && cordy <= 4)
	{
		output.x = cordx;
		output.y = cordy + 1;
		return output;

	}
	if (cordx == 8 && cordy == 5)
	{
		output.x = cordx + 1;
		output.y = cordy + 1;
		return output;
	}

	//Set next coords in red's area
	if (cordx >= 9 && cordx <= 13 && cordy == 6)
	{
		output.x = cordx + 1;
		output.y = cordy;
		return output;
	}
	if (cordx == 14 && cordy == 6)
	{
		output.x = cordx;
		output.y = cordy + 1;
		return output;
	}
	if (cordx == 14 && cordy == 7)
	{
		if (playernum == PLR_RED)
		{
			//Send the piece towards finished
			output.x = cordx - 1;
			output.y = cordy;
			return output;
		}
		else
		{
			output.x = cordx;
			output.y = cordy + 1;
			return output;
		}
	}
	if (cordx >= 9 && cordx <= 13 && cordy == 7)
	{
		output.x = cordx - 1;
		output.y = cordy;
		return output;
	}
	if (cordy == 8 && cordx >= 10 && cordx <= 14)
	{
		output.x = cordx - 1;
		output.y = cordy;
		return output;
	}
	if (cordx == 9  && cordy == 8)
	{
		output.x = cordx - 1;
		output.y = cordy + 1;
		return output;
	}

	//Set next coords in yellow's area
	if (cordy >= 9 && cordy <= 13 && cordx == 8)
	{
		output.x = cordx;
		output.y = cordy + 1;
		return output;
	}
	if (cordx == 8 && cordy == 14)
	{
		output.x = cordx - 1;
		output.y = cordy;
		return output;
	}
	if (cordx == 7 && cordy == 14)
	{
		if (playernum == PLR_YELLOW)
		{
			//Send the piece towards finished
			output.x = cordx;
			output.y = cordy - 1;
			return output;
		}
		else
		{
			output.x = cordx - 1;
			output.y = cordy;
			return output;
		}
	}
	if (cordy >= 9 && cordy <= 13 && cordx == 7)
	{
		output.x = cordx;
		output.y = cordy - 1;
		return output;
	}
	if (cordx == 6 && cordy >= 10 && cordy <= 14)
	{
		output.x = cordx;
		output.y = cordy - 1;
		return output;

	}
	if (cordx == 6 && cordy == 9)
	{
		output.x = cordx - 1;
		output.y = cordy - 1;
		return output;
	}
}

point getuserinput()
{
	int ymax = getmaxy();
	point output;
	mouse_data mouse;

	//Input using mouse for getting board coordinates of a piece which user wants to move
	do
	{
		read_mouse(&mouse);
		if (button_press(LEFT_BUTTON, &mouse))
		{
			if (mouse.x > boardgraf.left && mouse.x < boardgraf.left + boardgraf.blocksize * 15 && mouse.y > boardgraf.top && mouse.y < boardgraf.top + boardgraf.blocksize * 15)
			{
				output.x = (mouse.x - boardgraf.left) / boardgraf.blocksize;
				output.y = (mouse.y - boardgraf.top) / boardgraf.blocksize;
				return output;
			}
			else
				writestatus("Complete your turn first");
		}
	}while(1); //Infinite loop
}

void movepiece(int fromx, int fromy, int tox, int toy)
{
	//------------
	if(board[fromx][fromy].top <0)
		{
		gotoxy(1,1);
		cout << "errorrrr!!!!!!!!!!!!!!!!!!!! while calling movepiece";
		getch();
		return;
		}
	//-----------
		
	int playernum = board[fromx][fromy].player;
	int piece =	brd_pop(board[fromx][fromy]);
	brd_push(board[tox][toy], playernum,piece);
	player[playernum].piece[piece].location.x = tox;
	player[playernum].piece[piece].location.y = toy;
	player[playernum].piece[piece].status = ST_ONBOARD;
}

void undomove()
{
	
}

int rolldie(int playerturn)
{
	int number, x , y, xmax = getmaxx(), ymax = getmaxy();
	randomize();
	number = 1 + rand() % 6;
	
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar( (boardgraf.left - 5) / 2 - 10, (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 60, (boardgraf.left - 5) / 2 + 10, (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 40);
	bar( (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 10, (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 60, (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 + 10, (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 40);
	bar( (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 10, boardgraf.top + boardgraf.blocksize * 15 - 60, (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 + 10, boardgraf.top + boardgraf.blocksize * 15 - 40);
	bar( (boardgraf.left - 5) / 2 - 10, boardgraf.top + boardgraf.blocksize * 15 - 60, (boardgraf.left - 5) / 2 + 10, boardgraf.top + boardgraf.blocksize * 15 - 40);
		
	switch (playerturn)
	{
		case PLR_BLUE:
			x = (boardgraf.left - 5) / 2 - 10;
			y = (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 60;
			break;
		case PLR_GREEN:
			x = (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 10;
			y = (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 60;
			break;
		case PLR_RED:
			x = (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 10;
			y = boardgraf.top + boardgraf.blocksize * 15 - 60;
			break;
		case PLR_YELLOW:
			x = (boardgraf.left - 5) / 2 - 10;
			y = boardgraf.top + boardgraf.blocksize * 15 - 60;
			break;
	}

	number = getch() - 48;
	drawdice(number, x, y, 20);
	
	return number;
}

void drawscreenelements()
{
	int xmax = getmaxx(),
		ymax = getmaxy();
	int i,j, size;

	cleardevice();
	setfillstyle(SOLID_FILL, LIGHTGRAY);	
	bar(0,0,xmax,ymax);
	setfillstyle(SOLID_FILL, WHITE);	
	
	// ----------- Call draw board here
	bar(boardgraf.left, boardgraf.top, boardgraf.left + boardgraf.blocksize * 15, boardgraf.top + boardgraf.blocksize *15);
	// -----------
	
	/* Draw border */

	for (i = 0; i <= 4; i++)
	{
		setcolor(BLUE);
		line(boardgraf.left - (4 - i), i, boardgraf.left + 15 * boardgraf.blocksize + (4-i),i);
		setcolor(YELLOW);
		line(boardgraf.left - (4 - i), i, boardgraf.left - (4 - i), boardgraf.top + 15 * boardgraf.blocksize + (4 - i));
		setcolor(RED);
		line(boardgraf.left - i, i + boardgraf.blocksize * 15 + 6 , boardgraf.left + 15 * boardgraf.blocksize + i,i + boardgraf.blocksize * 15 + 6);
		setcolor(GREEN);
		line(boardgraf.left + boardgraf.blocksize * 15 + i, i + boardgraf.blocksize * 15 + 6 , boardgraf.left + boardgraf.blocksize * 15 + i, 4 - i);
	}
	
	setcolor(BLACK);
//	rectangle(0, 0, boardgraf.left - 5, (boardgraf.blocksize * 15) / 2 +  boardgraf.top );
//	rectangle(0, (boardgraf.blocksize * 15) / 2 +  boardgraf.top , boardgraf.left - 5, (boardgraf.blocksize * 15) +  boardgraf.top + 5);
//	rectangle(boardgraf.left + boardgraf.blocksize * 15 + 5, 0, xmax , (boardgraf.blocksize * 15) / 2 +  boardgraf.top);
//	rectangle(boardgraf.left + boardgraf.blocksize * 15 + 5, (boardgraf.blocksize * 15) / 2 +  boardgraf.top , xmax , (boardgraf.blocksize * 15) +  boardgraf.top + 5);
	
	settextjustify(LEFT_TEXT, TOP_TEXT);
	setcolor(BLUE);
	outtextxy(5,5,"Blue");
	setcolor(BLACK);
	outtextxy(boardgraf.left + boardgraf.blocksize * 15 + 10, 5, "Green");
	setcolor(RED);
	outtextxy(boardgraf.left + boardgraf.blocksize * 15 + 10, (boardgraf.blocksize * 15) / 2 +  boardgraf.top + 5, "Red");
	setcolor(YELLOW);
	outtextxy(5,(boardgraf.blocksize * 15) / 2 +  boardgraf.top + 5, "Yellow"); 

	/* Draw buttons */
	size = imagesize(0,0,80,20);	
	help.up   = (int*)malloc(size);
	help.down = (int*)malloc(size);
	undo.up   = (int*)malloc(size);
	undo.down = (int*)malloc(size);
	save.up   = (int*)malloc(size);
	save.down = (int*)malloc(size);
	quit.up   = (int*)malloc(size);
	quit.down = (int*)malloc(size);
	size = imagesize(0,0,49,20);
	roll.up   = (int*)malloc(size);
	roll.down = (int*)malloc(size);
	
	if (help.up == NULL || help.down == NULL || undo.up == NULL || undo.down == NULL || save.up == NULL || save.down == NULL || quit.up == NULL || quit.down == NULL || roll.up == NULL || roll.down == NULL)
	{
		closegraph();
		cout << "Your system has run out of memory!\n" 
		     << "Ludo master 2002 will quit now.\n" 
		     << "Try allocating more memory to the programme if you are using windows\n"
			 << "For dos, use QEMM drivers";
		getch();
		exit(0);
	}

	/* Menu */
	
	//Quit button:	
	quit.coords[0][0] = xmax - 79;
	quit.coords[0][1] = ymax - 20;
	quit.coords[1][0] = xmax;
	quit.coords[1][1] = ymax;
	draw_button( quit.coords[0][0] , quit.coords[0][1], quit.coords[1][0], quit.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Quit", quit.up, quit.down);
	
	//Help button:
	help.coords[0][0] = quit.coords[0][0] - 80;
	help.coords[0][1] = ymax - 20;
	help.coords[1][0] = quit.coords[1][0] - 80;
	help.coords[1][1] = ymax;
	draw_button( help.coords[0][0] , help.coords[0][1], help.coords[1][0], help.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Help", help.up, help.down);

	//Save button:
	save.coords[0][0] = help.coords[0][0] - 80;
	save.coords[0][1] = ymax - 20;
	save.coords[1][0] = help.coords[1][0] - 80;
	save.coords[1][1] = ymax;
	draw_button( save.coords[0][0] , save.coords[0][1], save.coords[1][0], save.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Save", save.up, save.down);
	
	//Undo button:
	undo.coords[0][0] = save.coords[0][0] - 80;
	undo.coords[0][1] = ymax - 20;
	undo.coords[1][0] = save.coords[1][0] - 80;
	undo.coords[1][1] = ymax;
	draw_button( undo.coords[0][0] , undo.coords[0][1], undo.coords[1][0], undo.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Undo", undo.up, undo.down);
	
	//Draw Roll button, store it as image button and vanish it
	roll.coords[0][0] = 0;
	roll.coords[0][1] = ymax - 20;
	roll.coords[1][0] = 49;
	roll.coords[1][1] = ymax;
	draw_button(roll.coords[0][0], roll.coords[0][1], roll.coords[1][0], roll.coords[1][1], DARKGRAY, LIGHTGRAY, BLACK, WHITE, "Roll", roll.up, roll.down);
	setfillstyle (SOLID_FILL, LIGHTGRAY);
	bar (roll.coords[0][0], roll.coords[0][1], roll.coords[1][0], roll.coords[1][1]);
	
	//Draw board
	drawboard( boardgraf.left, boardgraf.top, boardgraf.blocksize);
	
	//Draw the bottom most message bar
	writestatus(" ");
		
	for (j = 0; j <= 3; j++)
		if (player[j].type != TYPE_NONE)
			for (i = 0; i <= 3; i++)
				draw_block_pieces(player[j].piece[i].location.x, player[j].piece[i].location.y);
}

int getuseroption(int playerturn)
{
	mouse_data mouse;
	int xmax = getmaxx(),
		ymax = getmaxy();
	int left, top;
//	rectangle(0, 0, boardgraf.left - 5, (boardgraf.blocksize * 15) / 2 +  boardgraf.top );
//	rectangle(0, (boardgraf.blocksize * 15) / 2 +  boardgraf.top , boardgraf.left - 5, (boardgraf.blocksize * 15) +  boardgraf.top + 5);
//	rectangle(boardgraf.left + boardgraf.blocksize * 15 + 5, 0, xmax , (boardgraf.blocksize * 15) / 2 +  boardgraf.top);
//	rectangle(boardgraf.left + boardgraf.blocksize * 15 + 5, (boardgraf.blocksize * 15) / 2 +  boardgraf.top , xmax , (boardgraf.blocksize * 15) +  boardgraf.top + 5);
	

	if (player[playerturn].type == TYPE_HUMAN)
	{
		//Draw the roll button
		switch (playerturn)
		{
			case PLR_BLUE:
				left = (boardgraf.left - 5) / 2 - 25;
				top  = (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 30;
				break;
			case PLR_GREEN:
				left = (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 25;
				top  = (boardgraf.blocksize * 15) / 2 +  boardgraf.top - 30;
				break;
			case PLR_RED:
				left = (boardgraf.left + boardgraf.blocksize * 15 + 5 + xmax) / 2 - 25;
				top  = boardgraf.blocksize * 15 +  boardgraf.top - 30;
				break;
			case PLR_YELLOW:
				left = (boardgraf.left - 5) / 2 - 25;
				top  = boardgraf.blocksize * 15 +  boardgraf.top - 30;
				break;
		}
		hide_mouse();
		putimage(left, top, roll.up, 0);
		show_mouse();


		//Get user's mouse input
		do
		{
			read_mouse(&mouse);
			
			if (button_press(LEFT_BUTTON, &mouse))
			{
				//If roll dice is clicked
				if (mouse.x > left && mouse.x < left + 50 && mouse.y > top && mouse.y < top + 20)
				{
					// Animate the button as pressed and vanish it
					hide_mouse();
					putimage(left, top, roll.down, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					putimage(left, top, roll.up, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					setfillstyle( SOLID_FILL, LIGHTGRAY);
					bar(left, top, left + 50, top + 20);
					show_mouse();
					return ROLLDIE;
				}
				
				//If undo is clicked
				if (mouse.x > undo.coords[0][0] && mouse.x < undo.coords[1][0] && mouse.y > undo.coords[0][1] && mouse.y < undo.coords[1][1])
				{
					hide_mouse();
					putimage(undo.coords[0][0], undo.coords[0][1], undo.down, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					putimage(undo.coords[0][0], undo.coords[0][1], undo.up, 0);
					show_mouse();
					
					//-----Undo statements here
					return UNDO;
				}
				
				//If save is clicked
				if (mouse.x > save.coords[0][0] && mouse.x < save.coords[1][0] && mouse.y > save.coords[0][1] && mouse.y < save.coords[1][1])
				{
					hide_mouse();
					putimage(save.coords[0][0], save.coords[0][1], save.down, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					putimage(save.coords[0][0], save.coords[0][1], save.up, 0);
					show_mouse();
					
					//-----Save statements here
					return SAVE;
				}
				
				//If Help is clicked

				{
					hide_mouse();
					putimage(help.coords[0][0], help.coords[0][1], help.down, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					putimage(help.coords[0][0], help.coords[0][1], help.up, 0);
					show_mouse();
					
					//-----Help statements here
					return HELP;
				}
				
				//If Quit is clicked
				if (mouse.x > quit.coords[0][0] && mouse.x < quit.coords[1][0] && mouse.y > quit.coords[0][1] && mouse.y < quit.coords[1][1])
				{
					hide_mouse();
					putimage(quit.coords[0][0], quit.coords[0][1], quit.down, 0);
					show_mouse();
					delay(100);
					hide_mouse();
					putimage(quit.coords[0][0], quit.coords[0][1], quit.up, 0);
					show_mouse();
					
					//-----Quit statements here
					return QUIT;
				}
			}
		}while(1); //Infinite loop
	}
	
}

void sendhome(int cordx, int cordy)
{
	//---------------	
	if (board[cordx][cordy].top != 0)
	{	
		gotoxy (1,1);
		cout << "Errooor in sendhome() in game.cpp!!!";
		return;
	}
	//--------
		
	int playernum = board[cordx][cordy].player;
	int piecenum =	brd_pop(board[cordx][cordy]);
	int puttox = player[playernum].piece[piecenum].homelocation.x;
	int puttoy = player[playernum].piece[piecenum].homelocation.y;
	brd_push(board[puttox][puttoy], playernum,piecenum);
	player[playernum].piece[piecenum].status = ST_ATHOME;
	player[playernum].piece[piecenum].location.x = puttox;
	player[playernum].piece[piecenum].location.y = puttoy;
}

void savegame()
{
	int i,j;
	fstream file("save.lud",ios::out);

	file.write((char*)&turn, sizeof turn);

	for (i = 0; i <= 3; i++)
	{
		file.write((char*)&player[i].type, sizeof player[i].type);
		
		for (j = 0; j <= 3; j++)
		{	
			file.write( (char*)&player[i].piece[j].status, sizeof player[i].piece[j].status);
			file.write( (char*)&player[i].piece[j].location, sizeof player[i].piece[j].location);
		}
	}
	
	file.close();
	return;	
}

int brd_pop(Board& block)
{
	//-----
		if (block.top == -1)
		{
			gotoxy (1,1);
			cout << "Logical error while calling brd_pop()";
		}
	//-----
		
	int output;
	
	output = block.pieces[block.top];
	block.top --;
	return output;
}

void brd_push(Board& block, int playernum, int piece)
{
	//-----
		if (block.top >= 4)
		{
			gotoxy(1,1);
			cout << "Logical error while calling brd_push()";
			return;
		}
		
		if (block.top != -1 && block.player != playernum)
		{
			cout << "ek he jagah par do geetian bithanay ki nakam koshish!!!!";
			return;	
		}
	//-----
		
	block.player = playernum;
	block.top++;
	block.pieces[block.top] = piece;
	return;
}

void addundo(int turn, int fromx, int fromy, int tox, int toy, int boolkilled, int playerkilled, int piecekilled )
{
	if (undotail == NULL)
	{
		//If its first move then make a new node
		undotail = new undonode;
		undotail->prev = NULL;
	}
	else
	{
		//Add new node
			undonode *tempnode = new undonode;
			tempnode->prev = undotail;
			undotail = tempnode;
	}

	//Assign values to node
	undotail->playerturn = turn;
	undotail->fromx = fromx;
	undotail->fromy = fromy;
	undotail->tox = tox;
	undotail->toy = toy;
	undotail->boolkilled = boolkilled;
	undotail->playerkilled = playerkilled;
	undotail->piecekilled = piecekilled;
}

void doundo()
{
	if (undotail == NULL)
	{
		writestatus("There is no move to undo");
		delay(100);
		return;
	}

	//Move the pieces back according to values stored in the node
	movepiece(undotail->tox, undotail->toy, undotail->fromx, undotail->fromy);
	if(undotail->boolkilled == TRUE)
	{
		movepiece(player[undotail->playerkilled].piece[undotail->piecekilled].homelocation.x, player[undotail->playerkilled].piece[undotail->piecekilled].homelocation.y,
				  undotail->tox, undotail->toy);
		drawblock(player[undotail->playerkilled].piece[undotail->piecekilled].homelocation.x, player[undotail->playerkilled].piece[undotail->piecekilled].homelocation.y);
	}
	turn = undotail->playerturn;

	drawblock(undotail->fromx,undotail->fromy);
	drawblock(undotail->tox,undotail->toy);
	draw_block_pieces(undotail->fromx,undotail->fromy);
	draw_block_pieces(undotail->tox,undotail->toy);

	//Delete the last node, just before the tail
	undonode *tempnode;
	tempnode = undotail;
	undotail = tempnode->prev;
	delete tempnode;
}