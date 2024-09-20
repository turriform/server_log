#include "log_summary_internal.h"
#include "log_summary.h"
#include "log_line.h"

#define ASCII_SPACE 32U
#define ASCII_N 10U

#define PATH_SEP "/"

typedef enum Status
{
    STATUS_info,
    STATUS_ok = 0,
    STATUS_err

} status_t;

typedef struct ThreadParams
{
    log_summary_t *self;
    hash_table_t *t_refs;
    hash_table_t *t_urls;
    size_t total_bytes;
    size_t thread_n;
    void *mutex;

} pthread_params_t;

#define STATUS_CRITICAL STATUS_err

void log_summary_status(status_t status, const char *msg)
{
    printf("Status message: %s %s\n", msg, strerror(errno));

    if (status >= STATUS_CRITICAL)
    {

        printf("Critical... exiting\n");
        exit(EXIT_FAILURE);
    }
}

size_t *log_summary_get_optimal_file_idx(size_t n_threads, size_t num_files)
{

    size_t rows = (size_t)(num_files / n_threads) + (size_t)((num_files % n_threads) ? 1 : 0);

    size_t start = 0;
    size_t end = num_files - 1;

    size_t *arr_res = calloc(rows * n_threads, sizeof(size_t));

    while (start < end)
    {
        for (size_t i = 0; i != rows; i++)
        {
            for (size_t j = 0; j != n_threads; j++)
            {
                if (i % 2 == 0)
                {
                    arr_res[i * n_threads + j] = start + 1;
                    start++;
                }
                else
                {
                    arr_res[i * n_threads + j] = end + 1;
                    end--;
                }
            }
        }
    }

    // zeroes
    for (size_t i = num_files; i != rows * n_threads; i++)
    {
        arr_res[i] = 0;
    }

    return arr_res;
}

log_summary_t *log_summary(const char *dirname, size_t n_threads)
{

    log_summary_t *log_summary = malloc(sizeof(log_summary_t));
    if (log_summary == NULL)
    {
        return NULL;
    }

    log_summary->files = hash_table_create();
    if (log_summary->files == NULL)
    {

        free(log_summary);
        return NULL;
    }

    log_summary->n_threads = n_threads;
    log_summary->n_files = 0;

    ///

    DIR *dir = opendir(dirname);
    if (dir == NULL){
        log_summary_status(STATUS_err, "Error opening folder");
    }

    struct dirent *dn;
    struct stat fs;

    char abs_path[LOG_BUF] = "";

    while ((dn = readdir(dir)) != NULL)
    {
        if (dn->d_name[0] != '.')

        {
            getcwd(abs_path, LOG_BUF);
            str_n_cat(abs_path, PATH_SEP);
            str_n_cat(abs_path, dirname);
            str_n_cat(abs_path, PATH_SEP);
            str_n_cat(abs_path, dn->d_name);

            FILE *fp = fopen(abs_path, "r");
            if (fp == NULL)
            {
                log_summary_status(STATUS_err, "Cannot open file");
            }
            fstat(fileno(fp), &fs);
            hash_table_insert_with_count(log_summary->files, abs_path, fs.st_size);
            log_summary->n_files++;
            fclose(fp);
        }
        memset(abs_path, 0, LOG_BUF);
    }

    closedir(dir);

    log_summary->n_threads = MIN(log_summary->n_threads, log_summary->n_files);

    log_summary->n_max_files_per_th =
        (size_t)(log_summary->n_files / n_threads) +
        (size_t)((log_summary->n_files % n_threads) ? 1 : 0);

    log_summary->optimal_files_ids =
        log_summary_get_optimal_file_idx(log_summary->n_threads, log_summary->n_files);

    ///

    log_summary->sorted_files = bucket_list_create_ref(log_summary->files);

    if (log_summary->sorted_files == NULL)
    {
        free(log_summary->files);
        free(log_summary);
    }

    bucket_list_sort_q(log_summary->sorted_files);

    log_summary->refs = hash_table_create();
    log_summary->urls = hash_table_create();

    log_summary->total_bytes = 0;

    if (log_summary->refs == NULL || log_summary->urls == NULL)
    {
        free(log_summary);
        return NULL;
    }

    return log_summary;
}

