#include "structs.h"
#include "functions.h"
#include "concatf.h"
#include <stdlib.h>
#include <math.h>
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
	Character	player = ((Character *)game->characters.content)[0];
	sfIntRect	rec = {0, 0, 16, 32};
	Array		sprites = game->sprites;
	sfVector2i	cam = game->cam;

	rec.top = (player.movement.position / 2 + (player.movement.state * 2) + (player.movement.animation * 2)) * 32;
	rec.left = (player.movement.position % 2) * 32;
	sfSprite_setTextureRect(((Sprite *)sprites.content)[MALE_CHARACTER + player.texture].sprite, rec);
	image(game, ((Sprite *)sprites.content)[MALE_CHARACTER + player.texture].sprite,  player.movement.pos.x + cam.x, player.movement.pos.y + cam.y, 16, 32);
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

void	drawLifeBar(game_t *game)
{
	int		lifeBuffer = ((Character *)game->characters.content)[0].stats.life;
	int		x = 0;
	int		y = 0;
	int		h = 0;
	int		b = 0;
	sfIntRect	rec = {0, 0, 16, 16};

	for (int i = 1 ; i <= ((Character *)game->characters.content)[0].stats.lifeMax ; i++) {
		if (lifeBuffer <= 10 && lifeBuffer > 0)
			rec.left = 48 + 160 * h + 16 * (10 - lifeBuffer);
		else if (lifeBuffer >= 10)
			rec.left = 48 + 160 * h;
		else
			rec.left = 528;
		sfSprite_setTextureRect(((Sprite *)game->sprites.content)[LIFE_BAR].sprite, rec);
		image(game, ((Sprite *)game->sprites.content)[LIFE_BAR].sprite, x, 465 - y, 16, 16);
		rec.left = 16 * h;
		sfSprite_setTextureRect(((Sprite *)game->sprites.content)[LIFE_BAR].sprite, rec);
		image(game, ((Sprite *)game->sprites.content)[LIFE_BAR].sprite, x, 465 - y, 16, 16);
		if (b == 0)
			x = x + 15;
		if (b == 1) {
			h++;
			if (h == 3) {
				h = 0;
				x += 15;
			}
		}
		if (x >= 15 * 10) {
			x = 0;
			y += 15;
		}
		if (b == 0 && y >= 3 * 15) {
			h++;
			y = 0;
		} 
		if (h == 3 && b == 0) {
			x = 0;
			b = 1;
			h = 0;
			y = y + 45;
		}
		lifeBuffer = lifeBuffer - 10;
	}
}

void	drawEnergyBar(game_t *game)
{
	int		energyBuffer = ((Character *)game->characters.content)[0].stats.energy;
	int		x = 0;
	int		y = 0;
	int		h = 0;
	int		b = 0;
	sfIntRect	rec = {0, 0, 16, 16};

	for (int i = 1 ; i <= ((Character *)game->characters.content)[0].stats.maxEnergy ; i++) {
		if (energyBuffer <= 10 && energyBuffer > 0)
			rec.left = 48 + 160 * h + 16 * (10 - energyBuffer);
		else if (energyBuffer >= 10)
			rec.left = 48 + 160 * h;
		else
			rec.left = 528;
		sfSprite_setTextureRect(((Sprite *)game->sprites.content)[ENERGY_BAR].sprite, rec);
		image(game, ((Sprite *)game->sprites.content)[ENERGY_BAR].sprite, 625 - x, 465 - y, 16, 16);
		rec.left = 16 * h;
		sfSprite_setTextureRect(((Sprite *)game->sprites.content)[ENERGY_BAR].sprite, rec);
		image(game, ((Sprite *)game->sprites.content)[ENERGY_BAR].sprite, 625 - x, 465 - y, 16, 16);
		if (b == 0)
			x = x + 15;
		if (b == 1) {
			h++;
			if (h == 3) {
				h = 0;
				x += 15;
			}
		}
		if (x >= 15 * 10) {
			x = 0;
			y += 15;
		}
		if (b == 0 && y >= 3 * 15) {
			h++;
			y = 0;
		} 
		if (h == 3 && b == 0) {
			x = 0;
			b = 1;
			h = 0;
			y = y + 45;
		}
		energyBuffer = energyBuffer - 10;
	}
}

void	displayHUD(game_t *game)
{
	drawEnergyBar(game);
	drawLifeBar(game);
}

