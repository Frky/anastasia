#ifndef __RULES_H__
#define __RULES_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint8_t number;
	uint8_t number_picked;
	uint8_t sum_guess;
} player_t;


typedef struct {
	char *pname;
	uint8_t starter;
	uint8_t round;
	player_t players[2];
	bool over;
} game_t;


game_t *new_game(uint8_t starter, char *name);
bool game_over(game_t *game);
void new_round(game_t *game);

#endif
