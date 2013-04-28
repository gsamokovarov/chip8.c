#ifndef _APP_H
#define _APP_H

#include <signal.h>

typedef struct io io_t;

typedef app_options {
  io_t * io;
} app_options_t;

typedef struct app {
  volatile sig_atomic_t running;
  app_options_t         options;
} app_t;

static app_t * current_app = 0;

app_t * app_new(void);
void app_parse_command_line(app_t *, int, char **);
void app_free(app_t *);

void current_app_setup_signal_handlers(void);
void current_app_handle_interrupt(int);

#endif
