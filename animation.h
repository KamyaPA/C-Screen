#include "screen.h"
#define  MS_PR_FRAME 200


/* = The animation frame encoding  = * * * * * * * * * * * * * * *\
 *                                                               *
 * 0                , end frame                                  *
 * 1, x, y          , move window (x,y) spaces                   *
 * 2, x, y, "...\0" , replace from (x, y) with chars form ...\0  *
 * 3, s,			, set new update_frequency                   *
 *                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum frame_encoding {
	end,
	move,
	change,
	change_str,
	set_frequency,
	step_frequency,
};

/* The structure for an animation*/
typedef struct Animation {
	int update_frequency;		/* How often the animation updates in ms*/
	int target_update;			/* The target dependent on when the animation was spawned */
	int loop;					/* 0: Non looping; 1: Looping animation */
	Window * animated_window;	/* Target window */
	List * animation_start;		/* Each list item has a char array conforming to the animation frame encoding */
	List * animation_frame;     /* The currant frame of animation */
	List * attachment;			/* Where in the attachment list it is located*/
} Animation;

int animation_init(void);
Animation * animation_create(Window * window, int loop, int update_frequency);

List *  animation_add_frame(Animation * animation, ...);
void animation_add_to_cycle(Animation * animation);
void animation_cycle(void);
void animation_do_frame(Animation * animation);
void animation_next_total_frame();

void animation_remove();
void animation_delete(Animation * animation);
void animation_term();
