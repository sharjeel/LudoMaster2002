/*********************************************************************/
/* C mouse library MOUSE.C for MS-DOS                                */
/* Jasper Bedaux, 1997                                               */
/* http://www.science.uva.nl/~bedaux/          bedaux@science.uva.nl */
/*********************************************************************/

#include <dos.h>
#include "mouse.h"

#define MOUSE 0x33         /* mouse driver interrupt number */
#define TRUE 1
#define FALSE 0

static int mouse_visible;
static union REGS inregs, outregs;
static struct SREGS segregs;

static const int cursor_data[] = {
	/* standard arrow cursor */
	1, 0,                            /* x, y of hotpoint                    */

	0x9FFF, 0x8FFF, 0x87FF, 0x83FF,  /* SCREEN MASK (16 words)              */
	0x81FF, 0x80FF, 0x807F, 0x803F,  /* Each word marks 16 pixels (lowest   */
	0x801F, 0x800F, 0x80FF, 0x887F,  /* bit is rightmost pixel). The mask   */
	0x987F, 0xBC3F, 0xFC3F, 0xFE7F,  /* is AND_ed with screen contents.     */

	0x0000, 0x2000, 0x3000, 0x3800,  /* CURSOR MASK (16 words)              */
	0x3C00, 0x3E00, 0x3F00, 0x3F80,  /* After the screen is AND_ed with the */
	0x3FC0, 0x3E00, 0x3600, 0x2300,  /* screen mask it is XOR_ed with this  */
	0x0300, 0x0180, 0x0180, 0x0000,  /* cursor mask.                        */

	/* pointing hand cursor */
	2, 1,
	0xCFFF, 0x87FF, 0x87FF, 0x87FF, 0x813F, 0x8007, 0x8003, 0x8003,
	0x8003, 0x8003, 0x8003, 0x8003, 0x8007, 0xC007, 0xC007, 0xE03F,

	0x0000, 0x3000, 0x3000, 0x3000, 0x3000, 0x36C0, 0x36D8, 0x36D8,
	0x36D8, 0x36D8, 0x3FF8, 0x3FF8, 0x3FF0, 0x1FF0, 0x1FC0, 0x0000,

	/* hourglass cursor */
	7, 7,
	0xC003, 0xC003, 0xC003, 0xC003, 0xE007, 0xF00F, 0xF81F, 0xFC3F,
	0xFC3F, 0xF81F, 0xF00F, 0xE007, 0xC003, 0xC003, 0xC003, 0xC003,

	0x0000, 0x1FF8, 0x0000, 0x1FF8, 0x0D50, 0x06A0, 0x0340, 0x0180,
	0x0180, 0x03C0, 0x07E0, 0x0D50, 0x1AA8, 0x0000, 0x1FF8, 0x0000,

	/* pen cursor */
	0, 15,
	0xFFF1, 0xFFE0, 0xFFC0, 0xFF80, 0xFF01, 0xFE03, 0xFC07, 0xF80F,
	0xF01F, 0xE03F, 0xE07F, 0xC0FF, 0xC1FF, 0x87FF, 0x1FFF, 0x3FFF,

	0x0000, 0x000E, 0x001C, 0x003A, 0x0074, 0x00E8, 0x01D0, 0x03A0,
	0x0740, 0x0E80, 0x0D00, 0x1A00, 0x1800, 0x2000, 0x4000, 0x0000

	/* You can just add here more cursors if you like. The only thing to */
	/* do is add its name in enum cursor_types in MOUSE.H.               */
};

void set_cursor(cursor_type cursor) { /* types defined in header */
	inregs.x.ax = 9;
	inregs.x.bx = *(cursor_data + 34 * cursor);      /* x hotpoint */
	inregs.x.cx = *(cursor_data + 34 * cursor + 1);    /* y hotpoint */
	inregs.x.dx = FP_OFF(cursor_data + 34 * cursor + 2); /* pointer to */
	segregs.es = FP_SEG(cursor_data + 34 * cursor + 2);  /* the masks  */
	int86x(MOUSE, &inregs, &outregs, &segregs);
}

