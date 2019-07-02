#ifndef __CREATORS_FUNCTIONS_
#define __CREATORS_FUNCTIONS_

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>

sfText			*createText();
sfSound			*createSound();
void			*protectedMalloc(size_t size);
sfSprite		*createSfSprite(sfTexture *texture);
sfRenderWindow		*createMainWindow();
sfRenderWindow		*createRenderWindow(sfVideoMode mode, const void *title, sfUint32 style, const sfContextSettings *settings, bool unicode);
sfRectangleShape	*createRectangleShape();

#endif