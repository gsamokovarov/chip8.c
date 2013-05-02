#ifndef _APP_H
#define _APP_H

#include <signal.h>
#include <getopt.h>

typedef struct io io_t;
typedef struct chip8 chip8_t;

typedef struct app {
  volatile sig_atomic_t running;
  io_t                * io;
  chip8_t             * chip8;
} app_t;

const static struct option app_options[] = {
  {"io", required_argument, 0, 'i'},
  {"help", no_argument, 0, 'h'},
  {0, 0, 0, 0}
};

extern app_t * current_app;

app_t * app_new(void);
void app_setup(app_t *);
void app_run(app_t *);
void app_parse_command_line(app_t *, int, char **);
void app_teardown(app_t *);
void app_free(app_t *);

void current_app_set_to(app_t *);
void current_app_setup_signal_handlers(void);
void current_app_handle_interrupt(int);

#endif
