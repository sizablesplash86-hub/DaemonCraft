/* POSIX Headers */
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/stat.h>

#define OS "Linux"
#define ROOT "/home/daemoncraft"
#define INSTS_ROOT "/home/daemoncraft/instances"
#define RUNNING_INSTS "/home/daemoncraft/instances/active.pid"