#include "mouse.h"
#include "vars.h"
#include "grafix.h"
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <iostream.h>
#include <dos.h>
	
extern Board board[15][15];
extern PlayerInfo player[4];

/* Private Function prototypes */
int gameoptions();
int loadgame();
void new_game_vars();

/* Function Definitions */
int mainoptions()
{

	//Will draw five buttons each having the following fucntions
	//1. New Game
	//2. Load Game
	//3. Rules
	//4. About
	//5. Quit
	button button[5];

	int xmax = getmaxx();
	int ymax = getmaxy();
	char *text;
	int i;
	unsigned size;
	mouse_data mouse;
	
	cleardevice();
	hide_mouse();
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0,0,xmax,ymax);

	//Draw the five buttons and store their info
	for(i = 0; i <= 4; i++)
	{
		//Mention the coordinates of the buttons
		button[i].coords[0][0] = xmax / 2 - 100;       // Left
		button[i].coords[0][1] = (i * 30) + 100;      // Top
		button[i].coords[1][0] = xmax / 2 + 100;       // Right
		button[i].coords[1][1] = ((i + 1) * 30) + 99; // Bottom
		
		//Dynamically allocate memory for images of up and down buttons
		size = imagesize(button[i].coords[0][0], button[i].coords[0][1], button[i].coords[1][0], button[i].coords[1][1]);

		button[i].up = (int *)malloc(size);
		if(!button[i].up){
			cout << "Error..";
			getch();
			exit(0);
		}

		button[i].down = (int *)malloc(size);
		if(!button[i].down){
			cout << "Error..";
			getch();
			exit(0);
		}

		//Put the text on button's face
		switch (i)
		{
			case 0:
				text = "Start New Game";
				break;
			case 1:
				text = "Load Game";
				break;
			case 2:
				text = "The Rules";
				break;
			case 3:
				text = "About Ludo Master 2002";
				break;
			case 4:
				text = "Quit";
				break;
		}
		//Draw button and store info
		draw_button(button[i].coords[0][0], button[i].coords[0][1], button[i].coords[1][0], button[i].coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, text, button[i].up,button[i].down);
	}
	show_mouse();

	//Wait for user's response through mouse
	do
	{
		//When mouse button is pressed see if it is pressed on a button, if yes then suprress that button
		read_mouse(&mouse);

		if (button_press(LEFT_BUTTON, &mouse))
		{
			// See which button user has pressed
			for(int i = 0; i <= 4; i++)
			{

				if ( mouse.x >= button[i].coords[0][0] && mouse.x <= button[i].coords[1][0] && mouse.y >= button[i].coords[0][1] && mouse.y <= button[i].coords[1][1])
				{
					hide_mouse();
					putimage(button[i].coords[0][0], button[i].coords[0][1], button[i].down, 0);
					show_mouse();
					delay(100);
                    hide_mouse();
					putimage(button[i].coords[0][0], button[i].coords[0][1], button[i].up  , 0);
					show_mouse();

					//Delete the memory allocated to buttons' images
                    for(int j = 0; j <= 4; j++)
					{
						free( (void*) button[j].up);
						free( (void*) button[j].down);
					}
 
					//Actions for each button user may click on
					switch (i)
					{
						case 0:
							// Start new game
							new_game_vars();
							return gameoptions();
							break;
						case 1:
							// Load game
//							return loadgame();
							return 1;
							break;
						case 2:
							// Rules of Ludo
							return 1;
							break;
						case 3:
							// About the author
							return 1;
							break;
						case 4:                   	
							// Quit
                            exit(1);
							break;
					}
			   }

			}
		}
	}	while(1); //Infinite loop
}

