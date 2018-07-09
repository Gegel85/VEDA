#include "functions.h"
#include "concatf.h"
#include "structs.h"
#include "configParser.h"
#include "battle_api.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern game_t game;

Battle	invalidData(ParserResult result, char *path, char *message)
{
	Battle		battle;
	char		*buffer;

	Parser_destroyData(result.data, result.type);
	memset(&battle, 0, sizeof(battle));
	battle.type = BATTLE_ERROR;
	printf("%s: %s: %s\n", ERROR, path, message);
	buffer = concatf("Error: File '%s' contains invalid battle data:\n%s", path, message);
	dispMsg("Battle Error", buffer, 0);
	free(buffer);
	return battle;
}

Array	invalidDataArray(ParserResult result, char *path, char *message)
{
	char	*buffer;

	Parser_destroyData(result.data, result.type);
	printf("%s: %s: %s\n", ERROR, path, message);
	buffer = concatf("Error: File '%s' contains invalid battle data:\n%s", path, message);
	dispMsg("Battle Error", buffer, 0);
	free(buffer);
	return (Array){NULL, -1};
}

Battle	invalidType(ParserResult result, char *path, char *message, ParserTypes got, ParserTypes expected)
{
	Battle		battle;
	char		*buffer;

	Parser_destroyData(result.data, result.type);
	memset(&battle, 0, sizeof(battle));
	battle.type = BATTLE_ERROR;
	printf("%s: %s: %s\n", ERROR, path, message);
	buffer = concatf("Error: File '%s' contains invalid battle data:\n%s:\n%s expected but got %s\n", path, message, typeToString(expected), typeToString(got));
	dispMsg("Battle Error", buffer, 0);
	free(buffer);
	return battle;
}

Array	invalidTypeArray(ParserResult result, char *path, char *message, ParserTypes got, ParserTypes expected)
{
	char	*buffer;

	Parser_destroyData(result.data, result.type);
	printf("%s: %s: %s\n", ERROR, path, message);
	buffer = concatf("Error: File '%s' contains invalid battle data:\n%s:\n%s expected but got %s\n", path, message, typeToString(expected), typeToString(got));
	dispMsg("Battle Error", buffer, 0);
	free(buffer);
	return (Array){NULL, -1};
}

