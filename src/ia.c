#include <stdlib.h>
#include <stdint.h>

#include "config.h"
#include "tools.h"

/*** Description of a game stored on 64 bits  
  **
  **	For each round:
  **		- 2 bits for the opponent choice
  **		- 3 bits for the  opponent guess of sum
  **		- 2 bit for the IA choice
  **		- 3 bits for the IA guess of sum
  **
  **		 9  |  8  | 7|6|5  |  4  |  3  | 2|1|0
  **		choice[0] | sum[0] | choice[1] | sum[1]
  **
  **	General information
  **		- 1 bit for the starter
  **		- 1 bit for the winner
  **
  **	
  ** 	
  ***/

#define ROUND_LENGTH	10

uint64_t init_game(uint8_t starter) {

	uint64_t gint = 0;
	
	/* Adding the starter */
	gint |= (starter & 0x01) << 1;

	return gint;
}


uint64_t save_round(uint64_t gint, uint8_t round, uint8_t choice[2], uint8_t guess[2]) {
	
	uint32_t new_round = 0;	

	new_round |= (choice[1 - IA_ID] & 0x03) << 8;
	new_round |= (guess[1 - IA_ID] & 0x07) << 5;

	new_round |= (choice[IA_ID] & 0x03) << 3;
	new_round |= (guess[IA_ID] & 0x07);

	return gint;
}

uint8_t *compute_nb_left(uint64_t *gint) {

	uint8_t *left = malloc(2*sizeof(uint8_t));

	left[0] = 3;
	left[1] = 3;

	uint8_t round_id = 1;
	uint16_t round = (*gint >> (64 - ROUND_LENGTH * round_id)) & 0x1FF;

	while (round != 0 && round_id < 6) {
		uint8_t sum = (round >> 8) & 0x3 + (round >> 3) & 0x03; 
		if (abso((round & 0x7) - sum) < abso((round >> 5) - sum)) {
			left[0]--;
		} else {
			left[1]--;
		}
		round_id++;
		round = (*gint >> (64 - ROUND_LENGTH * round_id)) & 0x1FF;
	}
	return left;
}


uint64_t end_game(uint64_t gint, uint8_t winner) {

	gint |= winner & 0x01;
	return gint;

}