int init_mouse(void) {     /* first function to call */
	int number_of_buttons;
	inregs.x.ax = 0;              /* reset mouse driver */
	int86(MOUSE, &inregs, &outregs);
	if (outregs.x.bx == 2 || inregs.x.bx == 0xFFFF) {   
		number_of_buttons = 2;
	}
	else if (outregs.x.bx == 3) {
		number_of_buttons = 3;
	}
	else {
		number_of_buttons = -1; /* unknown result */
	}
	if (!outregs.x.ax) {
		number_of_buttons = 0;  /* no mouse/driver installed */
	}
	mouse_visible = FALSE;    /* default after reset */
	set_cursor(ARROW);
	show_mouse();
	set_sensitivity(8, 8);
	return number_of_buttons;
}

void show_mouse(void) {
	inregs.x.ax = 1;
	int86(MOUSE, &inregs, &outregs);
	mouse_visible = TRUE;
}

void hide_mouse(void) {
	if (mouse_visible) {   /* Check needed because multiple calls to */
		inregs.x.ax = 2;   /* hide_mouse() would require multiple calls to */
		int86(MOUSE, &inregs, &outregs); /* show_mouse(). Not the case for */
		mouse_visible = FALSE; /* show_mouse() (of course, because show_ */
	}                       /* mouse() need to be called more than once  */
}                       /* for a region which is set for updating).  */

void set_mouse_position(int x, int y) {
	inregs.x.ax = 4;
	inregs.x.cx = x;
	inregs.x.dx = y;
	int86(MOUSE, &inregs, &outregs);
}

void set_sensitivity(unsigned int x, unsigned int y) {
	inregs.x.ax = 0xF;
	inregs.x.cx = x; /* number of mouse motions to horizontal 8 pixels */
	inregs.x.dx = y; /* number of mouse motions to vertical 8 pixels */
	int86(MOUSE, &inregs, &outregs);
}

void set_handler(void far interrupt (*handler)(), unsigned int event) {
	inregs.x.ax = 0xC; 
	inregs.x.cx = event;     /* event masks defined in MOUSE.H */
	inregs.x.dx = FP_SEG(handler);
	segregs.es = FP_OFF(handler);
	int86x(MOUSE, &inregs, &outregs, &segregs);
}

void set_update_region(int left, int top, int right, int bottom) {
	/* Initially mouse gets hidden when it comes in the specified region  */
	/* but after one call to show_mouse() (after you have drawn something */
	/* on the screen) it becomes visible again. When you draw something   */
	/* on top of the mouse cursor without setting the update region the   */
	/* cursor will not become visible after a call to show_mouse().       */
	/* (That's why this function exists.)                                 */
	inregs.x.ax = 0x10;
	inregs.x.cx = left;
	inregs.x.dx = top;
	inregs.x.si = right;
	inregs.x.di = bottom;
	int86(MOUSE, &inregs, &outregs);
}

void mouse_active_page(unsigned int page) { /* set active graphics page */
	inregs.x.ax = 0x1D;
	inregs.x.bx = page;
	int86(MOUSE, &inregs, &outregs);
}

void read_mouse(mouse_data *mouse) {
	inregs.x.ax = 3;
	int86(MOUSE, &inregs, &outregs);
	mouse->x = outregs.x.cx;
	mouse->y = outregs.x.dx;
	mouse->left_down = outregs.x.bx & 1;    /* bit 1 is left button  */
	mouse->right_down = (outregs.x.bx & 2) >> 1;      /* bit 2 right */
	mouse->middle_down = (outregs.x.bx & 4) >> 2;    /* bit 3 middle */
}

int button_press(button_type button, mouse_data *mouse) {
	inregs.x.ax = 5;
	inregs.x.bx = button;
	int86(MOUSE, &inregs, &outregs);
	mouse->left_down = outregs.x.ax & 1;
	mouse->right_down = (outregs.x.ax & 2) >> 1;
	mouse->middle_down = (outregs.x.ax & 4) >> 2;
	mouse->x = outregs.x.cx;    /* position button was last pressed */
	mouse->y = outregs.x.dx;
	return outregs.x.bx;  /* number of presses since last call */
}

int button_release(button_type button, mouse_data *mouse) {
	inregs.x.ax = 6;
	inregs.x.bx = button;
	int86(MOUSE, &inregs, &outregs);
	mouse->left_down = outregs.x.ax & 1;
	mouse->right_down = (outregs.x.ax & 2) >> 1;
	mouse->middle_down = (outregs.x.ax & 4) >> 2;
	mouse->x = outregs.x.cx;    /* position button was last released */
	mouse->y = outregs.x.dx;
	return outregs.x.bx; /* number of releases since last call */
}
