#include "configParser.h"
#include <string.h>
#include <malloc.h>

ParserObj	*ParserObj_getElement(ParserObj *list, char *index)
{
	for (; list; list = list->next)
		if (strcmp(list->index, index) == 0)
			return (list);
	return (NULL);
}

bool	ParserObj_addElement(ParserObj *list, void *data, ParserTypes type, char *index)
{
	for (; list->next; list = list->next) {
		if (!list->index)
			break;
		if (strcmp(list->index, index) == 0)
			return (false);
	}
	if (list->index && strcmp(list->index, index) == 0)
		return (false);
	if (list->index) {
		list->next = malloc(sizeof(*list->next));
		if (!list->next)
			return (false);
		list->next->next = NULL;
		list->next->prev = list;
		list = list->next;
	}
	list->data = copyData(data, getSizeOf(type));
	list->index = index;
	list->type = type;
	return (true);
}

void	destroyObjEntry(ParserObj *list)
{
	ParserObj	buff;

	memset(&buff, 0, sizeof(buff));
	Parser_destroyData(list->data, list->type);
	if (!list->prev) {
		if (list->next) {
			buff = *list->next;
			buff.prev = NULL;
		}
		free(list->next);
		*list = buff;
		return;
	}
	list->prev->next = list->next;
	if (list->next)
		list->next->prev = list->prev;
	free(list);
}

void	ParserObj_delElement(ParserObj *list, char *index)
{
	for (; list; list = list->next)
		if (strcmp(list->index, index) == 0) {
			destroyObjEntry(list);
			return;
		}
}

void	ParserObj_destroy(ParserObj *list)
{
	for (; list->next; list = list->next) {
		Parser_destroyData(list->data, list->type);
		free(list->index);
		free(list->prev);
	}
	Parser_destroyData(list->data, list->type);
	free(list->prev);
	free(list->index);
	free(list);
}