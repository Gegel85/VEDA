#ifndef __GAME_STRUCT_HEADER_
#define __GAME_STRUCT_HEADER_

#include <SFML/Graphics.h>
#include <lua.h>
#include "icon.h"
#include "button.h"
#include "array.h"
#include "language.h"
#include "sprite.h"
#include "object.h"
#include "battle.h"

typedef struct {
	char		*displayedText;
	char		*dialogOwnerName;
	char		*rawText;
	int		index;
	float		speed;
	sfClock		*clock;
} DialogDisplayed;

typedef struct {
	sfUint32	buffer[BUFFER_MAX_SIZE + 1];
	int		bufPos;
	int		bufSize;
} Input;

typedef struct {
	Object		*objects;
	Sprite		background;
	char		*backgroundPath;
	char		*path;
} Map;

typedef struct {
	int		menu;
	sfVector2i	cameraPos;
	Array		characters;
	Battle		battle_infos;
	Map		loadedMap;
	int		dialogs;
	DialogDisplayed	*dialogsOnScreen;
	int		menuSelected;
} GameState;

typedef struct {
	Icon			icon;
	Button			*buttons;
	Array			sprites;
	Array			musics;
	Array			sfx;
	Array			fonts;
	sfRenderWindow		*window;
	Language		*languages;
	sfRectangleShape	*rectangle;
	sfCircleShape		*circle;
	sfText			*text;
	lua_State		*dialogLuaScript;
} Ressources;

typedef struct {
	bool		debug;
	Ressources	ressources;
	Settings	settings;
	GameState	state;
	Input		input;
	sfVector2i	languagesConf;
} game_t;

#endif