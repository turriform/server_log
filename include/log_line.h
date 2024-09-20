#if !defined(_L_LOG_LINE_H)
#define _L_LOG_LINE_H
#include "log_summary_internal.h"




typedef struct LogLine
{
    char  ip_str[256];
    char  user_id_str[256];
    char  time_str[256];
    char  request[LOG_REQUEST_SZ];
    size_t request_type;
    size_t request_size;
    char  url[LOG_REQUEST_SZ];
    char  useragent[LOG_REQUEST_SZ];

}log_line_t;

void log_line_parse_fill(log_line_t * log, const char *line_buf);
void log_line_print(log_line_t *log_line);

#endif // _L_LOG_LINE_H
