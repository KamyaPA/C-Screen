#include "animation.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#define GET_ANIMATION_FROM_LIST(LIST) ((Animation *)LIST->item)
#define ANIMATION_BUFFER_SIZE 20

	List * animations;

	int animation_init(void){
		animations = (List *) malloc (sizeof(List));
		*animations = list_init();
		if(animations) return 1;
		else return 0;
	}

	void __set_update_frequency( Animation * animation, int update_frequency) {
			
		animation->update_frequency = update_frequency > 0 ? update_frequency : 1;

		animation->target_update = screen_get_nframes() % animation->update_frequency;
	}

	Animation * animation_create(Window * window, int loop, int update_frequency){
		Animation * new = (Animation *) malloc (sizeof(Animation));

		new->animation_start = (List *) malloc (sizeof(List));
		*(new->animation_start) = list_init();
		new->animation_frame = new->animation_start;

		new->loop = loop;
		__set_update_frequency(new, update_frequency);

		new->animated_window = window;
		return new;
	}

	int __load_animation_insert_argument(char argument, List ** temp_list, int counter){
		/* Is the list longer than the bugger size */
		if(counter % ANIMATION_BUFFER_SIZE == ANIMATION_BUFFER_SIZE - 1 && counter > 0 ){
			list_add(*temp_list, malloc(ANIMATION_BUFFER_SIZE));
			*temp_list = (*temp_list)->next;
		}
		/* Insert Char */
		((char *)(*temp_list)->item)[counter % ANIMATION_BUFFER_SIZE] = argument;
		return counter + 1;
	}

	int __load_animation_mode_move(va_list args, List ** temp_list, int counter){
		int i;
		char argument;
		for(i = 0; i < 2; i++){
			argument = va_arg(args, int);
			counter = __load_animation_insert_argument(argument, temp_list, counter);	
		}
		return counter;	
	}

	int __load_animation_mode_change(va_list args, List ** temp_list, int counter){
		char argument;
		int counter_start = counter;
		do{
			argument = va_arg(args, int);
			counter = __load_animation_insert_argument(argument, temp_list, counter);	
		} while(argument != 0 || counter - counter_start < 2);
		return counter;
	}

	int __load_animation_mode_change_str(va_list args, List ** temp_list, int counter){
		int i;
		char * arguments;
		char argument;
		for(i = 0; i < 2; i++){
			argument = va_arg(args, int);
			counter = __load_animation_insert_argument(argument, temp_list, counter);	
		}
		arguments = (char *) va_arg(args, void *);
		while(*arguments != '\0'){
			counter = __load_animation_insert_argument(*arguments, temp_list, counter);
			arguments++;
		}
		/*Copy null char */	
		return __load_animation_insert_argument(*arguments, temp_list, counter);
	}

	int __load_animation_mode_set_frequency(va_list args, List ** temp_list, int counter){
		return __load_animation_insert_argument(va_arg(args, int), temp_list, counter);	
	}


	int __load_animation_frame(va_list args, List * temp_list){
		int counter = 0;
		char mode;
		do{
			mode = va_arg(args, int);
			counter = __load_animation_insert_argument(mode, &temp_list, counter);
			switch (mode){
				case move:
					counter = __load_animation_mode_move(args, &temp_list, counter); break;
				case change:
					counter = __load_animation_mode_change(args, &temp_list, counter); break;
				case change_str:
					counter = __load_animation_mode_change_str(args, &temp_list, counter); break;
				case set_frequency: 
				case step_frequency:
					counter = __load_animation_mode_set_frequency(args, &temp_list, counter); break;
			}	
		} while(mode != end);
		return counter;
	}

	List *animation_add_frame(Animation * animation, ...){
		int loop_count = 0;
		char * animation_str;
		List * temp_str_holder = (List *) malloc (sizeof(List)); 
		int animation_str_size_count;

		va_list args;
		va_start(args, animation);

		*temp_str_holder = list_init();
		list_add(temp_str_holder, malloc(ANIMATION_BUFFER_SIZE));	
		
		animation_str_size_count = __load_animation_frame(args, temp_str_holder);
			
		va_end(args);
		
		animation_str = (char *) malloc (animation_str_size_count);	
		while(animation_str_size_count > 0){
			int i;
			/* How many valid chars are in buffer */
			int check = 
				animation_str_size_count > ANIMATION_BUFFER_SIZE ? 
				ANIMATION_BUFFER_SIZE                            : 
				animation_str_size_count % ANIMATION_BUFFER_SIZE ;
			
			/* Copy buffer */
			for(i = 0; i < check; i++){
				animation_str[ANIMATION_BUFFER_SIZE * loop_count + i] = ((char *)temp_str_holder->item)[i];	
			}

			/* Delete list */
			if(temp_str_holder->next == NULL){ 
				list_remove(temp_str_holder);
			}
			else {
				temp_str_holder = temp_str_holder->next;
				list_remove(temp_str_holder->before);	
			}

			/*Do Counters*/
			animation_str_size_count -= ANIMATION_BUFFER_SIZE;
			loop_count++;
		}

		return list_add(animation->animation_start, animation_str);
	}
	/* Adds an animation to the animations list */
	void animation_add_to_cycle(Animation * animation) {
		list_add(animations, animation);
	}


	/* Does the next frame */
	void animation_do_frame(Animation * animation) {
		int i;
		char * animation_frame_str;

		if (animation->animation_frame == NULL) return;
		
		animation_frame_str = (char *) animation->animation_frame->item;

		if (animation->loop && animation->animation_frame->next == NULL)
			animation->animation_frame = animation->animation_start;
		else
			animation->animation_frame = animation->animation_frame->next;

		/* Returns on an encounter of a null char*/
		for(i = 0;; i++ ){
			switch (animation_frame_str[i]){
				case end:
					return;
				case move:
					i++;
					animation->animated_window->start.x += animation_frame_str[i];
					i++;
					animation->animated_window->start.y += animation_frame_str[i];
					break;
				case change: case change_str:
					window_write(animation->animated_window, animation_frame_str + i + 3, animation_frame_str[i + 1], animation_frame_str[i + 2]);
					i += 3 + strlen(animation_frame_str + i + 3);
					break;
				case set_frequency:
					i++;
					__set_update_frequency(animation, animation_frame_str[i]);
					break;	
				case step_frequency:
					i++;
					__set_update_frequency(animation, animation->update_frequency + animation_frame_str[i]);
					break;	
			}	
		}
	}

	void __cycle_animations(List * animation_list){
		if(animation_list == NULL || animation_list->item == NULL) return;
		__cycle_animations(animation_list->next);
		if(    screen_get_nframes() 
			%  GET_ANIMATION_FROM_LIST(animation_list)->update_frequency 
			== GET_ANIMATION_FROM_LIST(animation_list)->target_update 
		  ) {
			animation_do_frame( GET_ANIMATION_FROM_LIST(animation_list) );
		}
	}

	void animation_cycle(void){
		__cycle_animations(animations);
	}

	void animation_delete(Animation * animation){
		list_delete(animation->animation_start);
		list_remove(animation->attachment); 
	}

	void __term_animation_list(List * animation_list){
		if(animation_list == NULL) return;
		__term_animation_list(animation_list->next);
		animation_delete((Animation *) animation_list->item);
		free(animation_list);
	}

	void animation_term(){
		__term_animation_list(animations);
	}

	void animation_next_total_frame(){
		static long time_before = 0;
		struct timespec rem;
		struct timespec req = {0,0};
		
		animation_cycle();
		screen_update();
		time_before = (int)(clock()/CLOCKS_PER_SEC) * 1000000000L - time_before;

		req.tv_nsec = MS_PR_FRAME * 1000000L - time_before;
		while(nanosleep(&req, &rem ) == -1);
	}
