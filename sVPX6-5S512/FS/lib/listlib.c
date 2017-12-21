#include "listLib.h"

static void listDel(LIST_HEAD_S *prev,LIST_HEAD_S *next)
{
	next->prev = prev;
	prev->next = next;
}

void listAdd(LIST_HEAD_S *newNode,LIST_HEAD_S *prev,LIST_HEAD_S *next)
{
	next->prev = newNode;
	newNode->next = next;
	newNode->prev = prev;
	prev->next = newNode;
}

void listInitHead(LIST_HEAD_S *list)
{
	list->next = list;
	list->prev = list;
}


/**
 * listDelInit - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
void listDelInit(LIST_HEAD_S *entry)
{
	listDel(entry->prev, entry->next);
	listInitHead(entry);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void listAddTail(LIST_HEAD_S *newNode,LIST_HEAD_S *head)
{
	listAdd(newNode, head->prev, head);
}



