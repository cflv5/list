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

#define ADD_BEFORE_INDICATOR 1
#define ADD_AFTER_INDICATOR 2

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

int list_add_after(struct list *list, void *item, list_predicate *predicate)
{
    return add_item_to_list_generic(list, item, predicate, ADD_AFTER_INDICATOR);
}

int list_add_before(struct list *list, void *item, list_predicate *predicate)
{
    return add_item_to_list_generic(list, item, predicate, ADD_BEFORE_INDICATOR);
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

/**
 * @brief 
 * 
 * @param list list to add item
 * @param item item that is wanted to be added
 * @param predicate predicate function to decide item index
 * @param position wheter item is position before or after the predicate index
 * 
 * @return int 
 */
static int add_item_to_list_generic(struct list *list, void *item, list_predicate *predicate, int position)
{
    struct list_node *node;
    struct list_node *curr;
    int cont_f = 1;
    
    if (list == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    pthread_mutex_lock(&list->lock);
    node = (struct list_node *)malloc(sizeof(struct list_node));
    node->item = item;
    node->next = NULL;
    node->prev = NULL;

    curr = list->list;
    while (curr != NULL && cont_f)
    {
        if (predicate(curr->item))
        {
            cont_f = 0;
        }

        curr = curr->next;
    }

    if (curr == NULL)
    {
        return ERROR_PREDICATE_FAILED;
    }

    node->next = curr->next;
    curr->next = node;
    node->prev = curr;

    return LIST_OK;
}