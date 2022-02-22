#include "log.h"
#include "animation.h"
#include "game.h"
#include <stdlib.h>

static int gamestate_init (void);
static int gamestate_term (void);

int main(){
	gamestate_init();

	game_start();
	game_loop();
	gamestate_term();
	return EXIT_SUCCESS;
}

/* Sets up the game */
static int gamestate_init (void) {
	if (!log_init()) return 0; 
	/* More setup code here */
	if (!screen_init()) return 0;
	if (!animation_init()) return 0;

	printlog(GAME_LOG, "Setup done\n");
	return 1;
}

/* Terminates the game state*/
static int gamestate_term (void) {
	printlog(GAME_LOG, "Ending game\n");

	log_term();
	screen_term();
	return 0;
}

