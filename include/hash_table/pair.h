#if !defined(LOCAL_PAIR_H)
#define LOCAL_PAIR_H
#include "hash_table_internal.h"

typedef struct KeyValue
{
    size_t key;
    size_t jumps;
    size_t count;
    char *value;
} key_value_t;

typedef struct Bucket
{
    struct Bucket *self;

    bool existed;
    bool exists;

    key_value_t key_value;


} bucket_t;


bucket_t *bucket_create(size_t key);

void bucket_destroy(bucket_t *self);

void bucket_update_on_delete(bucket_t *self);

void bucket_calculate_hash(bucket_t *self, size_t table_size, char *value);

void bucket_print(bucket_t *self);

void bucket_hash_jump(bucket_t *self, size_t table_size);

//

bool bucket_lt_by_count(bucket_t *self, bucket_t *other); // bucket cpmparator by count, self is larger

size_t hash_polinomial(size_t table_size, char *value);

#endif // _LOCAL_PAIR_H
