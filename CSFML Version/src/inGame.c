#include "structs.h"
#include "functions.h"
#include "concatf.h"
#include <stdlib.h>
#include <string.h>

void	displayLowerLayer(game_t *game)
{
	Object		*map = game->map;
	Array		sprites = game->sprites;
	sfVector2i	cam = game->cam;
	sfIntRect	rec = {0, 0, 16, 16};
	char		*buffer = NULL;

	for (int i = 0; map && map[i].layer; i++) {
		if (map[i].layer == 1 && map[i].pos.x + cam.x > -32 && map[i].pos.x + cam.x < 672 && map[i].pos.y + cam.y > -32 && map[i].pos.y + cam.y < 672) {
			if (!((Sprite *)sprites.content)[OBJECTS].sprite) {
				sfRectangleShape_setOutlineColor(game->rectangle, (sfColor){0, 0, 0, 0});
				sfRectangleShape_setFillColor(game->rectangle, (sfColor){((map[i].id * 10) >> 16) * 20 % 256, ((map[i].id * 10) >> 8) * 10 % 256, map[i].id * 10 % 256, 255});
				rect(game, map[i].pos.x + cam.x, map[i].pos.y + cam.y, 16, 16);
			} else {
				rec.left = (map[i].id - 1) * 16 % 640;
				rec.top = (map[i].id - 1) * 16 / 640;
				sfSprite_setTextureRect(((Sprite *)sprites.content)[OBJECTS].sprite, rec);
				image(game, ((Sprite *)sprites.content)[OBJECTS].sprite,  map[i].pos.x + cam.x, map[i].pos.y + cam.y, 16, 16);
			}
			if (game->debug) {
				if (game->text) {
					sfText_setCharacterSize(game->text, 15);
					sfText_setScale(game->text, game->baseScale);
					sfText_setColor(game->text, (sfColor){0, 0, 0, 255});
				}
				buffer = concatf("%i", map[i].id);
				text(buffer, game, map[i].pos.x + cam.x, map[i].pos.y + cam.y);
				free(buffer);
			}
		}
	}
}

void	displayCharacters(game_t *game)
{
	Player		player = game->player;
	sfIntRect	rec = {0, 0, 16, 32};
	Array		sprites = game->sprites;
	sfVector2i	cam = game->cam;

	rec.top = (player.position / 2 + (player.state * 2) + (player.animation * 2)) * 32;
	rec.left = (player.position % 2) * 32;
	sfSprite_setTextureRect(((Sprite *)sprites.content)[MALE_CHARACTER + player.isFemale].sprite, rec);
	image(game, ((Sprite *)sprites.content)[MALE_CHARACTER + player.isFemale].sprite,  player.pos.x + cam.x, player.pos.y + cam.y, 16, 32);
}

void	displayUpperLayer(game_t *game)
{
	Object		*map = game->map;
	Array		sprites = game->sprites;
	sfVector2i	cam = game->cam;
	sfIntRect	rec = {0, 0, 16, 16};
	bool		newLayer = true;
	char		*buffer = NULL;

	for (int layer = 2; newLayer; layer++) {
		newLayer = false;
		for (int i = 0; map && map[i].layer; i++) {
			if (map[i].layer == layer && map[i].pos.x + cam.x > -32 && map[i].pos.x + cam.x < 672 && map[i].pos.y + cam.y > -32 && map[i].pos.y + cam.y < 672) {
				if (!((Sprite *)sprites.content)[OBJECTS].sprite) {
					sfRectangleShape_setOutlineColor(game->rectangle, (sfColor){0, 0, 0, 0});
					sfRectangleShape_setFillColor(game->rectangle, (sfColor){((map[i].id * 10) >> 16) * 20 % 256, ((map[i].id * 10) >> 8) * 10 % 256, map[i].id * 10 % 256, 255});
					rect(game, map[i].pos.x + cam.x, map[i].pos.y + cam.y, 16, 16);
				} else {
					rec.left = (map[i].id - 1) * 16 % 640;
					rec.top = (map[i].id - 1) * 16 / 640;
					sfSprite_setTextureRect(((Sprite *)sprites.content)[OBJECTS].sprite, rec);
					image(game, ((Sprite *)sprites.content)[OBJECTS].sprite,  map[i].pos.x + cam.x, map[i].pos.y + cam.y, 16, 16);
				}
				if (game->debug) {
					if (game->text) {
						sfText_setCharacterSize(game->text, 15);
						sfText_setScale(game->text, game->baseScale);
						sfText_setColor(game->text, (sfColor){layer == 2 ? 255 : 0, layer == 3 ? 255 : 0, layer == 4 ? 255 : 0, 255});
					}
					buffer = concatf("%i", map[i].id);
					text(buffer, game, map[i].pos.x + cam.x, map[i].pos.y + cam.y);
					free(buffer);
				}
			}
			newLayer = newLayer || map[i].layer > layer;
		}
	}
	for (int i = 0; game->debug && map && map[i].layer; i++)
		if (map[i].solid && map[i].pos.x + cam.x > -32 && map[i].pos.x + cam.x < 672 && map[i].pos.y + cam.y > -32 && map[i].pos.y + cam.y < 672) {
			sfRectangleShape_setFillColor(game->rectangle, (sfColor){255, 0, 255, 125});
			sfRectangleShape_setOutlineColor(game->rectangle, (sfColor){0, 0, 0, 125});
			rect(game, map[i].pos.x + cam.x + 1, map[i].pos.y + cam.y + 1, 14, 14);
		}
}

