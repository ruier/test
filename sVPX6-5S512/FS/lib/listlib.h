#ifndef __LIST_LIB_H__
#define __LIST_LIB_H__

struct tagLIST_HEAD_S;

typedef struct tagLIST_HEAD_S 
{
	struct tagLIST_HEAD_S *next;
	struct tagLIST_HEAD_S *prev;
}LIST_HEAD_S;


#define listEntry(link, type, member) \
	((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))

#define listHead(list, type, member)		\
	listEntry((list)->next, type, member)
	
#define listNext(elm, member)					\
	listEntry((elm)->member.next, typeof(*elm), member)
	

#define listForEachEntry(pos, list, member)			\
	for (pos = listHead(list, typeof(*pos), member);	\
	     &pos->member != (list);				\
	     pos = listNext(pos, member))

#define listEntry(link, type, member) \
		((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))
	
#define listForEachSafe(pos,n,head) \
       for(pos = (head)->next, n = pos->next; pos != (head); pos = n, \
	   	   n = pos->next)
	

extern void listAdd(LIST_HEAD_S *newNode,LIST_HEAD_S *prev,LIST_HEAD_S *next);

extern void listAddTail(LIST_HEAD_S *newNode,LIST_HEAD_S *head);

#endif





