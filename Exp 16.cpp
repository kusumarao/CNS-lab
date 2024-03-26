#include <stdio.h>
#include <stdlib.h>

int main() {
    // Run Snort command
    printf("Running Snort Intrusion Detection System...\n");
    system("snort -A console -q -c /path/to/snort.conf");

    return 0;
}
