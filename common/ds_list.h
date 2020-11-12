#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include "default.h"

struct list_node
{
    struct list_node *prev;
    struct list_node *next;
};

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


DEF_INLINE void list_init(struct list_node *l)
{
    l->next = l->prev = l;
}


/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
DEF_INLINE void list_insert_after(struct list_node *l, struct list_node *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
DEF_INLINE void list_insert_before(struct list_node *l, struct list_node *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
DEF_INLINE void list_remove(struct list_node *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
DEF_INLINE int list_isempty(const struct list_node *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
DEF_INLINE unsigned int list_len(const struct list_node *l)
{
    unsigned int len = 0;
    const struct list_node *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define list_entry(node, type, member) \
    container_of(node, type, member)

/**
 * rt_list_for_each - iterate over a list
 * @pos:	the struct list_node * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * rt_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the struct list_node * to use as a loop cursor.
 * @n:		another struct list_node * to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * rt_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * rt_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member), \
         n = list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * rt_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)


/**************************** single list *************/
struct slist_node
{
    struct slist_node *next;
};

DEF_INLINE void slist_init(struct slist_node *l)
{
    l->next = (void *)0;
}

DEF_INLINE void slist_append(struct slist_node *l,struct slist_node *n)
{
    struct slist_node *node;
    node = l;
    while(node->next)
        node = node->next;
    node->next = n;
    n->next = (void *)0;
}

DEF_INLINE void slist_insert(struct slist_node *l,struct slist_node *n)
{
    n->next = l->next;
    l->next = n;
}


DEF_INLINE unsigned int slist_len(const struct slist_node *l)
{
    unsigned int len = 0;
    const struct slist_node *list = l->next;
    while (list != NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

DEF_INLINE struct slist_node *slist_remove(struct slist_node *l, struct slist_node *n)
{
    /* remove slist head */
    struct slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (struct slist_node *)0) node->next = node->next->next;

    return l;
}

DEF_INLINE struct slist_node *slist_first(struct slist_node *l)
{
    return l->next;
}

DEF_INLINE struct slist_node *slist_tail(struct slist_node *l)
{
    while (l->next) l = l->next;

    return l;
}

DEF_INLINE struct slist_node *slist_next(struct slist_node *n)
{
    return n->next;
}

DEF_INLINE int slist_isempty(struct slist_node *l)
{
    return l->next == NULL;
}

/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define slist_entry(node, type, member) \
    container_of(node, type, member)

/**
 * rt_slist_for_each - iterate over a single list
 * @pos:    the rt_slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define rt_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define slist_for_each_entry(pos, head, member) \
    for (pos = slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != ((void *)0); \
         pos = slist_entry(pos->member.next, typeof(*pos), member))

/**
 * slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define slist_first_entry(ptr, type, member) \
    slist_entry((ptr)->next, type, member)

/**
 * slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define slist_tail_entry(ptr, type, member) \
    slist_entry(slist_tail(ptr), type, member)

#endif // __LIST_H
