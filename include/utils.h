#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdio.h>
#include "character.h"
#include "language.h"
#include "array.h"

char		**split(char *str, char separator);
int		get_nbrlen(int nbr);
Character	*getPlayer();
void		setVolumes(Array sounds, float volume);
int		dispMsg(char *title, char *content, int variate);
int		findLanguage(char *lang_id);
int		getLanguageArrayLen();
void		setFont(sfText *text, sfFont *font);
char		*getButtonContent(int nameId);
float		isKeyPressed(unsigned int keyID, sfRenderWindow *window);
char		*getKeyString(unsigned char keyID);
char		*getParentFolder(char *path);
char		*getAbsolutePath(char *path);
char		*getKeyString(unsigned char keyID);
char		*getButtonContent(int nameId);
ssize_t		getline(char **lineptr, size_t *n, FILE *stream);
int		playSoundEffect(sfSoundBuffer *buffer);

#if defined _WIN32 || defined __WIN32 || defined __WIN32__
	long	random();
	void	srandom(long seed);
#endif