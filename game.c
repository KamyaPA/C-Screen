/* For game specific elements */
#include "animation.h"
#include <time.h>

int game_start (void)  {
    Window *border_top    = window_init(0               , 0                , SCREEN_WIDTH, 1               );
	Window *border_bottom = window_init(0               , SCREEN_HIGHT - 1 , SCREEN_WIDTH, 1               );	
	Window *border_left	  = window_init(0               , 1                , 2           , SCREEN_HIGHT - 2);
	Window *border_right  = window_init(SCREEN_WIDTH - 2, 1                , 2           , SCREEN_HIGHT - 2);
		
	window_write(border_top   , "/////////////////////////////////////\255\032///////////////////////////////////////////////////////////////", 0, 0);
	window_write(border_bottom, "////////////////////////////////////////////////////////////////////////////////////////////////////", 0, 0);
	window_write(border_left  , "//////////////////////////////////////////////////////////////////////////////////", 0, 0);
	window_write(border_right , "//////////////////////////////////////////////////////////////////////////////////", 0, 0);
	
	Window *animated	  = window_init(6, 6, 3, 1);
	Animation *animation  = animation_create(animated, 1, 1);
	animation_add_frame(animation, change, 1, 0,  '|', end, end);	
	animation_add_frame(animation, change, 1, 0,  '/', end, end);	
	animation_add_frame(animation, change, 0, 0, '-', '-', '-',  end, end);	
	animation_add_frame(animation, change_str, 0, 0,  " \\ ",  end);	
	animation_add_to_cycle(animation);

	
	
	return 0;
}

int game_loop (void) {
	for(;;){
		animation_next_total_frame();
	}
	return 0;
}

int game_end (void) {
	return 0;
}

