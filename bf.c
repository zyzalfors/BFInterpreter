#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define SIZE 30000

void moveip(char** ipp, const int dir, const char* from, const char* to, const char open, const char close) {
    int64_t mism = 1;

    while(mism > 0) {
        *ipp += dir;

        if(*ipp < from || *ipp > to) {
            *ipp = NULL;
            return;
        }

        if(**ipp == open) mism++;
        if(**ipp == close) mism--;
    }
}

int mismpar(const char* firstip, const char* lastip, const char open, const char close) {
    int64_t mism = 0;

    for(char* ip = (char*) firstip; ip <= lastip; ip++) {
        if(*ip == open) mism++;
        if(*ip == close && mism == 0) return 1;
        if(*ip == close) mism--;
    }

    return mism != 0;
}

void read(const char* path, std::string& ist) {
    std::ifstream file(path);
    if(!file.is_open()) return;

    char c;
    while(file.get(c)) {
        if(c != '>' && c != '<' && c != '+' && c != '-' && c != '.' && c != ',' && c != '[' && c != ']') continue;
        ist += c;
    }

    file.close();
}

int interpret(const char* path, const size_t data_size) {
    std::string ist;
    read(path, ist);
    if(ist.empty()) return 1;

    const char* firstip = ist.data();
    const char* lastip = firstip + ist.size() - 1;
    char* ip = (char*) firstip;

    if(mismpar(firstip, lastip, '[', ']')) return 2;

    std::vector<char> data(data_size, 0);
    const char* firstdp = data.data();
    const char* lastdp = firstdp + data.size() - 1;
    char* dp = (char*) firstdp;

    int err = 0;
    while(ip <= lastip) {
        if(*ip == '>') {
            if(dp < lastdp) dp++;
            else dp = (char*) firstdp;
        }
        else if(*ip == '<') {
            if(dp > firstdp) dp--;
            else dp = (char*) lastdp;
        }
        else if(*ip == '+') (*dp)++;
        else if(*ip == '-') (*dp)--;
        else if(*ip == '.') std::printf("%c", *dp);
        else if(*ip == ',') std::cin >> *dp;
        else if(*ip == '[' && !*dp) moveip(&ip, 1, firstip, lastip, '[', ']');
        else if(*ip == ']' && *dp) moveip(&ip, -1, firstip, lastip, ']', '[');

        ip++;
    }

    return err;
}

void print_err(int err) {
    switch(err) {
        case 1:
            std::cout << "Error: unable to read code\n";
            break;

        case 2:
            std::cout << "Error: unmatched brackets\n";
            break;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) return 0;

    size_t data_size = argc > 2 ? std::strtol(argv[2], NULL, 10) : SIZE;
    if(!data_size) data_size = SIZE;

    print_err(interpret(argv[1], data_size));
    return 0;
}
