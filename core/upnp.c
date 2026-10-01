#include "core.h"

void setup_upnp_port_forwarding(int external_port, int internal_port) {
    char command[256];
    // Example using the standard 'upnpc' command-line utility available on Linux
    snprintf(command, sizeof(command), "upnpc -a 192.168.150.237 %d %d TCP", internal_port, external_port);
    
    int result = system(command);
    if (result == 0) {
        printf("[UPnP] Successfully requested router port forward for port %d!\n", external_port);
    } else {
        printf("[UPnP] Failed to configure router port forwarding.\n");
    }
}
