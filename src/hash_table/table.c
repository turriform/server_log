#include "hash_table_internal.h"
#include "table.h"

void hash_table_update_capacity(hash_table_t *self)
{
    self->filled++;
    self->load_factor = (double)self->filled / (double)self->size;
}

bool hash_table_spot_available(hash_table_t *self, size_t key)
{
    return self->buckets[key]->exists == false;
}

bool hash_table_spot_existed(hash_table_t *self, size_t key)
{
    return self->buckets[key]->existed == true;
}

static inline bool
hash_table_value_match(hash_table_t *self, size_t key, char *value)
{
    return strncmp(self->buckets[key]->key_value.value, value, strlen(value)) == 0;
}

static inline bool is_prime(size_t n)
{
    if (n <= 1)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }

    for (size_t i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}

size_t next_prime(size_t table_size)
{
    if (table_size <= 1)
    {
        return 2;
    }

    size_t prime = table_size;
    bool found = false;

    while (!found)
    {
        prime++;
        if (is_prime(prime))
        {
            found = true;
        }
    }

    return prime;
}

bucket_t *hash_table_bucket_get(hash_table_t *hash_table, char *value)
{
    size_t key = hash_polinomial(hash_table->size, value);
    size_t jumps = hash_table->max_jumps;

    while (jumps--)
    {
        if (hash_table_value_match(hash_table, key, value))
        {
            printf("Found Bucket \n");

            return hash_table->buckets[key];
        }

        key++;
        key %= hash_table->size;
    }

    return NULL;
}

void hash_table_bucket_delete(hash_table_t *hash_table, char *value)
{
    bucket_t *bucket = hash_table_bucket_get(hash_table, value);
    if (bucket == NULL)
    {
        printf("Bucket not found \n");
        return;
    }
    bucket_print(bucket);

    bucket_update_on_delete(bucket);
}

bucket_t *hash_table_bucket_insert_with_count(hash_table_t *hash_table, char *value, size_t count)
{
    bucket_t *bucket = hash_table_bucket_insert(hash_table, value);
    bucket->key_value.count += count - 1;
    return bucket;
}

bucket_t *hash_table_bucket_insert(hash_table_t *hash_table, char *value)
{
    size_t key = hash_polinomial(hash_table->size, value);
    size_t jumps = 0;
    // printf("%ld \n", key);

    while (!hash_table_spot_available(hash_table, key))
    {
        if (hash_table_value_match(hash_table, key, value))
        {
            hash_table->buckets[key]->key_value.count++;
            return hash_table->buckets[key];
        }

        key++;
        key %= hash_table->size;
        jumps++;
    }

    hash_table->buckets[key]->exists = true;
    hash_table->buckets[key]->existed = true;
    hash_table->buckets[key]->key_value.jumps = jumps;
    hash_table->buckets[key]->key_value.value = malloc(strlen(value) + 1);
    if (hash_table->buckets[key]->key_value.value == NULL)
    {
        printf("Cannot allocate memory for value");
        exit(EXIT_FAILURE);
    }

    memcpy(hash_table->buckets[key]->key_value.value, value, strlen(value) + 1);
    hash_table->buckets[key]->key_value.key = key;
    hash_table->buckets[key]->key_value.count++;

    hash_table->max_jumps = MAX(hash_table->max_jumps, jumps);
    hash_table_update_capacity(hash_table);

    return hash_table->buckets[key];
}

void hash_table_insert(hash_table_t *self, char *val)
{
    if (self->load_factor > HASH_TABLE_MAX_LOAD)
    {
        hash_table_resize(self);
    }
    hash_table_bucket_insert(self, val);
}

void hash_table_insert_with_count(hash_table_t *self, char *val, size_t count)
{
    if (self->load_factor > HASH_TABLE_MAX_LOAD)
    {
        hash_table_resize(self);
    }
    hash_table_bucket_insert_with_count(self, val, count);
}

void hash_table_resize(hash_table_t *self)
{

    size_t old_size = self->size;
    self->size = next_prime(old_size * 2);
    self->filled = 0;
    self->load_factor = 0;
    self->max_jumps = 0;

    bucket_t **old_buckets = self->buckets;

    self->buckets = calloc(self->size, sizeof(bucket_t *));
    if (self->buckets == NULL)
    {
        printf("Error occured while allocating memory\n");
        return;
    }

    // filling new buckets
    bucket_t *bucket;
    for (size_t i = 0; i != self->size; i++)
    {
        bucket = bucket_create(i);
        self->buckets[i] = bucket;
    }

    for (size_t i = 0; i != old_size; i++)
    {
        // rewriting old buckets to new
        if (old_buckets[i]->exists)
        {
            bucket_t *new_bucket = hash_table_bucket_insert(self, old_buckets[i]->key_value.value);
            new_bucket->key_value.count = old_buckets[i]->key_value.count;
        }
        bucket_destroy(old_buckets[i]);
    }
    free(old_buckets);
}

hash_table_t *hash_table_create(void)
{
    size_t initial_capacity = HASH_TABLE_INITIAL_SZ;
    hash_table_t *hash_table = malloc(sizeof(hash_table_t));
    if (hash_table == NULL)
    {
        printf("Error occured while allocating memory\n");
        return NULL;
    }

    hash_table->buckets = calloc(initial_capacity, sizeof(bucket_t *));
    if (hash_table->buckets == NULL)
    {
        printf("Error occured while allocating memory\n");
        return NULL;
    }

    bucket_t *bucket;
    for (size_t i = 0; i != initial_capacity; i++)
    {
        bucket = bucket_create(i);
        hash_table->buckets[i] = bucket;
    }

    hash_table->size = (size_t)initial_capacity;
    hash_table->filled = 0;
    hash_table->load_factor = (double)hash_table->filled / (double)hash_table->size;
    hash_table->max_jumps = 0;

    return hash_table;
}

void hash_table_destroy(hash_table_t *self)
{
    for (size_t i = 0; i != self->size; i++)
    {
        bucket_destroy(self->buckets[i]);
    }

    free(self->buckets);
    free(self);
}

void hash_table_print(hash_table_t *self)
{
    printf("-----------------------------------------\n*\t\tHash table\t\t*\n-----------------------------------------\n");
    printf("\tSize:\t\t\t%ld \n", self->size);
    printf("\tFilled:\t\t\t%ld \n", self->filled);
    printf("\tLoad:\t\t\t%.2f \n", self->load_factor);
    printf("\tMax Jumps (alpha):\t%ld \n\n\n", self->max_jumps);

    // size_t limit = (self->size < 10) ? self->size : 10;

    // for (size_t i = 0; i != self->size; i++)
    // for (size_t i = 0; i != limit; i++)

    // {
    //     if (self->buckets[i]->exists)
    //     {
    //         bucket_print(self->buckets[i]);
    //     }
    // }
    // printf("\n\n");
}
