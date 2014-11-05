#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "rules.h"
#include "tools.h"

#define DEBUG	0
/*
	Descrption of game coding (bit 0 means least significant bit):
	bit 0: is the game over ?
	bit 1: who started the game ? (P0 or P1 ?)
*/

uint8_t ask_number(game_t *game) {
	char n;
	do 
	{
		if (VERBOSE) {
			if (NICE) {
				printf("[%s] Might I ask you for your choice (max %u)? ", 
					game->pname, game->players[1-IA_ID].number);
			} else {
				printf("[CHOICE] ");
			}
			scanf("%u", &n);
		}
	} while (n > 9);
	return n;
}

uint8_t ask_sum(game_t *game) {
	char n;
	if (VERBOSE) {
		if (NICE) {
			printf("[%s] What is your guess for the sum? ", game->pname);
		} else {
			printf("[SUM] ");
		}
	}
	scanf("%u", &n);
	return n;
}

game_t *new_game(uint8_t starter, char *name) {
	game_t *game = malloc(sizeof(game_t));
	game->pname = name;
	game->starter = starter;
	game->round = 0;
	game->over = false;

	size_t i;
	for (i = 0; i < 2; i++) {
		game->players[i].number = 3;
	}

	return game;
}


bool game_over(game_t *game) {
	return game->over;
}


bool verify_choice(game_t *game) {
	return (game->players[1-IA_ID].number_picked <= 
			game->players[1-IA_ID].number);
} 

uint8_t who_is_the_best(game_t *game) {
	int8_t sum = game->players[0].number_picked + 
				game->players[1].number_picked;
	int8_t guess[2] = { (int8_t) game->players[0].sum_guess, 
				(int8_t) game->players[1].sum_guess }; 

#if DEBUG
	printf("[DEBUG] sum: %i ; guesses: %i ; %i ; errors: %i ; %i\n", sum, guess[0], guess[1], abso(guess[0] - sum), abso(guess[1] - sum));
#endif

	if (abso(guess[0] - sum) < abso(guess[1] - sum)) {
		return 0;
	} else {
		return 1;
	}
}

void new_round(game_t *game) {
	uint8_t pId = (game->starter + game->round) % 2;

	if (pId != IA_ID) {
		/* Ask number of the IA */
		/* TODO */
#if IA
		game->players[IA_ID].number_picked = 0;
#else
		game->players[IA_ID].number_picked = 0;
#endif
		if (VERBOSE && NICE) 
			printf("Okay, I've made my choice.\n");
	}

	/* Ask number of the player */
	do {
		game->players[1 - IA_ID].number_picked = ask_number(game);
	} while (!verify_choice(game));
	
	
	if (pId == IA_ID) {
		/* Ask number of the IA */
		/* TODO */
#if IA
		game->players[IA_ID].number_picked = 0;
#else
		game->players[IA_ID].number_picked = 0;
#endif
		if (VERBOSE && NICE) 
			printf("Okay, I've made my choice.\n");
	}
	
	/* At this point, we have the two choices */

	if (pId == IA_ID) {
		/* TODO */
#if IA
		game->players[IA_ID].sum_guess = 1;
#else
		game->players[IA_ID].sum_guess = 1;
#endif
		if (VERBOSE) {
			if (NICE)
				printf("Hmmm ... I think we have %u stones.\n", game->players[IA_ID].sum_guess);
			else
				printf("[IA_GUESS] %u\n", game->players[IA_ID].sum_guess);
		}

		uint8_t choice = 255;
		do {
			if (choice != 255 && VERBOSE && NICE) 
				printf("I am sorry, I already chose this number ... Would you mind chosing another number?\n");
			choice = ask_sum(game);
		} while (choice == game->players[IA_ID].sum_guess);
		game->players[1 - IA_ID].sum_guess = choice;
	} else {
		uint8_t choice = ask_sum(game);
		game->players[1 - IA_ID].sum_guess = choice;
		/* TODO */
		game->players[IA_ID].sum_guess = 1;
		if (VERBOSE) {
			if (NICE)
				printf("Hmmm ... I think we have %u stones.\n", game->players[IA_ID].sum_guess);
			else
				printf("[IA_GUESS] %u\n", game->players[IA_ID].sum_guess);
		}
	}

	/* Determine the winner */
	uint8_t winner = who_is_the_best(game);

	uint8_t sum = game->players[0].number_picked + game->players[1].number_picked;
	if (VERBOSE) {
		if (NICE)
			printf("Result: there were %u stones.", sum);
		else
			printf("[RESULT] %u\n", sum);
	}

	if (VERBOSE) {
		if (winner == IA_ID) {
			if (NICE) {
				printf("I am closer, sorry %s ...\n\n", game->pname);
			} else {
				printf("[IA WINS]\n");
			}
		} else { 
			if (NICE) {
				printf("Congrat's, you won this round.\n\n");
			} else {
				printf("[YOU WIN]\n");
			}
		}
	}

	game->players[1-winner].number--;

	game->round++;
	game->over = (game->players[0].number == 0 || game->players[1].number == 0);
}