Array	loadProjectiles(char *path)
{
	ParserResult	result = Parser_parseFile(path, JSON_TO_ARRAY);
	ParserObj	*buffer;
	ParserObj	*buffer2;
	ParserObj	*currProjectile;
	sfVector2f	scaling = game.baseScale;
	Projectile	projBuffer;
	Array		array;

	printf("%s: Loading projectiles in file %s\n", INFO, path);
	if (result.error) {
		printf("%s: %s\n", ERROR, result.error);
		result.error = concatf("Error: Couldn't load file '%s':\n%s\n", path, result.error);
		dispMsg("Battle Error", result.error, 0);
		free(result.error);
		return (Array){NULL, -1};
	} else if (result.type != ParserArrayType) {
		printf("%s: %s: Invalid type\n", ERROR, path);
		buffer = (void *)concatf("Error: %s: Invalid type found in the file\n", path);
		dispMsg("Battle Error", (void *)buffer, 0);
		free(buffer);
		return (Array){NULL, -1};
	} else if (((ParserArray *)result.data)->type != ParserObjType) {
		printf("%s: %s: Invalid type\n", ERROR, path);
		buffer = (void *)concatf("Error: %s: Array contains invalid data\n", path);
		dispMsg("Battle Error", (void *)buffer, 0);
		free(buffer);
		return (Array){NULL, -1};
	}
	array.length = ((ParserArray *)result.data)->length;
	array.content = malloc(array.length * sizeof(Projectile));
	memset(array.content, 0, array.length * sizeof(Projectile));
	for (int i = 0; i < array.length; i++) {
		scaling = (sfVector2f){1, 1};
		currProjectile = ParserArray_getElement(result.data, i);
		memset(&projBuffer, 0, sizeof(projBuffer));
		if (buffer = ParserObj_getElement(currProjectile, "sprite_sheet")) {
			if (buffer->type == ParserStringType) {
				projBuffer.sprite = createSprite((Sprite_config)
				{
					ParserString_toCharStar(buffer->data),
				        (sfVector2f){1, 1},
					(sfVector2i){0, 0},
					(sfVector2i){0, 0},
				});
				projBuffer.needToDestroySprite = true;
			} else if (buffer->type == ParserIntType) {
				projBuffer.sprite = ((Sprite *)game.sprites.content)[ParserInt_toInt(buffer->data) % game.sprites.length];
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"sprite_sheet\"", buffer->type, ParserStringType);
		} else
			printf("%s: Field \"sprite_sheet\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "sprite_size")) {
			if (buffer->type == ParserObjType) {
				projBuffer.sprite.rect.width = projBuffer.sprite.texture ? sfTexture_getSize(projBuffer.sprite.texture).x : 0;
				projBuffer.sprite.rect.height = projBuffer.sprite.texture ? sfTexture_getSize(projBuffer.sprite.texture).y : 0;
				if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
					if (buffer2->type == ParserIntType) {
						projBuffer.sprite.rect.width = ParserInt_toInt(buffer2->data);
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"x\" in \"sprite_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"x\" is missing in \"sprite_size\"\n", WARNING);
				if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
					if (buffer2->type == ParserIntType) {
						projBuffer.sprite.rect.height = ParserInt_toInt(buffer2->data);
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"y\" in \"sprite_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"y\" is missing in \"sprite_size\"\n", WARNING);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"sprite_size\"", buffer->type, ParserObjType);
		} else
			printf("%s: Field \"sprite_size\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "bullet_size")) {
			if (buffer->type == ParserObjType) {
				if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
					if (buffer2->type == ParserIntType) {
						scaling.x = game.baseScale.x * (float)ParserInt_toInt(buffer2->data) / (float)projBuffer.sprite.rect.width;
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"x\" in \"bullet_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"x\" is missing in \"bullet_size\"\n", WARNING);
				if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
					if (buffer2->type == ParserIntType) {
						scaling.y = game.baseScale.y * (float)ParserInt_toInt(buffer2->data) / (float)projBuffer.sprite.rect.height;
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"y\" in \"bullet_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"y\" is missing in \"bullet_size\"\n", WARNING);
				if (projBuffer.sprite.sprite)
					sfSprite_setScale(projBuffer.sprite.sprite, scaling);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"bullet_size\"", buffer->type, ParserObjType);
		} else
			printf("%s: Field \"hitbox_size\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "hitbox_size")) {
			if (buffer->type == ParserObjType) {
				if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
					if (buffer2->type == ParserIntType) {
						projBuffer.hitbox.x = ParserInt_toInt(buffer2->data);
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"x\" in \"hitbox_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"x\" is missing in \"hitbox_size\"\n", WARNING);
				if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
					if (buffer2->type == ParserIntType) {
						projBuffer.hitbox.y = ParserInt_toInt(buffer2->data);
					} else
						return invalidTypeArray(result, path, "Invalid type for field \"y\" in \"hitbox_size\"", buffer2->type, ParserIntType);
				} else
					printf("%s: Field \"y\" is missing in \"hitbox_size\"\n", WARNING);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"hitbox_size\"", buffer->type, ParserObjType);
		} else
			printf("%s: Field \"hitbox_size\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "base_speed")) {
		        if (buffer->type == ParserIntType) {
				projBuffer.speed = ParserInt_toInt(buffer->data);
			} else if (buffer->type == ParserFloatType) {
				projBuffer.speed = ParserFloat_toFloat(buffer->data);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"base_speed\"", buffer->type, ParserFloatType);
		} else
			printf("%s: Field \"base_speed\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "base_acceleration")) {
		        if (buffer->type == ParserIntType) {
				projBuffer.acceleration = ParserInt_toInt(buffer->data);
			} else if (buffer->type == ParserFloatType) {
				projBuffer.acceleration = ParserFloat_toFloat(buffer->data);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"base_acceleration\"", buffer->type, ParserFloatType);
		} else
			printf("%s: Field \"base_acceleration\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "rotation_speed")) {
		        if (buffer->type == ParserIntType) {
				projBuffer.rotaSpeed = ParserInt_toInt(buffer->data);
			} else if (buffer->type == ParserFloatType) {
				projBuffer.rotaSpeed = ParserFloat_toFloat(buffer->data);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"rotation_speed\"", buffer->type, ParserFloatType);
		} else
			printf("%s: Field \"rotation_speed\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "base_angle")) {
		        if (buffer->type == ParserIntType) {
				projBuffer.angle = ParserInt_toInt(buffer->data);
			} else if (buffer->type == ParserFloatType) {
				projBuffer.angle = ParserFloat_toFloat(buffer->data);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"base_angle\"", buffer->type, ParserFloatType);
		} else
			printf("%s: Field \"base_angle\" is missing\n", WARNING);
		if (buffer = ParserObj_getElement(currProjectile, "animation_speed")) {
		        if (buffer->type == ParserIntType) {
				projBuffer.animSpeed = ParserInt_toInt(buffer->data);
			} else if (buffer->type == ParserFloatType) {
				projBuffer.animSpeed = ParserFloat_toFloat(buffer->data);
			} else
				return invalidTypeArray(result, path, "Invalid type for field \"animation_speed\"", buffer->type, ParserFloatType);
		} else
			printf("%s: Field \"animation_speed\" is missing\n", WARNING);
		projBuffer.bankID = i;
		if (projBuffer.sprite.sprite)
			sfSprite_setOrigin(projBuffer.sprite.sprite, (sfVector2f){(float)projBuffer.sprite.rect.width / 2, (float)projBuffer.sprite.rect.height / 2});
		((Projectile *)array.content)[i] = projBuffer;
	}
	Parser_destroyData(result.data, result.type);
	return array;
}

