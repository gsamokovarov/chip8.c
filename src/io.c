#include <stdlib.h>
#include "io.h"

void io_free(io_t * self) {
  free(self);
}
