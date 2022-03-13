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
