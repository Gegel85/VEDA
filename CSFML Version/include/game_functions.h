#include "functions.h"

void	(* const game_functions[])(game_t *game) = {
	&mainMenu,
	&inGame,
};