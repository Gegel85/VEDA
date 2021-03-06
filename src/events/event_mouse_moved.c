#include <structs.h>
#include <utils.h>
#include <stdlib.h>

void	manageMouseMoved(sfEvent *event)
{
	if (game.state.menuSelected == 1 && game.state.menu == 3) {
		if (event->mouseMove.x <= 140)
			game.settings.sfxVolume = 0;
		else if (event->mouseMove.x >= 440)
			game.settings.sfxVolume = 100;
		else
			game.settings.sfxVolume = (event->mouseMove.x - 140) / 3;
		if (getSoundEffect(DIRT1 + random() % 4))
			playSoundEffect(getSoundEffect(DIRT1 + random() % 4));
	} else if (game.state.menuSelected == 2 && game.state.menu == 3) {
		if (event->mouseMove.x <= 140)
			game.settings.musicVolume = 0;
		else if (event->mouseMove.x >= 440)
			game.settings.musicVolume = 100;
		else
			game.settings.musicVolume = (event->mouseMove.x - 140) / 3;
		setVolumes(game.resources.musics, game.settings.musicVolume);
	}
}