/* This is for screen control */

#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "screen.h"

#define POS_ON_SCREEN(X, Y) Y * SCREEN_WIDTH + X
#define POS_IN_WINDOW(X, Y, WIN) Y * WIN->scale.x + X
#define MAX_COLORS 8

void __screen_mkimage (char * screen, char * color);
void __write_to_screen(Window* window, char* screen, char * color);

List * windows;
int nframes;

int screen_init (void) {
	int i, j;
	/* Ncurses screen */
	windows  = (List *) malloc (sizeof(List));
	*windows = list_init();
	nframes = 0;
	initscr();

	/* Color */
	if(!has_colors()){
		screen_term();
		printf("Terminal does not support color\n");
		exit(1);
	}
	start_color();
	for(i = 0; i < MAX_COLORS; i++){
		for(j = 0; j < MAX_COLORS; j++){
			init_pair(i * MAX_COLORS + j, j, i);
		}
	}
	return 1 ;
}

void screen_update(){
	int y, x;
	static int displace_y_before;
	static int displace_x_before;
	const int displace_y = (getmaxy(stdscr) - SCREEN_HIGHT) / 2;
	const int displace_x = (getmaxx(stdscr) - SCREEN_WIDTH) / 2;
	char * screen = calloc (SCREEN_HIGHT, SCREEN_WIDTH);
	char * color  = calloc (SCREEN_HIGHT, SCREEN_WIDTH);
	char active_char;
	
	/* Has the view port moved, if true clear it */
	if(displace_x_before != displace_x || displace_y_before != displace_y){
		clear();
		displace_x_before = displace_x;
		displace_y_before = displace_y;
	}
	
	/* Load screen buffer */
	__screen_mkimage(screen, color);
	for(x = 0; x < SCREEN_WIDTH; x++){
		for( y = 0; y < SCREEN_HIGHT; y++){
			active_char = screen[POS_ON_SCREEN(x, y)];
			COLOR_PAIR(color[POS_ON_SCREEN(x, y)]);
			mvaddch(y + displace_y, x + displace_x, active_char == '\0' ? ' ' : active_char);
		}
	}

	mvprintw(0,0,"%d",nframes);

	free(screen);
	
	/* Update screen */
	nframes++;
	refresh();
}

int screen_term (void) {
	endwin();
	return 1;
}

int screen_get_nframes(void){
	return nframes;
}


Window *window_init(int start_x, int start_y, int scale_x, int scale_y) {
	Window *new = (Window *) malloc (sizeof(Window));
	new->start.x = start_x;
	new->start.y = start_y;
	new->scale.x = scale_x;
	new->scale.y = scale_y;
	new->picture = calloc(scale_x, scale_y);
	new->color   = calloc(scale_x, scale_y);
	new->list_attachment = list_add(windows, (void *) new);
	return new;
}

/* Writes the string to a window to the (x, y) coordinate, on a char value of '\t' its transparent */
void window_write(Window * window, char * string, int start_x, int start_y) {
	char color = 072;
	int pos = POS_IN_WINDOW(start_x, start_y, window);
	int max = POS_IN_WINDOW(window->scale.x, window->scale.y, window);
	int i = 0;
	while (pos != max && string[i] != '\0') {
		if(string[i] == '\n') return window_write(window, string + i + 1, start_x, start_y + 1);
		if(string[i] == 1   ) window->picture[pos] = '\0';
		else if(string[i] == '\255' ){color = string[++i]; goto skip_color;}
		else window->picture[pos] = string[i];
		window->color[pos] = color;
		pos++;
		skip_color:
		i++;
	}	
}

void window_fill(Window * window, char * fillament) {
	int length = strlen(fillament);
	int max = POS_IN_WINDOW(window->scale.x, window->scale.y, window);
	int i;
	for(i = 0; i < max; i++) {
		window->picture[i] = fillament[i % length];	
	}
}

void window_close(Window *window) {
	list_remove(window->list_attachment);
	free(window->picture);
	free(window->color);
	free(window);
}

void __write_to_screen(Window* window, char* screen, char * color){
	int i, j, x, y;
	for(i = 0; i < window->scale.x; i++){
		x = window->start.x + i;
		if (x >= SCREEN_WIDTH) continue;
		for (j = 0; j < window->scale.y; j++){
			y = window->start.y + j;
			if (!(y >= SCREEN_HIGHT || window->picture[POS_IN_WINDOW(i, j, window)] == '\0')){
				color[POS_ON_SCREEN(x, y)] = window->color[POS_IN_WINDOW(i, j, window)];	
				screen[POS_ON_SCREEN(x, y)] = window->picture[POS_IN_WINDOW(i, j, window)];
			}
		}
	}
}

void __screen_mkimage (char * screen, char * color) {
	List * window_list = windows;
	do __write_to_screen((Window *)(window_list->item), screen, color);
	while ((window_list = window_list->next) != NULL);
}



