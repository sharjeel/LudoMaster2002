/*********************************************************************/
/* Include file for C library MOUSE.C for MS-DOS                     */
/* Jasper Bedaux, 1997                                               */
/* http://www.science.uva.nl/~bedaux/          bedaux@science.uva.nl */
/*********************************************************************/

#ifndef __MOUSE_H     /* check if header was already included */
#define __MOUSE_H

typedef enum {ARROW, HAND, HOUR_GLASS, PEN} cursor_type;

typedef struct {
	int x;
	int y; 
	int left_down;
	int right_down;
	int middle_down;
} mouse_data;

typedef enum {LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON} button_type;

/* Events (bit masks) as argument for functions which set      */
/* interrupt handler. Events can be OR_ed together e.g.        */
/* set_handler(event_handler, L_BUTTON_PRESS | R_BUTTON_PRESS) */
/* yields that event_handler() is called when either the left  */
/* or the right button is pressed down.                        */
#define MOUSE_MOVES  	  1
#define L_BUTTON_PRESS    2
#define L_BUTTON_RELEASE  4
#define R_BUTTON_PRESS    8
#define R_BUTTON_RELEASE 16
#define M_BUTTON_PRESS   32
#define M_BUTTON_RELEASE 64

int init_mouse(void);      /* first function to call: resets mouse driver
	and returns number of buttons or 0 if mouse/driver not installed  */

void show_mouse(void);
void hide_mouse(void);

void set_cursor(cursor_type cursor);  /* cursor types defined above */

void set_mouse_position(int x, int y);

void set_sensitivity(unsigned int x, unsigned int y);
	/* number of 'mickeys' for 8 pixels on screen */		
	/* a mickey is the smallest detectable mouse movement */

void set_handler(void far interrupt (*handler)(), unsigned int event);
/****************************************************************************
 * The interrupt handler gets the following values via the CPU registers:   *
 * AX = event mask (as set by set_handler)                                  *
 * BX = button state (bit 0 LEFT, bit 1 RIGHT, bit 3 MIDDLE)                *
 * CX = mouse cursor x position                                             *
 * DX = mouse cursor y position                                             *
 * DI = number of 'mickeys' mouse has moved horizontal since last call      *
 *             positive values indicate a movement to the right             *
 * SI = number of 'mickeys' mouse has moved vertical since last call        *
 *             positive values indicate a down movement                     *
 ****************************************************************************/

void set_update_region(int left, int top, int right, int bottom);
	/* Initially mouse gets hidden when it reaches the specified region   */
	/* but after one call to show_mouse() (after you have drawn something */
	/* on the screen) it becomes visible again. When you draw something   */
	/* on top of the mouse cursor without setting the update region the   */
	/* cursor will not become visible after a call to show_mouse().       */
	/* (That's why this function exists.)                                 */ 

void read_mouse(mouse_data *mouse);
/*************************************************************************
 * after a call to this function you can use the following values:       *
 * mouse.x: x position of mouse cursor when read_mouse was called        *
 * mouse.y: y position                                                   *
 * mouse.left_down is TRUE (1) if left button was down during call       *
 * mouse.right_down TRUE if right button is down                         *
 * mouse.middle_down TRUE if middle button is down                       *
 *************************************************************************/

int button_press(button_type button, mouse_data *mouse);
	/* Returns number of button presses since last call to mouse and       */
	/* the elements of struct mouse get the value which they had at the    */
	/* moment the specified button was last pressed down.                  */

int button_release(button_type button, mouse_data *mouse);
	/* same as mouse_press but now for last button release(s) */

void mouse_active_page(unsigned int page);
	/* set active graphics page for mouse cursor */

#endif
