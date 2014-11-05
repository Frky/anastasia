#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "config.h"
#include "rules.h"


void main(int argc, char **argv) {

	uint8_t starter;

	/* Initialization of the random generator */
	// srand(gettimeofday());

	char name[20];

	VERBOSE = false;
	NICE = false;
	
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-v") == 0) {
			VERBOSE = true;
		} else if (strcmp(argv[i], "-vv") == 0) {
			VERBOSE = true;
			NICE = true;
		}
	}

	if (VERBOSE) {
		if (NICE)
			printf("Good morning stranger! What's your name? ");
		else
			printf("[USERNAME] ");
	}

	/* WARNING: buffer overflow possible here (mouahaha) */
	scanf("%s", name);

	if (VERBOSE) {
		if (NICE)
			printf("Nice to meet you %s! I am Anastasia.\n", name);
	}

	starter = (rand() < 0.5)?0:1;

	if (VERBOSE && NICE) {
		if (starter == IA_ID)
			printf("I will play first.\n");
		else
			printf("You will play first.\n");
	}

	game_t *game = new_game(starter, name);

	while (!game_over(game)) { 
		if (VERBOSE && !NICE) {
			printf("*** NEW ROUND ***\n");
		}
		new_round(game);
	}

	if (VERBOSE && NICE) {
		printf("It is over. You played well %s. Come back see me soon!\n", name);
		/* Machine learning powa */
#if LEARN
		learn_from_game(&game);
#endif
	}

}
