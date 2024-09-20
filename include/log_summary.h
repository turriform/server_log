#if !defined(_L_LOG_SUMMARY_H)
#define _L_LOG_SUMMARY_H

#include "log_summary_internal.h"
#include "log_line.h"



typedef struct LogSummary
{

    size_t n_threads;
    size_t n_max_files_per_th;

    size_t n_files; 
    hash_table_t * files;
    bucket_list_t * sorted_files;

    size_t * optimal_files_ids;



    hash_table_t * refs;
    hash_table_t * urls;
    size_t total_bytes;
   

}log_summary_t;

void log_summary_get_and_rel(const char * folder_name, size_t n_threads);

size_t *log_summary_get_optimal_file_idx(size_t n_threads, size_t num_files);

log_summary_t *log_summary(const char *dirname, size_t n_threads);
void log_summary_print(log_summary_t * self);
void log_summary_free(log_summary_t * self);
// void log_summary_process_file(pthread_params_t *t_args, size_t file_id);
#endif