#include "hash_table_internal.h"
#include "table.h"
#include "pair.h"
#include "hash_table_tools.h"


//comparators
bool bucket_lt_by_count(bucket_t *self, bucket_t *other)
{
    return self->key_value.count >= other->key_value.count;
}

bool bucket_gt_by_count(bucket_t *self, bucket_t *other)
{
    return self->key_value.count < other->key_value.count;
}


bucket_list_t *bucket_list_create_ref(hash_table_t *table)
{
    // return an array of buckets ordered by bucket.pair.count

    bucket_list_t *bucket_list = malloc(sizeof(bucket_list_t));
    if (bucket_list == NULL)
    {
        return NULL;
    }

 
    bucket_list->size = table->filled;

    bucket_list->buckets = calloc(table->filled, sizeof bucket_list->buckets[0]);

    if (bucket_list->buckets == NULL)
    {
        printf("Error allocating sorted_buckets");
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i != table->size; i++)
    {
        bucket_t *b = table->buckets[i];

        if (b->exists)
        {
            bucket_list->buckets[j++] = b;
        }
    }

    return bucket_list;
}

void bucket_list_print(bucket_list_t *list, size_t limit)
{
    size_t lim = MIN(list->size, limit);

    for (size_t i = 0; i != lim; i++)
    {
        key_value_t kv = list->buckets[i]->key_value;
        printf("[count: %ld\t][value: %s\n", kv.count, kv.value);
    }
}

void bucket_list_free(bucket_list_t *bucket_list)
{
    if (bucket_list != NULL)
    {
        // real buckets are only deleted at hash table destruction
        free(bucket_list->buckets);
        free(bucket_list);
    }
}

/// sorting

static inline void
bucket_list_sort_q_swap(bucket_t **first, bucket_t **second)
{
    bucket_t *tmp = *second;
    *second = *first;
    *first = tmp;
}

size_t
bucket_list_sort_q_partition(bucket_t **buckets, size_t low, size_t high, bool (*comparator)(bucket_t *, bucket_t *))
{

    bucket_t *pivot = buckets[high];

    int i = low - 1;

    for (size_t j = low; j != high; j++)
    {
        if (comparator(pivot, buckets[j]))
        {
            i++;
            bucket_list_sort_q_swap(&buckets[i], &buckets[j]);
        }
    }

    bucket_list_sort_q_swap(&buckets[i + 1], &buckets[high]);

    return i + 1;
}

void bucket_list_sort_q_init(bucket_t **bucket_list, size_t low, size_t high)
{
    if (low < high)
    {
        size_t part = bucket_list_sort_q_partition(bucket_list, low, high, bucket_gt_by_count);
        bucket_list_sort_q_init(bucket_list, low, part - 1);
        bucket_list_sort_q_init(bucket_list, part + 1, high);
    }
}

void bucket_list_sort_q(bucket_list_t *bucket_list)
{
    bucket_list_sort_q_init(bucket_list->buckets, 0, bucket_list->size - 1);
}