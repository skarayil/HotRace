/* *********************************************************************** */
/*                                                                         */
/*                                                     :::      ::::::::   */
/* parser.c                                          :+:      :+:    :+:   */
/*                                                 +:+ +:+         +:+     */
/* By: ksener <ksener@student.42kocaeli.com.tr   +#+  +:+       +#+        */
/*                                             +#+#+#+#+#+   +#+           */
/* Created: 2026/09/12 15:44:13 by ksener          #+#    #+#              */
/* Updated: 2026/09/12 15:50:42 by ksener          ###   ########.fr       */
/*                                                                         */
/* *********************************************************************** */

#include "hotrace.h"

char *next_line(char **cursor)
{
    char *line;
    char *p;

    if(!cursor || !*cursor || !**cursor)
        return NULL;
    p = *cursor;
    line = p;
    while(*p && *p != '\n')
        p++;
    if (*p == '\n')
    {
        *p = '\0';
        p++;
    }
    *cursor = p;
    return (line);
}


void pars_pairs(t_table table, char *input, char **end)
{
    char *cursor;
    char *key;
    char *value;
    
    cursor = input;
    while (1)
    {
        key = next_line(&cursor);
        if (!key || *key=='\0')
            break ;
        value = next_line(&cursor);
        if (!value)
            break ;
        table_insert(table, key, value);
    }
}