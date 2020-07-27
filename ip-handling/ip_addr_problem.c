#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_SIZE   50
#define MAX_DIGITS      20
#define N_IP_BYTES      4


int validate_mask(char *ip_addr) {
    int mask = 0;
    char *token = NULL;
    char *ptr_copy = ip_addr;
    while ((token = strsep(&ip_addr,  "/")) != NULL) {
        if (token == ptr_copy)
            continue;
        if (sscanf(token, "%d", &mask) != 1) {
            return -1;
        }

        char mask_str[MAX_DIGITS];
        memset(mask_str, 0, MAX_DIGITS);
        sprintf(mask_str, "%d", mask);
        if (strncmp(token, mask_str, strlen(mask_str)) != 0) {
            return -1;
        }
        break;
    }

    // bad mask
    if (mask < 0 || mask > 32) {
        return -1;
    }

    return 0;
}

int validate_ip_addr(char *ip_addr) {
    size_t ntokens = 0;
    char *token = NULL;

    while ((token = strsep(&ip_addr, ".")) != NULL) {
        int value = 0;
        if (sscanf(token, "%d", &value) != 1) {
            return -1;
        }

        // not a byte
        if (value < 0 || value > 255) {
            return -1;
        }

        // more bytes
        if (++ntokens > N_IP_BYTES) {
            return -1;
        }
    }

    // less bytes
    if (ntokens < N_IP_BYTES) {
        return -1;
    }

    return 0;
}

uint32_t ip_to_int(char *ip_str) {
    uint32_t ip_nr = 0;
    char *ptr = ip_str;
    for (int i = 0; i < N_IP_BYTES; ++i) {
        int nr = 0;
        if (sscanf(ptr, "%d", &nr) != 1) {
            return ip_nr;
        }
        ip_nr |= (nr << ((N_IP_BYTES - i - 1)  << 3));

        char *dot = strchr(ptr, '.');
        if (!dot) {
            return ip_nr;
        }
        ptr = dot + 1;
    }
    return ip_nr;
}

int get_mask(char *ip_str) {
    char *slash = strchr(ip_str, '/');
    if (!slash) {
        return 0;
    }

    int mask = 0;
    if (sscanf(slash + 1, "%d", &mask) != 1) {
        return 0;
    }
    return mask;
}

void to_string(uint32_t ip) {
    for (int i = 0; i < N_IP_BYTES; ++i) {
        if (i > 0) {
            printf(".");
        }
        int byte = (ip >> ((N_IP_BYTES - i - 1) << 3) & 0xFF);
        printf("%d", byte);
    }
}

void get_range(char *ip_addr) {
    uint32_t ip = ip_to_int(ip_addr);
    int mask = get_mask(ip_addr);

    uint32_t low_ip = (ip & (((1 << mask) - 1) << (32 - mask))) + 1;
    uint32_t high_ip = ip | ((1 << (32 - mask)) - 1);

    printf("range(%s): ", ip_addr);
    to_string(low_ip);
    printf(" - ");
    to_string(high_ip);
    printf("\n");
}

int main() {
    char line[MAX_LINE_SIZE + 1];
    char *res = NULL;
    do {
        bool valid = true;
        if (res) {
            char *dup = strdup(line);
            valid = (validate_ip_addr(line) == 0);

            strncpy(line, dup, MAX_LINE_SIZE);
            free(dup);

            dup = strdup(line);
            if (valid) {
                valid = (validate_mask(line) == 0);
            }
            if (!valid) {
                fprintf(stdout, "Invalid address: %s\n", dup);
            }
            strncpy(line, dup, MAX_LINE_SIZE);
            free(dup);
        }

        if (res && valid) {
            get_range(line);
        }

        memset(line, 0, MAX_LINE_SIZE + 1);
        res = fgets(line, MAX_LINE_SIZE, stdin);
        line[strlen(line) - 1] = '\0';
    } while (res != NULL);

    return 0;
}