void	dealDamages(Character character, int damages, int damageType)
{
	float	buffer = character.stats.life;
	float	res = 0;

	if (damageType != TRUE_DAMAGE) {
		for (int i = 0; i < 5; i++)
			if (character.wornItems[i].name) {
				res += character.wornItems[i].resistances[damageType];
				if (character.wornItems[i].resistances[damageType] > 0) {
					character.wornItems[i].durability -= damages / 10;
					if (character.wornItems[i].durability <= 0)
						character.wornItems[i].name = NULL;
					for (int j = 0; character.wornItems[i].name != NULL && j < DAMAGES_TYPE_NB; j++)
						character.wornItems[i].resistances[j] = character.wornItems[i].baseResistances[j] * character.wornItems[i].durability / character.wornItems[i].baseDurability;
				}
			}
		character.stats.life -= damages * (20 / (sqrt(abs(res)) + 20));
	} else
		character.stats.life -= damages;
	if (character.stats.life > character.stats.lifeMax * 10)
		character.stats.life = character.stats.lifeMax * 10;
	if (character.stats.life < 0)
		character.stats.life = 0;
	if (character.stats.life - buffer != 0) {
		character.damageDisplay[damageType] -= buffer - character.stats.life;
		sfClock_restart(character.damageClock[damageType]);
	}
}

void	execAction(game_t *game, Object obj)
{
	switch(obj.action) {
	case DEAL_DAMAGES:
		for (int i = 0; i < DAMAGES_TYPE_NB; i++)
			if (obj.damages[i])
				dealDamages(((Character *)game->characters.content)[0], obj.damages[i], i);
		break;
	case CHANGE_MUSIC:
		break;
	case LAUNCH_CUTSCENE:
		break;
	}
}

float	isPressed(int keyID)
{
	if (keyID <= 200)
		return (sfKeyboard_isKeyPressed(keyID));
	else if (keyID > 204)
		return (sfJoystick_isButtonPressed(0, keyID - 105));
	else if (keyID == 201)
		return (sfJoystick_getAxisPosition(0, sfJoystickY) > -3 ? 0 : -sfJoystick_getAxisPosition(0, sfJoystickY) / 100);
	else if (keyID == 202)
		return (sfJoystick_getAxisPosition(0, sfJoystickY) < 3 ? 0 : sfJoystick_getAxisPosition(0, sfJoystickY) / 100);
	else if (keyID == 203)
		return (sfJoystick_getAxisPosition(0, sfJoystickX) > -3 ? 0 : -sfJoystick_getAxisPosition(0, sfJoystickX) / 100);
	return (sfJoystick_getAxisPosition(0, sfJoystickX) < 3 ? 0 : sfJoystick_getAxisPosition(0, sfJoystickX) / 100);
}

