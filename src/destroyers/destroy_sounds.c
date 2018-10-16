#include <SFML/Audio.h>
#include <stdio.h>
#include <malloc.h>
#include "structs.h"

void	destroySounds()
{

	for (int i = 0; i < game.resources.musics.length; i++) {
		if (game.debug)
			printf("%s: Destroying music %i\n", INFO_BEG, i);
		if (((sfMusic **)game.resources.musics.content)[i] != NULL)
			sfMusic_destroy(((sfMusic **)game.resources.musics.content)[i]);
	}
	free(game.resources.musics.content);
	for (int i = 0; i < game.resources.sfx.length; i++) {
		if (game.debug)
			printf("%s: Destroying sfx %i\n", INFO_BEG, i);
		if (((sfMusic **)game.resources.sfx.content)[i] != NULL)
			sfMusic_destroy(((sfMusic **)game.resources.sfx.content)[i]);
	}
	free(game.resources.sfx.content);
}
