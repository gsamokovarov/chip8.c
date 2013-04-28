#ifndef _APP_H
#define _APP_H

#include <signal.h>

typedef struct io io_t;

typedef struct app {
  volatile sig_atomic_t running;
  io_t                * io;
  chip8_t             * chip8;
} app_t;

static app_t * current_app = 0;

app_t * app_new(void);
void app_setup(app_t *);
void app_run(app_t *);
void app_parse_command_line(app_t *, int, char **);
void app_teardown(app_t *);
void app_free(app_t *);

void current_app_setup_signal_handlers(void);
void current_app_handle_interrupt(int);

#endif