void	inGame(game_t *game)
{
	Player	player = game->player;
	Object	*map = game->map;

	game->player.canMove = true;
	displayLowerLayer(game);
	displayCharacters(game);
	displayUpperLayer(game);
	if (game->player.state == MOVING && sfTime_asSeconds(sfClock_getElapsedTime(game->player.animationClock)) >= 0.3) {
		game->player.animation = !game->player.animation;
		sfClock_restart(game->player.animationClock);
	} else if (game->player.state == MOVING && sfTime_asSeconds(sfClock_getElapsedTime(game->player.stateClock)) >= 0.3) {
		game->player.state = STATIC;
		game->player.animation = 0;
	}
	if (game->player.canMove) {
		memset(&game->player.blocked, 0, sizeof(game->player.blocked));
		for (int i = 0; map && map[i].layer; i++) {
			if (map[i].solid) {
                                if (player.pos.y + 31 >= map[i].pos.y && player.pos.y + 17 <= map[i].pos.y && player.pos.x < map[i].pos.x + 16 && player.pos.x + 16 > map[i].pos.x)
                                    game->player.blocked.down = true;
                                if (player.pos.y + 16 >= map[i].pos.y && player.pos.y + 16 <= map[i].pos.y + 16 && player.pos.x + 1 <= map[i].pos.x + 16 && map[i].pos.x <= player.pos.x + 16)
                                    game->player.blocked.up = true;
                                if (player.pos.y + 29 >= map[i].pos.y - 1 && player.pos.y + 17 <= map[i].pos.y + 16 && player.pos.x <= map[i].pos.x + 16 && map[i].pos.x <= player.pos.x)
                                    game->player.blocked.left = true;
                                if (player.pos.y + 29 >= map[i].pos.y - 1 && player.pos.y + 17 <= map[i].pos.y + 16 && player.pos.x + 16 <= map[i].pos.x && player.pos.x + 16 >= map[i].pos.x)
                                    game->player.blocked.right = true;
			}
		}
		if (!game->player.blocked.left && sfKeyboard_isKeyPressed(sfKeyLeft)) {
			game->player.pos.x -= 1;
			game->player.position = LEFT;
			game->player.state = MOVING;
			sfClock_restart(game->player.stateClock);
		} else if (!game->player.blocked.right && sfKeyboard_isKeyPressed(sfKeyRight)) {
			game->player.pos.x += 1;
			game->player.position = RIGHT;
			game->player.state = MOVING;
			sfClock_restart(game->player.stateClock);
		} else if (!game->player.blocked.up && sfKeyboard_isKeyPressed(sfKeyUp)) {
			game->player.pos.y -= 1;
			game->player.position = UP;
			game->player.state = MOVING;
			sfClock_restart(game->player.stateClock);
		} else if (!game->player.blocked.down && sfKeyboard_isKeyPressed(sfKeyDown)) {
			game->player.pos.y += 1;
			game->player.position = DOWN;
			game->player.state = MOVING;
			sfClock_restart(game->player.stateClock);
		}
	}
	if (game->player.pos.x + game->cam.x + 8 > 640) {
		game->cam.x -= 640;
	} else if (game->player.pos.x + game->cam.x + 8 < 0) {
		game->cam.x += 640;
	} else if (game->player.pos.y + game->cam.y - 16 > 480) {
		game->cam.y -= 480;
	} else if (game->player.pos.y + game->cam.y + 16 < 0) {
		game->cam.y += 480;
	}
}
