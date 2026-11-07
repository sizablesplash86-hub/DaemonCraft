#include "config.h"

char cmd[256];

int main(int argc, char *argv[])
{
  if (getuid() != 0)
  {
    printf("%s requires sudo\n", NAME);
    return -1;
  }

  if (argc < 2)
  {
    printf("Please enter sub command. Run \"sudo %s -h\" to see a list of commands\n", NAME);
    return 1;
  }

  else if (strcmp(argv[1], "-h") == 0)
  {
    printf("\n"
      "  -h    Lists commands\n"
      "  -v    Lists current version\n"
      // the rest are placeholders
      "  -m    manage an instance\n"
      "\n"
    );
    return 0;
  }

  else if (strcmp(argv[1], "-v") == 0)
  {
    printf("\n"
      "  %s : v%s\n"
      "  (C) SizableSplash\n"
      "  Built for %s\n"
      "Thank you for choosing %s!\n\n",
      NAME, VERSION, OS, NAME
    );
    return 0;
  }
  else if (strcmp(argv[1], "-jv") == 0) printf("%s", VERSION);

  else if (strcmp(argv[1], "-m") == 0)
  {
    mng_insts();
    return 0;
  }

  else if (strcmp(argv[1], "-d") == 0) dmn_sys();

  else printf("\"sudo %s %s\" not valid command\n", NAME, argv[1]);

  return 0;
}