void	movePlayer(game_t *game)
{
	Character	*player = &((Character *)game->characters.content)[0];
	Object		*map = game->map;

	memset(&player->movement.blocked, 0, sizeof(player->movement.blocked));
	for (int i = 0; map && map[i].layer; i++) {
		if (map[i].solid) {
			if (player->movement.pos.y + 31 >= map[i].pos.y && player->movement.pos.y + 17 <= map[i].pos.y && player->movement.pos.x < map[i].pos.x + 16 && player->movement.pos.x + 16 > map[i].pos.x)
			    player->movement.blocked.down = true;
			if (player->movement.pos.y + 16 >= map[i].pos.y && player->movement.pos.y + 16 <= map[i].pos.y + 16 && player->movement.pos.x + 1 <= map[i].pos.x + 16 && map[i].pos.x <= player->movement.pos.x + 16)
			    player->movement.blocked.up = true;
			if (player->movement.pos.y + 29 >= map[i].pos.y - 1 && player->movement.pos.y + 17 <= map[i].pos.y + 16 && player->movement.pos.x <= map[i].pos.x + 16 && map[i].pos.x <= player->movement.pos.x)
			    player->movement.blocked.left = true;
			if (player->movement.pos.y + 29 >= map[i].pos.y - 1 && player->movement.pos.y + 17 <= map[i].pos.y + 16 && player->movement.pos.x + 16 <= map[i].pos.x && player->movement.pos.x + 16 >= map[i].pos.x)
			    player->movement.blocked.right = true;
		}
	}
	player->movement.speed = 0;
	if (!player->movement.blocked.left && isPressed(game->settings.keys[KEY_LEFT])) {
		player->movement.pos.x -= isPressed(game->settings.keys[KEY_LEFT]);
		player->movement.position = LEFT;
		player->movement.state = MOVING;
		player->movement.speed += isPressed(game->settings.keys[KEY_LEFT]);
		sfClock_restart(player->movement.stateClock);
		if (isPressed(game->settings.keys[KEY_SPRINT]) && player->stats.energy >= player->stats.sprintSpeed) {
			player->movement.pos.x -= player->stats.sprintSpeed * isPressed(game->settings.keys[KEY_LEFT]) - 1;
			player->stats.energyClock++;
		}
	}
	if (!player->movement.blocked.right && isPressed(game->settings.keys[KEY_RIGHT])) {
		player->movement.pos.x += isPressed(game->settings.keys[KEY_RIGHT]);
		if (isPressed(game->settings.keys[KEY_RIGHT]) > isPressed(game->settings.keys[KEY_LEFT]))
			player->movement.position = RIGHT;
		player->movement.state = MOVING;
		player->movement.speed += isPressed(game->settings.keys[KEY_RIGHT]);
		sfClock_restart(player->movement.stateClock);
		if (isPressed(game->settings.keys[KEY_SPRINT]) && player->stats.energy >= player->stats.sprintSpeed) {
			player->movement.pos.x += player->stats.sprintSpeed * isPressed(game->settings.keys[KEY_RIGHT]) - 1;
			player->stats.energyClock++;
		}
	}
	if (!player->movement.blocked.up && isPressed(game->settings.keys[KEY_UP])) {
		player->movement.pos.y -= isPressed(game->settings.keys[KEY_UP]);
		if (isPressed(game->settings.keys[KEY_UP]) > isPressed(game->settings.keys[KEY_RIGHT]) && isPressed(game->settings.keys[KEY_UP]) > isPressed(game->settings.keys[KEY_LEFT]))
			player->movement.position = UP;
		player->movement.state = MOVING;
		sfClock_restart(player->movement.stateClock);
		player->movement.speed += isPressed(game->settings.keys[KEY_UP]);
		if (isPressed(game->settings.keys[KEY_SPRINT]) && player->stats.energy >= player->stats.sprintSpeed) {
			player->movement.pos.y -= player->stats.sprintSpeed * isPressed(game->settings.keys[KEY_UP]) - 1;
			player->stats.energyClock++;
		}
	}
	if (!player->movement.blocked.down && isPressed(game->settings.keys[KEY_DOWN])) {
		player->movement.pos.y += isPressed(game->settings.keys[KEY_DOWN]);
		if (isPressed(game->settings.keys[KEY_DOWN]) > isPressed(game->settings.keys[KEY_RIGHT]) && isPressed(game->settings.keys[KEY_DOWN]) > isPressed(game->settings.keys[KEY_LEFT]) && isPressed(game->settings.keys[KEY_DOWN]) > isPressed(game->settings.keys[KEY_UP]))
			player->movement.position = DOWN;
		player->movement.state = MOVING;
		sfClock_restart(player->movement.stateClock);
		player->movement.speed += isPressed(game->settings.keys[KEY_DOWN]);
		if (isPressed(game->settings.keys[KEY_SPRINT]) && player->stats.energy >= player->stats.sprintSpeed) {
			player->movement.pos.y += player->stats.sprintSpeed * isPressed(game->settings.keys[KEY_DOWN]) - 1;
			player->stats.energyClock++;
		}
	}
	for (int i = 0; map && map[i].layer; i++)
		if (player->movement.pos.y + 16 >= map[i].pos.y && player->movement.pos.y + 16 <= map[i].pos.y && player->movement.pos.x < map[i].pos.x + 16 && player->movement.pos.x + 16 > map[i].pos.x)
			execAction(game, map[i]);
	if (player->stats.energyClock >= player->stats.timeBeforeEnergyUse) {
		player->stats.energyClock -= player->stats.timeBeforeEnergyUse;
		player->stats.energy -= player->stats.energyUsedBySprint;
		sfClock_restart(player->stats.energyRegenClock);
	}
	if (sfTime_asSeconds(sfClock_getElapsedTime(player->stats.energyRegenClock)) >= 4)
		player->stats.energy += player->stats.energyRegen;
	if (player->stats.energy > player->stats.maxEnergy * 10)
		player->stats.energy = player->stats.maxEnergy * 10;
}

void	inGame(game_t *game)
{
	Character	*player = &((Character *)game->characters.content)[0];

	displayLowerLayer(game);
	displayCharacters(game);
	displayUpperLayer(game);
	displayHUD(game);
	if (player->movement.state == MOVING && sfTime_asSeconds(sfClock_getElapsedTime(player->movement.animationClock)) >= 0.1 / player->movement.speed) {
		player->movement.animation = !player->movement.animation;
		sfClock_restart(player->movement.animationClock);
	} else if (player->movement.state == MOVING && sfTime_asSeconds(sfClock_getElapsedTime(player->movement.stateClock)) >= 0.3) {
		player->movement.state = STATIC;
		player->movement.animation = 0;
	}
	if (player->stats.life > 10 * player->stats.lifeMax) 
		player->stats.life = 10 * player->stats.lifeMax;
	if (player->movement.canMove)
		movePlayer(game);
	if (player->movement.pos.x + game->cam.x + 8 > 640) {
		game->cam.x -= 640;
	} else if (player->movement.pos.x + game->cam.x + 8 < 0) {
		game->cam.x += 640;
	} else if (player->movement.pos.y + game->cam.y - 16 > 480) {
		game->cam.y -= 480;
	} else if (player->movement.pos.y + game->cam.y + 16 < 0) {
		game->cam.y += 480;
	}
}
