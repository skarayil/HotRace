#include "hotrace.h"

t_table *table_create(size_t size)
{
    t_table *t;
    size_t  i;

    t = malloc(sizeof(t_table));
    if (!t)
        return NULL;
    t->buckets = malloc(sizeof(t_entry *) * size);
    if (!t->buckets)
    {
        free(t);
        return NULL;
    }
    i = 0;
    while(i < size)
    {
        t->buckets[i] = NULL;
        i++;
    }
    t->size = size;
    t->count = 0;
    return (t);
}