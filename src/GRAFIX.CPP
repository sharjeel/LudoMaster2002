#include "grafix.h"
#include <graphics.h>

void store_dice_images()
{
}

void drawdice(int number, int x, int y, int size)
{
	int i;
	
	setfillstyle(SOLID_FILL, WHITE);
	bar(x,y, x+size, y+size);
	setcolor(BLACK);
	rectangle(x,y, x+size, y+size);

	switch (number)
	{
		case 1:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /2, y + size /2, i);
			break;			
		case 2:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size - size /4, i);
			break;			
		case 3:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /2, y + size /2, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size - size /4, i);
			break;
		case 4:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size - size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size - size /4, i);
			break;
		case 5:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /2, y + size /2, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size - size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size - size /4, i);
			break;
		case 6:
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size - size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size - size /4, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size /4, y + size /2, i);
			for (i = 0; i <= size /10; i ++)
				circle(x + size - size /4, y + size /2, i);
			break;
	}		
}

void piecedraw(int x, int y, int radius, int color)
{
	int i;
	setcolor(color);
	//Draw the inner colored portion
	for (i = 0; i <= radius; i++)
		circle(x, y, i);
	
	//Draw the outer black boundary
	setcolor(BLACK);
	circle(x,y,radius);
	setfillstyle(SOLID_FILL, color);
	floodfill(x,y,BLACK);	
}

void draw_button(int left, int top, int right, int bottom, int color, int shade1, int shade2, int txtcolor, char *text, int* up, int* down)
{
	int j;
	settextjustify(CENTER_TEXT, CENTER_TEXT);

	//down button image
	setfillstyle(SOLID_FILL, color);
	bar(left, top, right, bottom);
	
	for (j = 0; j < 4; j++) 
	{
		setcolor(shade2);
		line(left + j, top + j, right - j, top + j);
		line(left + j, top + j, left + j, bottom - j);
		
		setcolor(shade1);
		line(left + j, bottom - j, right - j, bottom - j);
		line(right - j, top + j, right - j, bottom - j);

	}

	setcolor(txtcolor);
	outtextxy((left + right) / 2 + 2, (top + bottom) / 2 + 2, text);

	getimage(left, top, right, bottom, down);
	
	//Up Image
		
	setfillstyle(SOLID_FILL, color);
	bar(left, top, right, bottom);

	for (j = 0; j < 4; j++) 
	{
		setcolor(shade1);
		line(left + j, top + j, right - j, top + j);
		setcolor(shade1);
		line(left + j, top + j, left + j, bottom - j);

		setcolor(shade2);
		line(left + j, bottom - j, right - j, bottom - j);
		setcolor(shade2);
		line(right - j, top + j, right - j, bottom - j);
	}
	setcolor(txtcolor);

	outtextxy((left + right) / 2, (top + bottom) / 2, text);

	getimage(left, top, right, bottom, up);

}

