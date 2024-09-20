#if !defined(_L_HASH_TABLE_TOOLS_L)
#define _L_HASH_TABLE_TOOLS_L
#include "hash_table.h"
#include "pair.h"

typedef struct BucketList
{
    bucket_t **buckets;
    size_t size;

} bucket_list_t;

bucket_list_t * bucket_list_create_ref(hash_table_t *table); // generates shared pointers for existing buckets in a hash table
void bucket_list_print(bucket_list_t *list, size_t limit);

void bucket_list_free(bucket_list_t *list);

void bucket_list_sort_q(bucket_list_t *bucket_list);
#endif // _L_HASH_TABLE_TOOLS_L
