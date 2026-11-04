/* Portable Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Third Party */
#include <curl/curl.h>

/* OS Config */
#include "os.h"

#define VERSION "0.3.0"
#define NAME "DaemonCraft"
#define JAVA_REPOS "https://repo.sizablesplash.com/daemoncraft/versions/java"
#define BEDROCK_REPOS "https://repo.sizablesplash.com/daemoncraft/versions/bedrock"

extern char cmd[256];

int dmn_sys(void);
int mng_insts(void);
int srt_inst(const char *inst_name, const char *inst_path, const char *inst_edition, const char *inst_version, const char *inst_mod, const char *inst_min, const char *inst_max);
int stop_inst(const char *inst_name);
int resume_inst(const char *inst_name, const char *inst_path);
int inst_conf_file(const char *inst_name, const char *inst_path);
int crt_inst(const char *inst_name, const char *inst_path);
int dwnld_srvr(const char *inst_path, const char *vers, const char *edition);