#ifndef __GAME_FUNCTIONS_H_
#define __GAME_FUNCTIONS_H_

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>
#include "structs.h"

void		initGame(game_t *game);
char		*concat(char *str1, char *str2, bool free1, bool free2);
void		launchGame(game_t *game);
void		manageEvents(game_t *game);
void		image(game_t *game, sfSprite *sprite, int x, int y, sfVector2f scale);
void		mainMenu(game_t *game);
Array		loadMusics();
Array		loadSprites();
Array		loadSfx();
Settings	loadSettings();
void		saveSettings(game_t *game);
char		*getButtonContent(int nameId, game_t *game);
Button		*loadButtons(game_t *game);
void		disableButtons(game_t *game);
void		disp_buttons(game_t *game);
void		play_button(game_t *game, int buttonId);

#endif
