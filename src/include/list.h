#ifndef __LIST_H_
#define __LIST_H_

#include "list_error.h"

/**
 * @brief Represents an operation applied on given item
 */
typedef void *(list_consumer)(void *item);

/**
 * @brief Evaluates the predicate on given item
 *
 * @return 0 if false, anything else if true
 */
typedef int *(list_predicate)(void *item);

/**
 * @brief Compares i1 to i2
 *
 * @return -1 if i1 > i2, 0 if i1 == i2, 1 if i1 < i2
 */
typedef int *(list_comperator)(void *i1, void *i2);

/**
 * @brief
 *
 */
typedef int *(list_sort_strategy)(struct list_node *items, int list_size);

struct list;

struct list_node;

struct list_iterator;

/**
 * @brief Creates an empty list
 *
 * @return empty list struct
 */
struct list *list_create();

/**
 * @brief Deallocates memory assigned to the list,
 * does not deletes individual items. \n
 * Assigns null to the referenced list
 *
 * @param list address of a list pointer
 */
void list_delete(struct list **list);

/**
 * @brief Deletes list and its items,
 * assigns null to the referenced list
 *
 * @param list address of a list pointer
 */
void list_delete_dynamic(struct list **list);

/**
 * @brief Adds an item to the list after the element on which
 * predicate evaluated true
 *
 * @param list address of a list pointer
 * @param item address of the item
 * @param predicate predicate to be evaluated
 * @param options TODO: flag to
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_add(struct list *list, void *item, list_predicate *predicate, short options);

/**
 * @brief Adds an item to the end of the list
 *
 * @param list address of a list
 * @param item address of the item
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_add_end(struct list *list, void *item);

/**
 * @brief Adds an item to the beginning of the list
 *
 * @param list address of a list
 * @param item address of the item
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_add_head(struct list *list, void *item);

/**
 * @brief Adds an item after the indexed element @n
 * if supplied index is grater then list size returnes
 * <i>ERROR_INDEX_OUT_OF_BOUND</i>
 *
 * @param list address of a list
 * @param item address of the item
 * @param index index of the element
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_add_index(struct list *list, void *item, unsigned int index);

/**
 * @brief Applies consumer function to every element in the list
 * @param list address of a list
 * @param consumer function pointer that is applied to the elements
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_foreach(struct list *list, list_consumer *consumer);

/**
 * @brief Removes an item by evaluating the predicate
 *
 * @param list address of a list
 * @param predicate predicate function to be evaluated
 * @param options TODO: flag to
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_remove(struct list *list, list_predicate *predicate, short options);

/**
 * @brief Removes an item at an index from the list
 * @param list address of a list
 * @param index index of the element
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_remove_index(struct list *list, unsigned int index);

/**
 * @brief
 *
 * @param list address of a list
 * @param predicate predicate function to be evaluated
 * @param options TODO: flag to
 * @return address of the item if found, else null
 */
void *list_get(struct list *list, list_predicate *predicate, short options);

/**
 * @brief Sorts the list using bubble sort
 *
 * @param list address of the list to be sorted
 * @param comperator function pointer to compare items
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_sort(struct list *list, list_comperator *comperator);

/**
 * @brief Sorts the list using supplied strategy
 *
 * @param list address of the list to be sorted
 * @param strategy function pointer to a strategy
 * @return 0 if successful, positive for error codes, negative for warnings
 */
int list_sort_by(struct list *list, list_sort_strategy *strategy);

#endif // !__LIST_H_