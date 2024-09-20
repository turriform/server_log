#include "log_summary_internal.h"
#include "log_line.h"

static inline void log_line_parse_str(char *dest, const char *line, size_t *i_val, const char open, const char close)
{
    size_t j = 0;
    size_t i = *i_val;

    char buf[LOG_LINE_BUF_MAX] = "";
    char c = '\0';

    while ((c = line[i++]) != open)
        ;

    c = '\0';
    while ((c = line[i]) != close)
    {
        if (c == '\n')
            return;
        buf[j++] = line[i++];
    }
    memcpy(dest, buf, strlen(buf));

    *i_val = i + 1;
}

void log_line_parse_fill(log_line_t *log_line, const char *line)
{

    
    size_t i = 0, j = 0;
    char c = ' ';
    char buf[LOG_LINE_BUF_MAX] = "";

    while (line[i++] == '\n')
        ;

    log_line_parse_str(log_line->request, line, &i, '\"', '\"');

    // get numbers

    i++;
    while ((c = line[i]) != ' ')
    {
        buf[j++] = line[i++];
    }
    j = 0;

    log_line->request_type = atol(buf);
    memset(buf, 0, strlen(buf));
    i++;
    while ((c = line[i]) != ' ')
    {
        buf[j++] = line[i++];
    }
    j = 0;

    log_line->request_size = atol(buf);
    memset(buf, 0, strlen(buf));

    // continue with strs

    log_line_parse_str(log_line->url, line, &i, '\"', '\"');
    log_line_parse_str(log_line->useragent, line, &i, '\"', '\"');
}

void log_line_print(log_line_t *log_line)
{

    printf("request:\t%s\n", log_line->request);
    printf("request_type:\t%ld\n", log_line->request_type);
    printf("request_size:\t%ld\n", log_line->request_size);
    printf("url:\t\t%s\n", log_line->url);
    printf("useragent:\t%s\n", log_line->useragent);
}