void log_summary_print(log_summary_t *self)
{

    hash_table_print(self->urls);
    hash_table_print(self->refs);

    bucket_list_print(self->sorted_files, 10);

    printf("Threads %ld\n", self->n_threads);
    printf("Files %ld \n", self->n_files);
    printf("Max files per thread %ld\n", self->n_max_files_per_th);

    // for (size_t i = 0; i != self->n_threads * self->n_max_files_per_th; i++)
    // {
    //     printf("%ld ", self->optimal_files_ids[i]);
    // }
    printf("\nTotal bytes %ld\n\n", self->total_bytes);

    printf("Geeting Referals...\n");

    printf("\nTop 10 Refs \n\n");
    bucket_list_t *buckets_refs = bucket_list_create_ref(self->refs);
    bucket_list_sort_q(buckets_refs);
    bucket_list_print(buckets_refs, 10);
    bucket_list_free(buckets_refs);

    printf("Geeting Urls...\n");

    printf("\nTop 10 Urls\n\n");
    bucket_list_t *buckets_urls = bucket_list_create_ref(self->urls);
    bucket_list_sort_q(buckets_urls);
    bucket_list_print(buckets_urls, 10);
    bucket_list_free(buckets_urls);
}
void log_summary_free(log_summary_t *self)
{
    if (self != NULL)
    {
        if (self->sorted_files)
            bucket_list_free(self->sorted_files);
        if (self->files)
            hash_table_destroy(self->files);
        if (self->urls)
            hash_table_destroy(self->urls);
        if (self->refs)
            hash_table_destroy(self->refs);
        if (self->optimal_files_ids)
            free(self->optimal_files_ids);

        free(self);
    }
}

void log_summary_process_file(pthread_params_t *t_args, size_t file_id)
{
    const char *filename = t_args->self->sorted_files->buckets[file_id]->key_value.value;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        log_summary_status(STATUS_err, "Error opening file");
    }

    //

    t_args->t_refs = hash_table_create();
    t_args->t_urls = hash_table_create();

    char line[LOG_LINE_BUF_MAX] = " ";
    size_t count = 0;

    while (fgets(line, LOG_LINE_BUF_MAX, fp))
    {
        log_line_t log_line = (log_line_t){0};
        log_line_parse_fill(&log_line, line);
        // log_line_print(&log_line);

        if (!strlen(log_line.ip_str))
        {
            count++;
            // printf("\nLine: %ld \n", count);
        }

        t_args->total_bytes += log_line.request_size;
        hash_table_insert_with_count(t_args->t_urls, log_line.url, log_line.request_size);

        hash_table_insert(t_args->t_refs, log_line.useragent);
        // // clean local

        memset(line, 0, LOG_MAXLINE);
    }

    fclose(fp);

    hash_table_print(t_args->t_urls);
    hash_table_print(t_args->t_refs);
}

void *t_log_summary_process_files(void *args)
{

    pthread_params_t *t_args = (pthread_params_t *)args;
    printf("Starting thread %ld\n", t_args->thread_n);

    size_t max_size = t_args->self->n_max_files_per_th * t_args->self->n_threads;
    for (size_t i = t_args->thread_n; i < max_size; i += t_args->self->n_threads)
    {
        size_t file_id = t_args->self->optimal_files_ids[i];
        if (file_id > 0)
        {

            log_summary_process_file(t_args, file_id - 1);
        }
    }

    printf("Finished thread %ld\n", t_args->thread_n);
    printf("\n");
    return NULL;
}

void log_summary_get_and_rel(const char *folder_name, size_t n_threads)
{

    log_summary_t *lsum = log_summary(folder_name, n_threads);

    pthread_params_t thread_params[LOG_THREADS_MAX];
    pthread_t threads[LOG_THREADS_MAX];

    for (size_t i = 0; i != lsum->n_threads; i++)
    {

        pthread_params_t params = {
            .self = lsum,
            .thread_n = i,
            .total_bytes = 0,
            .t_refs = NULL,
            .t_urls = NULL};
        thread_params[i] = params;
    }

    for (size_t i = 0; i != lsum->n_threads; i++)
    {
        pthread_create(&threads[i], NULL, t_log_summary_process_files, &thread_params[i]);
    }

    for (size_t i = 0; i != lsum->n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (size_t i = 0; i != lsum->n_threads; i++)
    {

        lsum->total_bytes += thread_params[i].total_bytes;

        hash_table_t *refs = thread_params[i].t_refs;
        size_t refs_filled = refs->filled;
        for (size_t j = 0; j != refs_filled; j++)
        {
            if (refs->buckets[j]->exists)
            {
                char *val = refs->buckets[j]->key_value.value;
                size_t count = refs->buckets[j]->key_value.count;
                hash_table_insert_with_count(lsum->refs, val, count);
            }
        }
        hash_table_destroy(refs);

        // urls
        hash_table_t *urls = thread_params[i].t_urls;
        size_t urls_filled = urls->filled;
        for (size_t j = 0; j != urls_filled; j++)
        {
            if (urls->buckets[j]->exists)
            {
                char *val = urls->buckets[j]->key_value.value;
                size_t count = urls->buckets[j]->key_value.count;
                hash_table_insert_with_count(lsum->urls, val, count);
            }
        }
        hash_table_destroy(urls);
    }

    log_summary_print(lsum);

    log_summary_free(lsum);
}