int gameoptions()
{
	int i, j, size;
	int xmax = getmaxx();
	int ymax = getmaxy();
	mouse_data mouse;
	
	button blue[3];
	button green[3];
	button red[3];
	button yellow[3];
	button ok;
	button cancel;

	// Clear previous elements and draw background
	cleardevice();
	hide_mouse();
	setfillstyle(SOLID_FILL, LIGHTGRAY);

	bar(0,0,xmax,ymax);

	settextjustify(CENTER_TEXT, BOTTOM_TEXT);
	setcolor(BLACK);
	outtextxy(xmax/2,20, "Please specify number and types of players");
	

	settextjustify(LEFT_TEXT, TOP_TEXT);
	setcolor(BLACK);
	//Draw block and buttons of green player
	setfillstyle(SOLID_FILL, GREEN);
	bar(xmax/2, 50, xmax/2 + 150, 200);
	outtextxy(xmax/2 + 5, 55, "Green Player:");

	//Draw block and buttons of red player
	setfillstyle(SOLID_FILL, RED);
	bar(xmax/2, 200, xmax/2 + 150, 350);
	outtextxy(xmax/2 + 5, 205, "Red Player");
	
	//Draw block and buttons of blue player
	setfillstyle(SOLID_FILL, BLUE);
	bar(xmax/2 - 150, 50, xmax/2, 200);
	outtextxy(xmax/2 - 145, 55, "Blue Player");
	
	//Draw block and buttons of yellow player
	setfillstyle(SOLID_FILL, YELLOW);
	bar(xmax/2 - 150, 200, xmax/2, 350);
	outtextxy(xmax/2 - 145, 205, "Yellow Player");

	//Assign coordinates and stores images of option buttons
	size = imagesize(0,0,100,30);
	for( i = 0; i <= 2; i++)
	{
		char *text;
		switch (i)
		{
			case 0:
				text = "None";
				break;
			case 1:
				text = "Human";
				break;
			case 2:
				text = "Computer";
				break;
		}
		
		//Set coordinaates for blue player's option buttons and store it as image
		blue[i].coords[0][0] = xmax/2 - 125;
		blue[i].coords[0][1] = 80 + i * 30;
		blue[i].coords[1][0] = xmax/2 - 25;
		blue[i].coords[1][1] = 80 + (i+1)*30;
		
		blue[i].up = (int *)malloc(size) ;
		blue[i].down = (int *)malloc(size) ;
		if(!blue[i].up || !blue[i].down)
		{
			cleardevice();
			closegraph();
			cout << endl << "Either u've run out of memory or memory can't be allocated. Ludo Master 2002 will quit now";
			getch();
			exit(0);
		}

		draw_button(blue[i].coords[0][0], blue[i].coords[0][1], blue[i].coords[1][0], blue[i].coords[1][1], DARKGRAY, LIGHTGRAY, BLACK, WHITE, text, blue[i].up,blue[i].down);
						
		//Set coordinaates for green player's option buttons and store it as image
		green[i].coords[0][0] = xmax/2 + 25;
		green[i].coords[0][1] = 80 + i * 30;
		green[i].coords[1][0] = xmax/2 + 125;
		green[i].coords[1][1] = 80 + (i+1)*30;
		
		green[i].up = blue[i].up;      // Images of buttons are same
		green[i].down = blue[i].down;  // Images of buttons are same
		putimage(green[i].coords[0][0], green[i].coords[0][1], green[i].up,0);
				
		//Set coordinaates for red player's option buttons and store it as image
		red[i].coords[0][0] = xmax/2 + 25;
		red[i].coords[0][1] = 230 + i * 30;
		red[i].coords[1][0] = xmax/2 + 125;
		red[i].coords[1][1] = 230 + (i+1)*30;
		
		red[i].up = blue[i].up;      // Images of buttons are same
		red[i].down = blue[i].down;  // Images of buttons are same
		putimage(red[i].coords[0][0], red[i].coords[0][1], red[i].up, 0);
		
		//Set coordinaates for yellow player's option buttons and store it as image
		yellow[i].coords[0][0] = xmax/2 - 125;
		yellow[i].coords[0][1] = 230 + i * 30;
		yellow[i].coords[1][0] = xmax/2 - 25;
		yellow[i].coords[1][1] = 230 + (i+1)*30;
		
		yellow[i].up = blue[i].up;      // Images of buttons are same
		yellow[i].down = blue[i].down;  // Images of buttons are same
		putimage(yellow[i].coords[0][0], yellow[i].coords[0][1], yellow[i].up,0);
	}
	
	//Draw Ok button
	size = imagesize(0,0,80,20);

	ok.coords[0][0] = xmax/2 - 105;
	ok.coords[0][1] = ymax - 70;
	ok.coords[1][0] = xmax/2 - 25;
	ok.coords[1][1] = ymax - 50;
	ok.up = (int *)malloc(size);
	ok.down = (int *)malloc(size);
	
	if(!ok.up || !ok.down) // Check if memory has been allocated
	{
		cleardevice();
		closegraph();
		cout << endl << "Either u've run out of memory or memory can't be allocated. Ludo Master 2002 will quit now";
		getch();
		exit(0);
	}
	
	draw_button(ok.coords[0][0], ok.coords[0][1], ok.coords[1][0], ok.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Start", ok.up,ok.down);

	//Draw cancel button
	cancel.coords[0][0] = xmax/2 + 25;
	cancel.coords[0][1] = ymax - 70;
	cancel.coords[1][0] = xmax/2 + 105;
	cancel.coords[1][1] = ymax - 50;
	cancel.up = (int *)malloc(size);
	cancel.down = (int *)malloc(size);
	
	if(!cancel.up || !cancel.down)
	{
		cleardevice();
		closegraph();
		cout << endl << "Either u've run out of memory or memory can't be allocated. Ludo Master 2002 will quit now";
		getch();
		exit(0);
	}
	draw_button(cancel.coords[0][0], cancel.coords[0][1], cancel.coords[1][0], cancel.coords[1][1], LIGHTBLUE, LIGHTCYAN, BLUE, WHITE, "Cancel", cancel.up, cancel.down);

	/* Set default as None option to all players */
	putimage(blue[0].coords[0][0], blue[0].coords[0][1], blue[0].down,0);
	putimage(green[0].coords[0][0], green[0].coords[0][1], green[0].down, 0);
	putimage(red[0].coords[0][0], red[0].coords[0][1], red[0].down,0);
	putimage(yellow[0].coords[0][0], yellow[0].coords[0][1], yellow[0].down,0);
	player[PLR_BLUE].type = TYPE_NONE;
	player[PLR_GREEN].type = TYPE_NONE;
	player[PLR_RED].type = TYPE_NONE;
	player[PLR_YELLOW].type = TYPE_NONE;
	
	show_mouse();

	/* Get user's input through mouse */
	do
	{
		read_mouse(&mouse);
		
		if (button_press(LEFT_BUTTON, &mouse))
		{
			// If blue player's option is pressed:
			if (mouse.x >= blue[0].coords[0][0] && mouse.x <= blue[2].coords[1][0] && mouse.y >= blue[0].coords[0][1] && mouse.y <= blue[2].coords[1][1])
			{
				hide_mouse();
				for (i = 0; i <= 2; i++)
				{
					putimage(blue[i].coords[0][0], blue[i].coords[0][1], blue[i].up, 0);
					if (mouse.x >= blue[i].coords[0][0] && mouse.x <= blue[i].coords[1][0] && mouse.y >= blue[i].coords[0][1] && mouse.y <= blue[i].coords[1][1])
					{
						putimage(blue[i].coords[0][0], blue[i].coords[0][1], blue[i].down, 0);
						player[PLR_BLUE].type = i;						
					}
				}
				show_mouse();
			}

			// If green player's option is pressed:
			if (mouse.x >= green[0].coords[0][0] && mouse.x <= green[2].coords[1][0] && mouse.y >= green[0].coords[0][1] && mouse.y <= green[2].coords[1][1])
			{
				hide_mouse();
				for (i = 0; i <= 2; i++)
				{
					putimage(green[i].coords[0][0], green[i].coords[0][1], green[i].up, 0);
					if (mouse.x >= green[i].coords[0][0] && mouse.x <= green[i].coords[1][0] && mouse.y >= green[i].coords[0][1] && mouse.y <= green[i].coords[1][1])
					{
						putimage(green[i].coords[0][0], green[i].coords[0][1], green[i].down, 0);
						player[PLR_GREEN].type = i;						
					}
				}
				show_mouse();
			}

			// If red player's option is pressed:
			if (mouse.x >= red[0].coords[0][0] && mouse.x <= red[2].coords[1][0] && mouse.y >= red[0].coords[0][1] && mouse.y <= red[2].coords[1][1])
			{
				hide_mouse();
				for (i = 0; i <= 2; i++)
				{
					putimage(red[i].coords[0][0], red[i].coords[0][1], red[i].up, 0);
					if (mouse.x >= red[i].coords[0][0] && mouse.x <= red[i].coords[1][0] && mouse.y >= red[i].coords[0][1] && mouse.y <= red[i].coords[1][1])
					{
						putimage(red[i].coords[0][0], red[i].coords[0][1], red[i].down, 0);
						player[PLR_RED].type = i;						
					}
				}
				show_mouse();
			}

			// If yellow player's option is pressed:
			if (mouse.x >= yellow[0].coords[0][0] && mouse.x <= yellow[2].coords[1][0] && mouse.y >= yellow[0].coords[0][1] && mouse.y <= yellow[2].coords[1][1])
			{
				hide_mouse();
				for (i = 0; i <= 2; i++)
				{
					putimage(yellow[i].coords[0][0], yellow[i].coords[0][1], yellow[i].up, 0);
					if (mouse.x >= yellow[i].coords[0][0] && mouse.x <= yellow[i].coords[1][0] && mouse.y >= yellow[i].coords[0][1] && mouse.y <= yellow[i].coords[1][1])
					{
						putimage(yellow[i].coords[0][0], yellow[i].coords[0][1], yellow[i].down, 0);
						player[PLR_YELLOW].type = i;						
					}
				}
				show_mouse();
			}

			// If Start/OK button is pressed:
			if (mouse.x >= ok.coords[0][0] && mouse.x <= ok.coords[1][0] && mouse.y >= ok.coords[0][1] && mouse.y <= ok.coords[1][1])
			{
				hide_mouse();
				putimage(ok.coords[0][0], ok.coords[0][1], ok.down, 0);
				show_mouse();
				delay(100);
				hide_mouse();
				putimage(ok.coords[0][0], ok.coords[0][1], ok.up, 0);
				show_mouse();
				delay(100);
				
				//Check how many players are playing
				int count = 0;
				for (i = 0; i <= 3; i++)
				{
					if (player[i].type != TYPE_NONE)
						count++;
				}
					
				//If number of players playing is less than 2 then give msg and do nothing
				
				if (count >= 2)
				{		
					// Free the memory allocated to images
					free ((void*) ok.up);
					free ((void*) ok.down);
					free ((void*) cancel.up);
					free ((void*) cancel.down);
					for(i = 0; i <= 3; i++)
					{
						free ((void*) blue[i].up);
						free ((void*) blue[i].down);
						free ((void*) green[i].up);
						free ((void*) green[i].down);
						free ((void*) red[i].up);
						free ((void*) red[i].down);
						free ((void*) yellow[i].up);
						free ((void*) yellow[i].down);
						return 1;
					}
				}
				else
				{
					settextjustify(CENTER_TEXT, CENTER_TEXT);
					setcolor(DARKGRAY);
					outtextxy(xmax/2, ymax - 90, "There should be atleast two players playing to start the game");
				}
			}
			
			// If Cancel button is pressed:	
			if (mouse.x >= cancel.coords[0][0] && mouse.x <= cancel.coords[1][0] && mouse.y >= cancel.coords[0][1] && mouse.y <= cancel.coords[1][1])
			{
				hide_mouse();
				putimage(cancel.coords[0][0], cancel.coords[0][1], cancel.down, 0);
				show_mouse();
				delay(100);
				hide_mouse();
				putimage(cancel.coords[0][0], cancel.coords[0][1], cancel.up, 0);
				show_mouse();
				delay(100);
				
				// Free the memory allocated to images
				free ((void*) ok.up);
				free ((void*) ok.down);
				free ((void*) cancel.up);
				free ((void*) cancel.down);
				for(i = 0; i <= 3; i++)
				{
					free ((void*) blue[i].up);
					free ((void*) blue[i].down);
					free ((void*) green[i].up);
					free ((void*) green[i].down);
					free ((void*) red[i].up);
					free ((void*) red[i].down);
					free ((void*) yellow[i].up);
					free ((void*) yellow[i].down);
				}
				
				return 0;
			}
		}
	}while(1);  //Infinite loop
}

int loadgame()
{
}

void new_game_vars()
{
	//Initiliase the variables for new game
	int i,j;
	
	for (i = 0; i <= 14; i++)
		for(j = 0; j <= 14; j++)
				board[i][j].top = -1;

	//Set home positions of blue geetees
	player[PLR_BLUE].piece[0].status = ST_ATHOME;
	player[PLR_BLUE].piece[1].status = ST_ATHOME;
	player[PLR_BLUE].piece[2].status = ST_ATHOME;
	player[PLR_BLUE].piece[3].status = ST_ATHOME;
	player[PLR_BLUE].piece[0].location.x = 1; 
	player[PLR_BLUE].piece[0].location.y = 1;
	player[PLR_BLUE].piece[1].location.x = 4; 
	player[PLR_BLUE].piece[1].location.y = 1;
	player[PLR_BLUE].piece[2].location.x = 1; 
	player[PLR_BLUE].piece[2].location.y = 4;
	player[PLR_BLUE].piece[3].location.x = 4; 
	player[PLR_BLUE].piece[3].location.y = 4;
	board[1][1].top = 0; 	board[1][1].pieces[0] = 0; 	board[1][1].player = PLR_BLUE;
	board[4][1].top = 0;	board[4][1].pieces[0] = 1;	board[4][1].player = PLR_BLUE;
	board[1][4].top = 0;	board[1][4].pieces[0] = 2;	board[1][4].player = PLR_BLUE;
	board[4][4].top = 0;	board[4][4].pieces[0] = 3;	board[4][4].player = PLR_BLUE;
	
	//Set home positions of green geetees
	player[PLR_GREEN].piece[0].status = ST_ATHOME;
	player[PLR_GREEN].piece[1].status = ST_ATHOME;
	player[PLR_GREEN].piece[2].status = ST_ATHOME;
	player[PLR_GREEN].piece[3].status = ST_ATHOME;
	player[PLR_GREEN].piece[0].location.x = 10; 
	player[PLR_GREEN].piece[0].location.y = 1;
	player[PLR_GREEN].piece[1].location.x = 13; 
	player[PLR_GREEN].piece[1].location.y = 1;
	player[PLR_GREEN].piece[2].location.x = 10; 
	player[PLR_GREEN].piece[2].location.y = 4;
	player[PLR_GREEN].piece[3].location.x = 13; 
	player[PLR_GREEN].piece[3].location.y = 4;
	board[10][1].top = 0; 	board[10][1].pieces[0] = 0;		board[10][1].player = PLR_GREEN;
	board[13][1].top = 0;	board[13][1].pieces[0] = 1;		board[13][1].player = PLR_GREEN;
	board[10][4].top = 0;	board[10][4].pieces[0] = 2;		board[10][4].player = PLR_GREEN;
	board[13][4].top = 0;	board[13][4].pieces[0] = 3;		board[13][4].player = PLR_GREEN;
}

void showlogo()
{
	int xmax = getmaxx();
	int ymax = getmaxy();
	mouse_data mouse;

	cleardevice();
	
	if(!init_mouse())
	{
		closegraph();
		cout << "Mouse couldn't be initialised";	
	}
	
	outtextxy(xmax/2 - 100, 100, "Ludo Master 2002");
	outtextxy(xmax/2 - 150, 300, "By Sharjeel Ahmed Qureshi");
	outtextxy(xmax/2 - 150, 400, "Press any key to continue");
	
	do
	{
		read_mouse(&mouse);
	}while(!kbhit());
		
}

