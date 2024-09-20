#if !defined(L_LOG_SUMMARY_INTERNAL_H)
#define L_LOG_SUMMARY_INTERNAL_H
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include <pthread.h>

#include "hash_table.h"


#define LOG_BUF             4096U

#define LOG_MAXLINE         LOG_BUF
#define LOG_REQUEST_SZ      LOG_BUF
#define LOG_URL_SZ          LOG_BUF
#define LOG_LINE_BUF_MAX    LOG_BUF * 4U
#define LOG_MAXWORD         LOG_BUF

#define LOG_THREADS_MAX     24U

#define LOG_PATH_SEP        "/"

#define str_n_cat(dest, src) strncat((char *)(dest), (const char *)(src), (strlen((const char *)(src)) + 1))

int fileno(FILE *fp);

#endif // L_LOG_SUMMARY_INTERNAL_H
