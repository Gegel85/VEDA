NAME =	VEDA

SRC = 	main.c		\
	initGame.c	\
	concat.c	\
	loadSprites.c	\
	loadSounds.c	\
	launchGame.c	\
	manageEvents.c	\
	mainMenu.c	\
	drawSprite.c	\
	settingsMgr.c	\
	load_buttons.c	\
	callbacks.c	\
	int_to_str.c	\
	loadFonts.c	\
	get_nbrlen.c	\
	loadLanguages.c	\
	clic_button.c	\
	loadLevel.c	\
	inGame.c	\
	settings.c	\
	saveFileMgr.c	\
	dispMsg.c	\
	change_buffer.c	\

OBJ =	$(SRC:%.c=src/%.o)

INC =	-Iinclude		\
	-Ilib/concatf/include	\

LDFLAGS =			\
	-lcsfml-audio		\
	-lcsfml-graphics	\
	-lcsfml-network		\
	-lcsfml-system		\
	-lcsfml-window		\
	-lm			\
	-lconcatf		\
	-L lib/concatf		\

CFLAGS= $(INC)	\
	-W	\
	-Wall	\
	-Wextra	\

CC =	gcc

RULE =	all

RES =	

all:    $(NAME)

windows:RES = icon.res
windows:icon.res all

icon.res:
	windres icon.rc -O coff -o icon.res

$(NAME):$(OBJ)
	$(MAKE) -C lib/concatf $(RULE)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS) $(RES)

clean:
	$(MAKE) -C lib/concatf clean
	$(RM) $(OBJ)
	$(RM) icon.res

fclean:	clean
	$(MAKE) -C lib/concatf fclean
	$(RM) $(NAME) $(NAME).exe

re:	fclean all

dbg:	CFLAGS += -g -O0
dbg:	RULE = dbg
dbg:	re

epi:	LDFLAGS = -lc_graph_prog -lm -lconcatf -L lib/concatf
epi:	dbg