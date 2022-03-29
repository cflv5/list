#ifndef __LIST_ERROR_H__
#define __LIST_ERROR_H__

#define MAX_ERROR_STR_SIZE 50

#define LIST_OK 0

#define ERROR_NULL_POINTER 1
#define ERROR_MEM_ALLOC 2
#define ERROR_INDEX_OUT_OF_BOUND 3
#define ERROR_PREDICATE_FAILED 4

#define WARNING_EMPTY_LIST -1

int list_code_to_string(int error_code, char *str, int size);

#endif // !__LIST_ERROR_H__