#include "list.h"

#define SCREEN_HIGHT 40
#define SCREEN_WIDTH 100

typedef struct Point {
	int x, y;
} Point;

typedef struct Window {
	List * list_attachment;
	Point start, scale;
	char * picture;
} Window;

int screen_init (void);
int screen_term (void);
void screen_update(void);

Window *window_init(int start_x, int start_y, int scale_x, int scale_y);
void window_write(Window * window, char * string, int start_x, int start_y);
void window_fill(Window * window, char * fillament);
void window_close(Window *window);

int screen_get_nframes(void);

