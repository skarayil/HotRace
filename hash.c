/* *********************************************************************** */
/*                                                                         */
/*                                                     :::      ::::::::   */
/* hash.c                                            :+:      :+:    :+:   */
/*                                                 +:+ +:+         +:+     */
/* By: ksener <ksener@student.42kocaeli.com.tr   +#+  +:+       +#+        */
/*                                             +#+#+#+#+#+   +#+           */
/* Created: 2026/09/12 15:44:15 by ksener          #+#    #+#              */
/* Updated: 2026/09/12 16:16:10 by ksener          ###   ########.fr       */
/*                                                                         */
/* *********************************************************************** */
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

static size_t hash_str(const char *key)
{
    size_t i;
    size_t h;

    h = (size_t)14695981039346656037ULL;
    i = 0;
    while(key[i])
    {
        h ^= (unsigned char)key[i];
        h *= (size_t)1099511628211ULL;
        i++;
    }
    return (h);
}

char    *table_search(t_table *t, const char *key)
{
    size_t  i;
    t_entry *e;
    if (!t || !key)
        return (NULL);
    i = hash_str(key) & (t->size - 1);
    e = t->buckets[i];
    while (e)
    {
        if (ft_str_cmp(e->key, key) == 0)
            return e->value;
        e = e->next;
    }
    return NULL;
}

void    table_free(t_table *t)
{
    size_t  i;
    t_entry e;
    t_entry next;
    if (!t)
        return ;
    i = 0;
    while (i < t->size)
    {
        e = t->buckets[i];
        while (e)
        {
            next = e->next;
            free(e);
            e = next;
        }
        i++;
    }
    free(t->buckets)
    free(t)
}


static void table_resize(t_table t)
{
    size_t  new_size;
    t_entry **new_buckets;
    t_entry e;
    t_entry next;
    size_t  i;
    size_t  j;

    new_size = t->size * 2;
    new_buckets = malloc(sizeof(t_entry *) * new_size);
    if !new_buckets
        return ;
    i = 0;
    while (i < new_size)
        new_buckets[i++] = NULL;
    i = 0;
    while (i < t_size)
    {
        e = t->backets[i];
        while (e)
        {
            next = e->next;
            j = hash_str(e->key) & (new_size -1);
            e->next = new_buckets[j];
            new_buckets[j] = e;
            e = next;
        }
        i++;
    }
    free(t->buckets);
    t->buckets = new_buckets;
    t->size = new_size;
}

void table_insert(t_table t, char *key,char *value)
{
    size_t i;
    t_entry e;
    if(!t || !key || !value)
        return ;
    i = hash_str(key) & (t->size - 1);
    e = t->buckets[i];
    while (e)
    {
        if (ft_strcmp(e->key, key) == 0)
        {
            e->value = value;
            return ;
        }
        e = e->next;
    }
    e = malloc(sizeof(t_entry));
    if (!e)
        return ;
    e->key = key;
    e->value = value;
    e->next = t->buckets[i];
    t->buckets[i] = e;
    t->count++;
    if (t->count > t->size * 3 / 4)
        table_resize(t);
    
}