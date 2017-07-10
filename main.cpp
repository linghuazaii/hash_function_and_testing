#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <map>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "hash.h"
using namespace std;

#define MAXLEN 50
#define MAXSIZE  1000 * 1000

int file;

string get_string() {
    char buffer[MAXLEN + 1];
    int length = random() % 50;
    int count = read(file, buffer, length);
    buffer[count] = 0;
    return string(buffer);
}

int main(int argc, char **argv) {
    srand(time(0));
    file = open("/dev/urandom", O_RDONLY);
    map<uint32_t, string> hashs;
    int conflicts = 0;
    for (int i = 0; i < MAXSIZE; ++i) {
        string str = get_string();
        //uint32_t hash = djb2_hash(str.c_str(), str.length());
        //uint32_t hash = sdbm_hash(str.c_str(), str.length());
        //uint32_t hash = stupid_hash(str.c_str(), str.length());
        //uint32_t hash = SuperFastHash(str.c_str(), str.length());
        //uint32_t hash = fnv_hash(str.c_str(), str.length());
        //uint32_t hash = jenkins_hash(str.c_str(), str.length());
        //uint32_t hash = pjw_hash(str.c_str(), str.length());
        //uint32_t hash = elf_hash(str.c_str(), str.length());
        //uint32_t hash = murmur3_hash(str.c_str(), str.length());
        uint32_t hash = bkdr_hash(str.c_str(), str.length());
        if (hashs.find(hash) != hashs.end() && str != hashs[hash])
            ++conflicts;
        else
            hashs.insert(make_pair(hash ,str));
        //cout<<str<<"       "<<hash<<endl;
    }
    cout<<"conflicts: "<<conflicts<<endl;

    close(file);

    return 0;
}
