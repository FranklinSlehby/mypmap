#ifndef FORMAT_H
#define FORMAT_H

#include <sys/types.h>
#include "procfs.h"
#include "options.h"

void print_output(pid_t pid, const MapArray *maps,
                  const ProcInfo *info, const Options *opts);

#endif