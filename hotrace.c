#include <stdio.h>

int	main(void)
{
	char	*input;
    t_table	*table;

	input = read_all(0);
	if (!input)
		return (1);
	table = table_create(1024);
	if (!table)
	{
		free(input);
		return (1);
	}
    parse_pairs(table, input, &cursor);
    process_queries(table, cursor);
    table_free(table);
    free(input);
    return 0;
}
