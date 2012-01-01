#include <unistd.h>
#include <stdio.h>

int main (int argc, char * argv[]) {


    char * c = "/opt/local/libexec/gnubin/ls";
    char *p[2];
    p[0] = c;
    p[1] = "-l";

    int r = execve (c, p, NULL);

    printf ("execve() -> %d\n", r);
}
