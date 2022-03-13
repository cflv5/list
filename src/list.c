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

    // TODO: define foreach function
    list_foreach(*list, deallocator); 

    free(*list);
    *list = NULL;

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