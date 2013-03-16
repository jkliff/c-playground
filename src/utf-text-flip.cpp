
#include <iostream>
#include <sstream>

using namespace std;

const unsigned int MAP[][2] = { { 0x0021, 0x00A1 },
                                { 0x0022, 0x201E },
                                { 0x0026, 0x214B },
                                { 0x0027, 0x002C },
                                { 0x0028, 0x0029 },
                                { 0x002E, 0x02D9 },
                                { 0x0033, 0x0190 },
                                { 0x0034, 0x152D },
                                { 0x0036, 0x0039 },
                                { 0x0037, 0x2C62 },
                                { 0x003B, 0x061B },
                                { 0x003C, 0x003E },
                                { 0x003F, 0x00BF },
                                { 0x0041, 0x2200 }, // a
                                { 0x0042, 0x10412 },
                                { 0x0043, 0x2183 },
                                { 0x0044, 0x25D6 }, //d
                                { 0x0045, 0x018E },
                                { 0x0046, 0x2132 },
                                { 0x0047, 0x2141 },
                                { 0x004A, 0x017F },
                                { 0x004B, 0x22CA },
                                { 0x004C, 0x2142 },
                                { 0x004D, 0x0057 },
                                { 0x004E, 0x1D0E },
                                { 0x0050, 0x0500 },
                                { 0x0051, 0x038C },
                                { 0x0052, 0x1D1A },
                                { 0x0054, 0x22A5 },
                                { 0x0055, 0x2229 },
                                { 0x0056, 0x1D27 },
                                { 0x0059, 0x2144 },
                                { 0x005B, 0x005D },
                                { 0x005F, 0x203E },
                                { 0x0061, 0x0250 },
                                { 0x0062, 0x0071 },
                                { 0x0063, 0x0254 },
                                { 0x0064, 0x0070 },
                                { 0x0065, 0x01DD },
                                { 0x0066, 0x025F },
                                { 0x0067, 0x0183 },
                                { 0x0068, 0x0265 },
                                { 0x0069, 0x0131 },
                                { 0x006A, 0x027E },
                                { 0x006B, 0x029E },
                                { 0x006C, 0x0283 },
                                { 0x006D, 0x026F },
                                { 0x006E, 0x0075 },
                                { 0x0070, 0x0064 },
                                { 0x0072, 0x0279 },
                                { 0x0074, 0x0287 },
                                { 0x0075, 0x006e },
                                { 0x0076, 0x028C },
                                { 0x0077, 0x028D },
                                { 0x0079, 0x028E },
                                { 0x007B, 0x007D },
                                { 0x203F, 0x2040 },
                                { 0x2045, 0x2046 },
                                { 0x2234, 0x2235 }

};

unsigned int lookupCode(unsigned int c) {

    //cerr << "lookup ";
    //cerr << std::hex << c << endl;

    for (auto m : MAP) {
        //cout << (char) m[0] << "-> " << (char) m[1] << endl;
        if (c == m[0]) {
            return m[1];
        }
    }

    return (int) c;
}

/**
 * This is needed on little endian architectures, or else utf characters get mangled by inverted byte order.
 */
int toNaturalOrder(unsigned long v, size_t s) {
    unsigned char * c = (unsigned char *) &v;
    //cerr << std::hex << (unsigned int) c[1] << endl;
    //cerr << std::hex << (unsigned int) c[0] << endl;
    //cerr << std::hex << v << endl;

    if (s == 1) {
        return c[0];
    } else if (s == 2) {
        return (c[1] << 0) | (c[0] << 8);
    } else if (s == 3) {
        return (c[2] << 0) | (c[1] << 8) | c[0] << 16;
    }

    return (c[1] << 0) | (c[0] << 8);
}

/**
 * Helper for debug
 */
char *octet2bin(long long num) {
    static char ret[32];
    int i = 0;
    unsigned long mask = 0x01;
    for (i = 32; i >= 0; --i) {
        //cerr << "current mask: " << mask << endl;
        ret[i] = num & mask ? '1' : '0';
        mask <<= 1;

    }
    return ret;
}

/**
 * outputs to stdout the
 */
void outputUTF8(unsigned long raw) {
    size_t s = 0;
    unsigned long v = 0;
    //unsigned char * c = (unsigned char *) &raw;
    if (raw <= 0x7f) {
        v = raw;
        s = 1;

    } else if (raw <= 0x07ff) {

        long keep = (raw & 0xff & ~0xc0) | 0x80;
        keep = ((raw << 2) & 0xff00 & ~0xc0ff) | 0xc000 | keep;

        v = keep;
        s = 2;

        //cerr << "dump raw:  " << octet2bin(raw) << "-> " << octet2bin(v) << endl;

    } else if (raw <= 0xffff) {

        //cerr << "dump raw:  " << octet2bin(raw) << endl;
        //cerr << "dump help: " << octet2bin(0x80) << endl;

        long keep = (raw & 0xff & ~0xc0) | 0x80;

        keep = ((raw << 2) & 0xff00 & ~0xc0ff) | 0x8000 | keep;
        v = ((raw << 4) & 0x0f0000) | 0xe00000 | keep;
        s = 3;

        //cerr << "dump done: " << octet2bin(v) << endl;

    } else if (raw <= 0x10ffff) {
        cerr << "unandled range of codepoints" << endl;
    } else {
        cerr << "unvalid utf representation ?" << endl;
        return;
    }

    v = toNaturalOrder(v, s);

    cout.write((char *) &v, s);

}

int main(void) {

    ostringstream oss;
    oss << cin.rdbuf();

    string s = oss.str();

    //string s =
    //"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque elit orci, elementum ac semper non, semper ut arcu. Nulla sit amet facilisis quam. Nunc vitae dolor a lacus vehicula dapibus ac ut nunc. Mauris cursus viverra est quis aliquam. Aenean nec ante at metus laoreet ultrices id porttitor orci. Integer et ligula eu dui tempus consectetur egestas quis nisl. Integer sed lacus rhoncus sem luctus condimentum. Duis mollis scelerisque risus ut fringilla. Sed ut enim mattis odio pulvinar porttitor. Nam at eros nec mi feugiat volutpat ut sed dui.\nPraesent sapien velit, malesuada at tincidunt in, mollis in eros. Sed semper nisl in neque egestas varius. Morbi eleifend bibendum metus, sed molestie neque cursus at. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nam eget justo enim, tincidunt dignissim tellus. In non nulla metus, at porttitor sem. Suspendisse potenti. Donec eleifend suscipit tellus malesuada egestas. Duis a quam sem, at lobortis augue. Pellentesque nec nisi nec nisl lacinia eleifend viverra in velit. Fusce suscipit, ipsum ut tempor egestas, dui elit imperdiet erat, ornare fringilla magna velit sed urna. Maecenas eget justo est. Curabitur nec risus at ligula facilisis semper.";
    //s = "oh, wait...";
    size_t ll = s.size();
    string r = string(s.rbegin(), s.rend());

    for (int i = 0; i < ll; i++) {
        unsigned int l = lookupCode(r[i]);

        outputUTF8(l);

    }

}
