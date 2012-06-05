/* 
 * iptranslate.c
 *
 *  Created on: Oct 8, 2011
 *      Author: john
 */

#include <stdio.h>
#include <netdb.h>

void translate_ip(char *);
char *octet2bin(int);
char octet2class(unsigned int);
void dump_addr_octets(const char *, int[]);
void process_addr(const char *, int[]);

static int CLASS_MAP[3] = { 0,  // 0 b0
    2,                          // 0 b10
    6                           // 0 b110
};

static char BASE_CLASS = 'A';
/**
 * Receives an nibble (presumably the first) and determines its network class.
 * In the case that it's not identified as either A, B or C, '?' is returned.
 */
char octet2class(unsigned int nibble) {
    int i;
    short mask = 0;
    for (i = 0; i < 3; i++)
    {
        unsigned short shift = 7 - i;
        mask = (mask << 1) + 1;

#ifdef __debug
        printf("%s & %s %d %s: %s\n", octet2bin(nibble >> shift),
               octet2bin(mask), mask, octet2bin(CLASS_MAP[i]),
               octet2bin(((nibble >> shift) & mask)));
#endif
        if (((nibble >> shift) & mask) == CLASS_MAP[i])
        {
            return BASE_CLASS + i;
        }
    }

    return '?';
}

/**
 * Receives an IPv4 address as string (as if given in command line form) and dumps its bitwise representation
 */
void translate_ip(char *s_addr) {
    int i;
    int nibble[4] = { -1, -1, -1, -1 };
    if (!sscanf(s_addr, "%d.%d.%d.%d", &nibble[0], &nibble[1], &nibble[2],
                &nibble[3]))
    {
        printf
            ("Parse error on %s. Does not look to be a valid IP address.\nAttempting hostname lookup...\n",
             s_addr);

        struct hostent *host = gethostbyname(s_addr);

        if (host == NULL)
        {
            printf("Lookup for %s failed.\n", s_addr);
            return;
        }
        // printf("length: %d\n", host->h_length);
        for (i = 0; host->h_addr_list[i] != NULL; ++i)
        {

            // nibble = *host->h_addr_list[i];
            // printf("\n\t%p %d %d %d %d", host->h_addr_list[i],
            // (short) host->h_addr_list[i][0], (short)
            // host->h_addr_list[i][1],
            // (short) host->h_addr_list[i][2], (short)
            // host->h_addr_list[i][3]);
            int j;
            for (j = 0; j < 4; j++)
            {
                nibble[j] = host->h_addr_list[i][j];
            }
            char s_ip[INET_ADDRSTRLEN];
            inet_ntop(host->h_addrtype, host->h_addr_list[i], s_ip,
                      INET_ADDRSTRLEN);
            // printf("\n\t%s\n", s_ip);

            process_addr(s_ip, nibble);

        }
        // printf("\n");

        return;
    }
    process_addr(s_addr, nibble);
}

void process_addr(const char *s_ip, int nibble[]) {
    // validate
    int i;
    for (i = 0; i < 4; ++i)
    {

        if (nibble[i] == -1)
        {
            printf
                ("%s does not seem to be a complete ip address. Octet nr %d not present.\n",
                 s_ip, i + 1);
            return;
        }
        if (nibble[i] > 0xFF)
        {
            printf
                ("%s seems not to be a valid ip address: value %d out of range.\n",
                 s_ip, nibble[i]);
            return;
        }
    }

    dump_addr_octets(s_ip, nibble);
}

void dump_addr_octets(const char *s_ip, int nibble[]) {
    int i;
    // dump class of network
    printf("%c ", octet2class(nibble[0]));

    // dump binary representation
    for (i = 0; i < 4; ++i)
    {
        printf("%s ", octet2bin(nibble[i]));
    }
    printf("(%s)\n", s_ip);

}

char *octet2bin(int num) {
    static char ret[8];
    int i = 0;
    unsigned short mask = 0x01;
    for (i = 7; i >= 0; --i)
    {
        ret[i] = num & mask ? '1' : '0';
        mask <<= 1;
    }
    return ret;
}

int main(int argc, char *argv[]) {

    int i;
    for (i = 1; i < argc; ++i)
    {
        translate_ip(argv[i]);
    }

    return 0;
}
