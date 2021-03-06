#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "concatf.h"

char	*header;

enum damagesTypes {
	TRUE_DAMAGE,
	FIRE_DAMAGE,
	SHARP_DAMAGE,
	PIERCE_DAMAGE,
	IMPACT_DAMAGE,
	POISON_DAMAGE,
	ELECTRICITY_DAMAGE,
	DAMAGES_TYPE_NB,
};

enum actions {
	NONE,
	DEAL_DAMAGES,
	CHANGE_MUSIC,
	LAUNCH_CUTSCENE,
};

enum musics {
	GAME_OVER,
	MACABRE,
	MYSTERIOUS,
	CLAVIER,
	MAIN_MENU_MUSIC,
	EPICBATTLE_START,
	EPICBATTLE_LOOP,
	SQRT1,
	SQRT2,
	EUGRT_FULL,
	EUGRT_START,
	EUGRT_LOOP,
	UMLAUT,
};

const bool	solidObjects[] = {
	true,  //1
	false, //2
	false, //3
	false, //4
	false, //5
	false, //6
	false, //7
	false, //8
	false, //9
	false, //10
	false, //11
	false, //12
	false, //13
	true,  //14
	false, //15
	false, //16
	false, //17
	false, //18
	false, //19
	true,  //20
	false, //21
	false, //22
	false, //23
	false, //24
	false, //25
	false, //26
	true,  //27
	true,  //28
	false, //29
	false, //30
	false, //31
	false, //32
};

enum sfx {
	IRON,
	ERROR_SOUND,
	EXCLAMATION,
	O_o,
	GRASS1,
	GRASS2,
	GRASS3,
	GRASS4,
	DIRT1,
	DIRT2,
	DIRT3,
	DIRT4,
	MUD1,
	MUD2,
	MUD3,
	MUD4,
};

typedef struct {
	int	object;
	int	replacement;
} Layer_config;

const Layer_config	layer[] = {
	{30, 1},
	{31, 27},
	{19, 19},
	{29, 29},
	{18, 18},
	{-1, -1},
};

typedef struct {
	int	x;
	int	y;
} sfVector2i;

typedef struct {
	int		id;
	bool		solid;
	sfVector2i	pos;
	int		layer;
	int		damages[DAMAGES_TYPE_NB];
	int		action;
	float		invulnerabiltyTime;
	int		footstepSound;
	int		footstepVariance;
} Object;

char	*concat(char *, char *, bool, bool);

char	**split(char *string, char separator)
{
	int	len = 2;
	char	*str = strdup(string);
	char	**result = NULL;
	
	for (int i = 0; str[i]; i++, len += (str[i] == separator));
	result = malloc(len * sizeof(*result));
	if (!result) {
		printf("Error: Couldn't allocate %liB", (long)(len * sizeof(*result)));
		exit(EXIT_FAILURE);
	}
	result[0] = str;
	result[len - 1] = NULL;
	len = 1;
	for (int i = 0; str[i]; i++) {
		if (str[i] == separator) {
			str[i] = '\0';
			result[len++] = &str[i + 1];
		}
	}
	return (result);
}

bool	is_nbr(char *nbr)
{
	int	i = 0;
	for (; nbr[i] && (nbr[i] == '+' || nbr[i] == '-'); i++);
	for (; nbr[i] && nbr[i] >= '0' && nbr[i] <= '9'; i++);
	if (nbr[i] == '.')
		i++;
	for (; nbr[i] && nbr[i] >= '0' && nbr[i] <= '9'; i++);
	return (nbr[i] == 0);
}

void	showStr(char *str)
{
	for (int i = 0; str[i]; i++) {
		if (str[i] == 127 || str[i] < ' ')
			printf("\\0%o", (unsigned char)str[i]);
		else
			printf("%c", str[i]);
	}
}

