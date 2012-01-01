#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

int main (int argc, char * argv[]) {


    char * c = const_cast <char*>(string ("/opt/local/libexec/gnubin/ls").c_str());
    char *p[2];
    p[0] = c;
    p[1] = const_cast <char*>("-l");


    int r;
//    extern "C" {
        r = execve (c, p, NULL);
//    }

    cout <<"execve() -> " << r << endl;

}
