#include "structs.h"
#include "macros.h"
#include "functions.h"
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int	getLanguage(Language *languages, char *lang_id)
{
	for (int i = 0; languages[i].name; i++)
		if (!strcmp(languages[i].id, lang_id))
			return (i);
	return (-1);
}

Language	createLanguage(char *path)
{
	Language	language;
	size_t		n = 0;
	int		len = 1;
	FILE		*stream;
	char		*buffer = NULL;
	char		*path_buffer = concat("data/languages/", path, false, false);

	memset(&language, 0, sizeof(language));
	if (!path_buffer) {
		printf("%s: Couldn't concatenate '%s' with '%s'\n", FATAL, "data/languages/", path);
		exit(EXIT_FAILURE);
	}
	printf("%s: Loading language folder %s\n", INFO, path_buffer);
	strcpy(language.id, path);
	buffer = concat(path_buffer, "/buttons.txt", false, false);
	if (!buffer) {
		printf("%s: Couldn't concatenate '%s' with '%s'\n", FATAL, buffer, "/name.txt");
		exit(EXIT_FAILURE);
	}
	stream = fopen(buffer, "r");
	if (!stream) {
		printf("%s: Couldn't open file %s (%s)\n", ERROR, buffer, strerror(errno));
		language.buttons = NULL;
	} else {
		n = 0;
		language.buttons = malloc(sizeof(*language.buttons));
		language.buttons[0] = NULL;
		for (len = 0; getline(&language.buttons[len], &n, stream) >= 0; len++, n = 0) {
			language.buttons = realloc(language.buttons, sizeof(*language.buttons) * (len + 3));
			if (!language.buttons) {
				printf("%s: Couldn't allocate %liB\n", FATAL, sizeof(*language.buttons) * (len + 3));
				exit(EXIT_FAILURE);
			}
			if (language.buttons[len] && language.buttons[len][strlen(language.buttons[len]) - 1] == '\n')
				language.buttons[len][strlen(language.buttons[len]) - 1] = 0;
			language.buttons[len + 1] = NULL;
		}
		language.buttons[len + 1] = NULL;
		fclose(stream);
	}
	free(buffer);
	buffer = concat(path_buffer, "/name.txt", false, false);
	if (!buffer) {
		printf("%s: Couldn't concatenate '%s' with '%s'\n", FATAL, buffer, "/name.txt");
		exit(EXIT_FAILURE);
	}
	stream = fopen(buffer, "r");
	if (!stream) {
		printf("%s: Couldn't open file %s (%s)\n", ERROR, buffer, strerror(errno));
		language.name = strdup(path);
	} else {
		n = 0;
		language.name = NULL;
		getline(&language.name, &n, stream);
		fclose(stream);
	}
	free(buffer);
	free(path_buffer);
	return (language);
}

Language	*loadLanguages(game_t *game)
{
	int		len = 0;
	char		*buffer = NULL;
	Language	*languages = NULL;
	char		**paths = NULL;
	DIR		*dir = opendir("data/languages");
	struct dirent	*dirEntry = NULL;

	if (!dir) {
		printf("%s: Couldn't find the languages directory\n", ERROR);
		return (NULL);
	}
	for (dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
		if (dirEntry->d_name[0] != '.') {
			len++;
			paths = realloc(paths, sizeof(*paths) * (len + 1));
			if (!paths) {
				printf("%s: Couldn't allocate %liB\n", FATAL, sizeof(*paths) * len);
				exit(EXIT_FAILURE);
			}
			paths[len - 1] = strdup(dirEntry->d_name);
		}
	}
	languages = malloc(sizeof(*languages) * (len + 1));
	if (!languages) {
		printf("%s: Couldn't allocate %liB\n", FATAL, sizeof(*languages) * (len + 1));
		exit(EXIT_FAILURE);
	}
	printf("%s: Loading %i languages\n", INFO, len);
	for (int i = 0; i < len; i++) {
		buffer = concat("Loading languages (", concat(paths[i], ")", false, false), false, true);
		displayLoadingBar(game, 5, MAX_STEPS, i, len, buffer);
		free(buffer);
		languages[i] = createLanguage(paths[i]);
		free(paths[i]);
	}
	free(paths);
	languages[len].name = NULL;
	printf("%s: Languages loaded !\n", INFO);
	closedir(dir);
	return (languages);
}
