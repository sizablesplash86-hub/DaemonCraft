/* POSIX Headers */
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>

#define OS "Debian"
#define ROOT "/home/daemoncraft"
#define INSTS_ROOT "/home/daemoncraft/instances"
#define RUNNING_INSTS "/home/daemoncraft/instances/active.pid"