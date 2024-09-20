#include "log_summary.h"

#include <assert.h>

void test_log_line(void)
{
    const char *line = "\n176.193.24.191 -username - [19/Jul/2020:07:43:40 +0000] \"GET /admin/jokes/joke/?p=1 HTTP/2.0\" 200 9134 \"https://baneks.site/admin/jokes/joke/\" \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36\" \"-\"\n";

    log_line_t log_line = (log_line_t){0};
    log_line_parse_fill(&log_line, line);
    assert(log_line.request_type == 200);
    log_line_print(&log_line);
}

int main(int argc, char**argv)

{

    if (argc<3){
        printf("usage: prog foldername  nthreads\neg: prog ./logs 4\n");
        exit(EXIT_SUCCESS);
    }

    size_t n_threads = atol(argv[2]);
    log_summary_get_and_rel(argv[1], n_threads);


}