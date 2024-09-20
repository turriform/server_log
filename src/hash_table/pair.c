#include "hash_table_internal.h"
#include "table.h"
#include "pair.h"

size_t hash_polinomial(size_t table_size, char *value)
{

    size_t init_factor = 1, a = 97;
    size_t i = 0, res = 0;
    char *str = value;
    while (str[i])
    {
        res += (init_factor * (size_t)str[i]) % table_size;
        init_factor = (init_factor * a) % table_size;
        i++;
    }
    return res % table_size;
}

void bucket_destroy(bucket_t *self)
{
    if (self != NULL)
    {
        if (self->key_value.value!=NULL){
            free(self->key_value.value);
        }
        free(self);
    }
}

void bucket_update_on_delete(bucket_t *self)
{
    self->exists = false;
    self->key_value.jumps = 0;
    self->key_value.count = 0;
}

void bucket_calculate_hash(bucket_t *self, size_t table_size, char *value)
{
    size_t hash = hash_polinomial(table_size, value);

    self->key_value.key = hash;
}

void bucket_print(bucket_t *self)
{
    printf("Bucket: ");

    printf("[jumps:%ld\t] ", self->key_value.jumps);
    printf("[count:%ld\t] ", self->key_value.count);
    printf("[exists:%s\t] ", self->exists ? "Yes" : "No");
    printf("[existed:%s\t] ", self->existed ? "Yes" : "No");
    printf("[key:\t%ld\t] ", self->key_value.key);
    printf("[val:\t%s]", self->key_value.value ? self->key_value.value : "N\\A");
    printf("\n");
}

void bucket_hash_jump(bucket_t *self, size_t table_size)
{
    self->key_value.jumps++;
    self->key_value.key++;
    self->key_value.key %= table_size;
}

bucket_t *bucket_create(size_t key)
{
    bucket_t *bucket = malloc(sizeof(bucket_t));

    bucket->self = bucket;
    bucket->existed = false;
    bucket->exists = false;
    bucket->key_value.jumps = 0;
    bucket->key_value.count = 0;
    bucket->key_value.key = key;
    bucket->key_value.value = NULL;

    return bucket;
}
