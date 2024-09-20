#if !defined(L_HASH_TABLE_H)
#define L_HASH_TABLE_H
#include "hash_table_internal.h"
#include "pair.h"

typedef struct HashTable
{
    size_t size;
    size_t filled;
    size_t max_jumps;
    size_t unique;
    double load_factor;
    bucket_t **buckets;

} hash_table_t;



hash_table_t * 
hash_table_create(void);

void 
hash_table_destroy(hash_table_t *self);

void 
hash_table_print(hash_table_t *self);

void 
hash_table_update_capacity(hash_table_t *self);

bool 
hash_table_spot_available(hash_table_t *self, size_t key);

bool 
hash_table_spot_existed(hash_table_t *self, size_t key);



bucket_t *
hash_table_bucket_get(hash_table_t *hash_table, char *value);

void 
hash_table_bucket_delete(hash_table_t *hash_table, char *value);

bucket_t *
hash_table_bucket_insert(hash_table_t *hash_table, char *value); 

void 
hash_table_insert(hash_table_t *self, char *val); // main function to insert the value into the table

void 
hash_table_insert_with_count(hash_table_t *self, char *val, size_t count); // main function to insert the value into the table

void 
hash_table_resize(hash_table_t *self); //table resize at max capacity


#endif