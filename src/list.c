/**
 * @file list.c
 * @author Bedirhan AKÇAY (bedirhanakcay@icloud.com)
 * @brief
 * @version 0.1
 * @date 2022-03-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "include/list.h"

#include <pthread.h>
#include <stdlib.h>

struct list_node
{
    void *item;
    struct list_node *next;
    struct list_node *prev;
};

struct list
{
    int size;
    struct list_node *list;

    pthread_mutex_t lock;
};

// static function declerations
static void free_item(void *item);

// public function definitions

struct list *list_create()
{
    struct list *list = malloc(sizeof(struct list));
    if (list == NULL)
    {
        return NULL;
    }

    list->list = NULL;
    list->size = 0;

    if (pthread_mutex_init(&list->lock, NULL) != 0)
    {
        list_delete(&list);
        return NULL;
    }

    return list;
}

int list_delete(struct list **list)
{
    if (list == NULL || *list == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    pthread_mutex_lock(&(*list)->lock);

    free(*list);
    *list = NULL;

    return LIST_OK;
}

int list_delete_dynamic(struct list **list)
{
    return list_delete_dynamic_by(list, free_item);
}

int list_delete_dynamic_by(struct list **list, list_consumer *deallocator)
{
    if (list == NULL || *list == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    pthread_mutex_lock(&(*list)->lock);

    l_foreach(*list, deallocator);
    delete_nodes(*list);

    free(*list);
    *list = NULL;

    return LIST_OK;
}

int list_foreach(struct list *list, list_consumer *consumer)
{
    if (list == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    pthread_mutex_lock(&list->lock);

    if (list->size == 0 || list->list == NULL)
    {
        return WARNING_EMPTY_LIST;
    }

    l_foreach(list, consumer);

    pthread_mutex_unlock(&list->lock);

    return LIST_OK;
}

// static function definitions

/**
 * @brief consumer function that frees memmory assigned to an item
 *
 * @param item
 */
static void free_item(void *item)
{
    if (item != NULL)
    {
        free(item);
    }
}

/**
 * @brief Iterates over the elements of the list
 *
 * @param list list to be iterated. Must be non-null.
 * @param consumer operation to be applied
 */
static void l_foreach(struct list *list, list_consumer *consumer)
{
    struct list_node *curr = list->list;
    while (curr != NULL)
    {
        consumer(curr->item);
        curr = curr->next;
    }
}

/**
 * @brief Deletes all nodes of a list
 */
static void delete_nodes(struct list *list)
{
    struct list_node *curr = list->list;
    while (curr != NULL)
    {
        free(curr);
        curr = curr->next;
    }
    list->list = NULL;
}