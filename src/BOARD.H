#include "vars.h"
#ifndef __board_h_
#define __board_h_
void drawboard(int startx, int starty, int blocksize);
void draw_block_pieces(int, int);
void drawblock(int, int);
int brd_pop(Board&);
void brd_push(Board&, int, int);
#endif