void	addDependencies(lua_State *Lua)
{
	luaL_openlibs(Lua);
	lua_pushcfunction(Lua, &getElapsedTime);
	lua_setglobal(Lua, "getElapsedTime");
	lua_pushcfunction(Lua, &addProjectileLua);
	lua_setglobal(Lua, "addProjectile");
	lua_pushcfunction(Lua, &stopTime);
	lua_setglobal(Lua, "stopTime");
	lua_pushcfunction(Lua, &yield);
	lua_setglobal(Lua, "yield");
}

Battle	loadBattleScript(char *path)
{
	Battle		battle;
	int		err;
	ParserResult	result = Parser_parseFile(path, NULL);
	ParserObj	*buffer;
	ParserObj	*buffer2;

	printf("%s: Loading %s as battle infos\n", INFO, path);
	memset(&battle, 0, sizeof(battle));
	battle.type = BATTLE_ERROR;
	if (result.error) {
		printf("%s: %s\n", ERROR, result.error);
		result.error = concatf("Error: Couldn't load file '%s':\n%s\n", path, result.error);
		dispMsg("Battle Error", result.error, 0);
		free(result.error);
		battle.type = BATTLE_ERROR;
		return battle;
	} else if (result.type != ParserObjType)
		return invalidType(result, path, "Invalid type found in file", result.type, ParserObjType);
	if (buffer = ParserObj_getElement(result.data, "type")) {
		if (buffer->type == ParserStringType) {
			if (strcmp(ParserString_toCharStar(buffer->data), "DANMAKU") == 0)
				battle.type = DANMAKU_BATTLE;
			else {
				buffer = (void *)concatf("Unknown battle type '%s'", ParserString_toCharStar(buffer->data));
				battle = invalidData(result, path, (void *)buffer);
				free(buffer);
				ParserObj_destroy(result.data);
				return battle;
			}
		} else if (buffer->type == ParserIntType) {
			battle.type = ParserInt_toInt(buffer->data);
		} else
			return invalidType(result, path, "Invalid type for field \"type\"", buffer->type, ParserStringType);
	} else
		return invalidData(result, path, "Field \"type\" is missing");
	if (buffer = ParserObj_getElement(result.data, "battle_name")) {
		if (buffer->type == ParserStringType) {
			battle.name = strdup(ParserString_toCharStar(buffer->data));
		} else
			return invalidType(result, path, "Invalid type for field \"battle_name\"", buffer->type, ParserStringType);
	} else
		printf("%s: Field \"battle_name\" is missing", WARNING);
	if (buffer = ParserObj_getElement(result.data, "boss_name")) {
		if (buffer->type == ParserStringType) {
			strcpy(battle.boss.name, ParserString_toCharStar(buffer->data));
		} else
			return invalidType(result, path, "Invalid type for field \"boss_name\"", buffer->type, ParserStringType);
	} else
		printf("%s: Field \"boss_name\" is missing\n", WARNING);
	if (buffer = ParserObj_getElement(result.data, "boss_sprite_sheet")) {
		if (buffer->type == ParserStringType) {
			battle.bossSprite = createSprite((Sprite_config){
				ParserString_toCharStar(buffer->data),
				(sfVector2f){1, 1},
				(sfVector2i){0, 0},
				(sfVector2i){0, 0},
			});
		} else if (buffer->type == ParserIntType) {
			battle.bossSprite = ((Sprite *)game.sprites.content)[ParserInt_toInt(buffer->data) % game.sprites.length];
		} else
			return invalidType(result, path, "Invalid type for field \"boss_sprite_sheet\"", buffer->type, ParserStringType);
	} else
		printf("%s: Field \"boss_sprite_sheet\" is missing\n", WARNING);
	if (buffer = ParserObj_getElement(result.data, "projectiles")) {
		if (buffer->type == ParserStringType) {
			battle.projectileBank = loadProjectiles(ParserString_toCharStar(buffer->data));
		} else
			return invalidType(result, path, "Invalid type for field \"projectiles\"", buffer->type, ParserStringType);
	} else
		return invalidData(result, path, "Field \"projectiles\" is missing");
	if (buffer = ParserObj_getElement(result.data, "base_script")) {
		if (buffer->type == ParserStringType) {
			battle.Lua  = luaL_newstate();
			battle.script = strdup(ParserString_toCharStar(buffer->data));
			addDependencies(battle.Lua);
			err = luaL_dofile(battle.Lua, ParserString_toCharStar(buffer->data));
			if (err) {
				buffer = (void *)concatf("An unexpected error occured when loading %s", ParserString_toCharStar(buffer->data));
				battle = invalidData(result, path, (void *)buffer);
				free(buffer);
				return battle;
			}
		} else
			return invalidType(result, path, "Invalid type for field \"base_script\"", buffer->type, ParserStringType);
	} else
		return invalidData(result, path, "Field \"base_script\" is missing");
	if (buffer = ParserObj_getElement(result.data, "music")) {
		if (buffer->type == ParserStringType) {
			battle.music = createMusic((Music_config){
				ParserString_toCharStar(buffer->data),
				true,
			});
		} else if (buffer->type == ParserIntType) {
			battle.music = ((sfMusic **)game.musics.content)[ParserInt_toInt(buffer->data) % game.musics.length];
		} else
			return invalidType(result, path, "Invalid type for field \"music\"", buffer->type, ParserStringType);
	} else
		printf("%s: Field \"boss_sprite_sheet\" is missing\n", WARNING);
	if (buffer = ParserObj_getElement(result.data, "boss_sprite_size")) {
		if (buffer->type == ParserObjType) {
			if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
				if (buffer2->type == ParserIntType) {
					battle.bossSprite.rect.width = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"x\" in \"boss_sprite_size\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"x\" is missing in \"boss_sprite_size\"\n", WARNING);
			if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
				if (buffer2->type == ParserIntType) {
					battle.bossSprite.rect.height = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"y\" in \"boss_sprite_size\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"y\" is missing in \"boss_sprite_size\"\n", WARNING);
		} else
			return invalidType(result, path, "Invalid type for field \"boss_sprite_size\"", buffer->type, ParserObjType);
	} else
		printf("%s: Field \"boss_sprite_size\" is missing\n", WARNING);
	if (buffer = ParserObj_getElement(result.data, "boss_hitbox")) {
		if (buffer->type == ParserObjType) {
			if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
				if (buffer2->type == ParserIntType) {
					battle.bossHitbox.x = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"x\" in \"boss_hitbox\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"x\" is missing in \"boss_hitbox\"\n", WARNING);
			if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
				if (buffer2->type == ParserIntType) {
					battle.bossHitbox.y = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"y\" in \"boss_hitbox\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"y\" is missing in \"boss_hitbox\"\n", WARNING);
		} else
			return invalidType(result, path, "Invalid type for field \"boss_hitbox\"", buffer->type, ParserIntType);
	} else
		printf("%s: Field \"boss_hitbox\" is missing\n", WARNING);
	if (buffer = ParserObj_getElement(result.data, "player_hitbox")) {
		if (buffer->type == ParserObjType) {
			if (buffer2 = ParserObj_getElement(buffer->data, "x")) {
				if (buffer2->type == ParserIntType) {
					battle.playerHitbox.x = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"x\" in \"player_hitbox\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"x\" is missing in \"player_hitbox\"\n", WARNING);
			if (buffer2 = ParserObj_getElement(buffer->data, "y")) {
				if (buffer2->type == ParserIntType) {
					battle.playerHitbox.y = ParserInt_toInt(buffer2->data);
				} else
					return invalidType(result, path, "Invalid type for field \"y\" in \"player_hitbox\"", buffer2->type, ParserIntType);
			} else
				printf("%s: Field \"y\" is missing in \"player_hitbox\"\n", WARNING);
		} else
			return invalidType(result, path, "Invalid type for field \"player_hitbox\"", buffer->type, ParserObjType);
	} else
		printf("%s: Field \"boss_hitbox\" is missing\n", WARNING);
	ParserObj_destroy(result.data);
	battle.clock = sfClock_create();
	return battle;
}

void	updateProjectiles(Array array)
{
	Projectile	*projs = array.content;

	for (int i = 0; i < array.length; i++) {
		projs[i].pos.x += cos(projs[i].angle * M_PI / 180) * projs[i].speed;
		projs[i].pos.y += sin(projs[i].angle * M_PI / 180) * projs[i].speed;
		projs[i].speed += projs[i].acceleration;
		projs[i].angle += projs[i].rotaSpeed;
	}
}

void	displayProjectiles(game_t *game)
{
	Projectile	*projs = game->battle_infos.projectiles.content;
	sfVector2f	pos;

	for (int i = 0; i < game->battle_infos.projectiles.length; i++) {
		if (projs[i].animSpeed >= 0 && sfTime_asSeconds(sfClock_getElapsedTime(projs[i].animClock)) >= projs[i].animSpeed) {
			sfClock_restart(projs[i].animClock);
			projs[i].animation = (projs[i].animation >= ((int)sfTexture_getSize(projs[i].sprite.texture).x / projs[i].sprite.rect.width) - 1) ? 0 : projs[i].animation + 1;
		}
		projs[i].sprite.rect.left = projs[i].animation * projs[i].sprite.rect.width;
		pos.x = (projs[i].pos.x + game->cam.x) * game->baseScale.x;
		pos.y = (projs[i].pos.y + game->cam.y) * game->baseScale.y;
		sfSprite_setRotation(projs[i].sprite.sprite, projs[i].angle);
		sfSprite_setPosition(projs[i].sprite.sprite, pos);
		sfSprite_setTextureRect(projs[i].sprite.sprite, projs[i].sprite.rect);
		sfRenderWindow_drawSprite(game->window, projs[i].sprite.sprite, NULL);
	}
}

int	errorHandler(lua_State *Lua)
{
	lua_pushstring(Lua, luaL_checkstring(Lua, 1));
	return 1;
}

void	battle(game_t *game)
{
	static	bool	launchLua = true;

	if (launchLua && game->battle_infos.Lua) {
		lua_State	*Lua = lua_newthread(game->battle_infos.Lua);
		int		err;
		char		*buffer;

		launchLua = false;
		// luaL_dofile(lua, "/users/gegel85/documents/github/veda/data/battles/alexandre/battle_normal/actions.lua");
		// lua_getglobal(lua, "test");
		// lua_call(lua, 0, 0);
		// exit(0);
		lua_pushcfunction(Lua, &errorHandler);
		lua_getglobal(Lua, "bossAI");
		err = lua_pcall(Lua, 0, 0, 1);
		if (err == LUA_ERRRUN) {
			buffer = concatf("A runtime error occurred during battle:\n%s", luaL_checkstring(Lua, 2));
			printf("%s: %s\n", ERROR, lua_tostring(Lua, 2));
			dispMsg("Battle Error", buffer, 0);
			free(buffer);
			back_on_title_screen(game, -1);
		} else if (err == LUA_ERRMEM) {
			buffer = concatf("A runtime error occurred during battle:\n%s: Out of memory", game->battle_infos.script);
			printf("%s: %s: Out of memory\n", ERROR, game->battle_infos.script);
			dispMsg("Battle Error", buffer, 0);
			free(buffer);
			back_on_title_screen(game, -1);
		} else if (err == LUA_ERRERR) {
			buffer = concatf("An unexpected error occurred during battle:\n%s", game->battle_infos.script);
			printf("%s: %s: Unknown error\n", ERROR, game->battle_infos.script);
			dispMsg("Battle Error", buffer, 0);
			free(buffer);
			back_on_title_screen(game, -1);
		}
		launchLua = true;
	} else {
		if (game->battle_infos.music && sfMusic_getStatus(game->battle_infos.music) != sfPlaying) {
			for (int i = 0; i < game->musics.length; i++)
				if (((sfMusic **)game->musics.content)[i] && sfMusic_getStatus(game->battle_infos.music) == sfPlaying)
					sfMusic_stop(((sfMusic **)game->musics.content)[i]);
			sfMusic_play(game->battle_infos.music);
			sfMusic_setVolume(game->battle_infos.music, game->settings.musicVolume);
		}
		displayLowerLayer(game);
		displayCharacters(game);
		displayCharacter(&game->battle_infos.boss, game, 10, game->battle_infos.bossSprite.sprite);
		displayUpperLayer(game);
		displayProjectiles(game);
		displayHUD(game);
		if (!game->battle_infos.timeStopped) {
			movePlayer(game);
			updateProjectiles(game->battle_infos.projectiles);
		} else {
			sfRectangleShape_setFillColor(game->rectangle, (sfColor){255, 230, 255, 55});
			rect(game, 0, 0, 640, 480);
		}
	}
}