Object	*loadLevel(char *path)
{
	int		fd;
	struct stat	buffer;
	char		*file_buffer = NULL;
	Object		*objs;
	char		**lines = NULL;

	if (stat(path, &buffer) < 0) {
		perror(path);
		return (NULL);
	}
	file_buffer = malloc(buffer.st_size + 1);
	if (!file_buffer) {
		printf("Error: Couldn't allocate %liB", (long)(buffer.st_size + 1));
		exit(EXIT_FAILURE);
	}
	fd = open(path, O_RDONLY);
	printf("Open %s\n", path);
	if (fd < 0) {
		perror(path);
		return (NULL);
	}
	read(fd, file_buffer, buffer.st_size);
	close(fd);
	objs = malloc(sizeof(*objs));
	memset(objs, 0, sizeof(*objs));
	file_buffer[buffer.st_size] = '\0';
	lines = split(file_buffer, '\n');
	header = strdup(lines[0]);
	for (int i = 1; lines[i] && strcmp(lines[i], ""); i += 3) {
		objs = realloc(objs, sizeof(*objs) * (i / 3 + 2));
		if (!objs) {
			printf("Error: Couldn't allocate %liB", sizeof(*objs) * (i / 3 + 2));
			exit(EXIT_FAILURE);
		}
		memset(&objs[i / 3], 0, sizeof(*objs));
		for (int j = 1; j < 3; j++)
			if (!lines[i + j]) {
				printf("Unexpected <EOF> after line %i\n", i + j);
				free(objs);
				free(lines[0]);
				free(lines);
				free(file_buffer);
				return (NULL);
			}
		for (int j = 0; j < 3; j++)
			if (!is_nbr(lines[i + j])) {
				printf("Invalid line %i (", i + j + 1);
				showStr(lines[i + j]);
				printf(")\n");
				for (int k = 0; lines[i + j + k] && strcmp(lines[j + i + k], "") && k < 11; k++) {
					printf("\tLine %i: ", i + j + k);
					showStr(lines[i + j + k]);
					printf("\n");
					if (k == 10)
						printf("\t...\n");
				}
				free(objs);
				free(lines[0]);
				free(lines);
				free(file_buffer);
				return (NULL);
			}
		objs[i / 3].id = atoi(lines[i]);
		objs[i / 3].pos.x = atoi(lines[i + 1]) * 16 - 16;
		objs[i / 3].pos.y = atoi(lines[i + 2]) * 16 - 16;
		objs[i / 3].layer = 1;
		objs[i / 3].solid = solidObjects[objs[i / 3].id - 1];
		objs[i / 3].footstepSound = IRON;
		objs[i / 3].footstepVariance = 1;
		if (objs[i / 3].id == 2) {
			objs[i / 3].footstepSound = GRASS1;
			objs[i / 3].footstepVariance = 4;
		} else if (objs[i / 3].id == 4) {
			objs[i / 3].footstepSound = DIRT1;
			objs[i / 3].footstepVariance = 4;
		} else if (objs[i / 3].id == 5) {
			objs[i / 3].action = DEAL_DAMAGES;
			objs[i / 3].damages[FIRE_DAMAGE] = 10;
			objs[i / 3].invulnerabiltyTime = 10;
			objs[i / 3].footstepSound = MUD1;
			objs[i / 3].footstepVariance = 4;
		} else if (objs[i / 3].id == 8) {
			objs[i / 3].action = CHANGE_MUSIC;
			objs[i / 3].invulnerabiltyTime = MYSTERIOUS;
		} else if (objs[i / 3].id == 9) {
			objs[i / 3].action = CHANGE_MUSIC;
			objs[i / 3].invulnerabiltyTime = MACABRE;
		} else if (objs[i / 3].id == 10) {
			objs[i / 3].action = CHANGE_MUSIC;
			objs[i / 3].invulnerabiltyTime = EPICBATTLE_START;
		} else if (objs[i / 3].id == 11) {
			objs[i / 3].action = DEAL_DAMAGES;
			objs[i / 3].damages[TRUE_DAMAGE] = -3;
			objs[i / 3].invulnerabiltyTime = 5;
		} else if (objs[i / 3].id == 12) {
			objs[i / 3].action = CHANGE_MUSIC;
			objs[i / 3].damages[FIRE_DAMAGE] = 10;
			objs[i / 3].invulnerabiltyTime = EUGRT_START;
		} else
			objs[i / 3].action = NONE;
		for (int j = 0; layer[j].object; j++)
			if (layer[j].object == objs[i / 3].id) {
				objs[i / 3].id = layer[j].replacement;
				objs[i / 3].layer = 2;
			}
		objs[i / 3 + 1].layer = 0;
	}
	free(lines[0]);
	free(lines);
	free(file_buffer);
	return (objs);
}

void	saveLevel(char *path, Object *objs)
{
	int	fd;
	char	*buffer = "test";
	char	*buf;
	int	ln = 0;

	printf("Saving to %s\n", path);
	fd = open(path, O_WRONLY | O_CREAT, 0664);
	if (fd < 0) {
		perror(path);
		return;
	}
	printf("Writing header (%s) (%i chars)\n", header, write(fd, header, strlen(header)));
	for (int i = 0; objs[i].layer; i++) {
		ln = 0;
		buffer = concatf("\n%i %i %i %i %i %i %f %i %i %i %i %i %i %i %i %i", objs[i].id, objs[i].pos.x, objs[i].pos.y, objs[i].layer, objs[i].solid, objs[i].action, objs[i].invulnerabiltyTime, objs[i].footstepSound, objs[i].footstepVariance, objs[i].damages[0], objs[i].damages[1], objs[i].damages[2], objs[i].damages[3], objs[i].damages[4], objs[i].damages[5], objs[i].damages[6]);
		buf = concatf("%S", buffer);
		for (int i = 0; buffer[i]; ln += buffer[i] == '\r', i++);
		printf("Writing object %i (%s) (%i chars and %i lines)\n", i, buf, write(fd, buffer, strlen(buffer)), ln);
		free(buffer);
		free(buf);
	}
	close(fd);
	printf("Converted file %s\n", path);
}

int	main(int argc, char **args)
{
	Object	*objs;

	if (argc == 1)
		printf("Usage:\n\t%s <path> [<path2> <path3> ...]", args[0]);
	for (int i = 1; i < argc; i++) {
		objs = loadLevel(args[i]);
		if (objs)
			saveLevel(args[i], objs);
		free(objs);
	}
}