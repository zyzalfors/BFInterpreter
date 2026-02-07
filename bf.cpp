#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define SIZE 30000

void move_ip(char** ipp, const int& dir, const char* from, const char* to, const char& open, const char& close) {
    std::int64_t mism = 1;

    while(mism > 0) {
        *ipp += dir;

        if(*ipp < from || *ipp > to) {
            *ipp = nullptr;
            return;
        }

        if(**ipp == open) mism++;
        if(**ipp == close) mism--;
    }
}

bool mism_par(const char* first_ip, const char* last_ip, const char& open, const char& close) {
    std::int64_t mism = 0;

    for(char* ip = (char*) first_ip; ip <= last_ip; ip++) {
        if(*ip == open) mism++;
        if(*ip == close && mism == 0) return true;
        if(*ip == close) mism--;
    }

    return mism != 0;
}

void read(const char* path, std::string& ist) {
    std::ifstream file(path);
    if(!file.is_open()) return;

    char c = 0;
    while(file.get(c))
        if(c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']') ist += c;

    file.close();
}

int interpret(const char* path, const std::size_t& size) {
    std::string ist;
    read(path, ist);

    if(ist.empty()) return 1;

    const char* first_ip = ist.data();
    const char* last_ip = first_ip + ist.size() - 1;
    char* ip = (char*) first_ip;

    if(mism_par(first_ip, last_ip, '[', ']')) return 2;

    std::vector<std::uint8_t> data(size, 0);
    const std::uint8_t* first_dp = data.data();
    const std::uint8_t* last_dp = first_dp + data.size() - 1;
    std::uint8_t* dp = (std::uint8_t*) first_dp;

    std::uint16_t input = 0;
    while(ip <= last_ip) {
        if(*ip == '>') {
            if(dp < last_dp) dp++;
            else
                dp = (std::uint8_t*) first_dp;
        }
        else if(*ip == '<') {
            if(dp > first_dp) dp--;
            else
                dp = (std::uint8_t*) last_dp;
        }
        else if(*ip == '+') (*dp)++;
        else if(*ip == '-') (*dp)--;
        else if(*ip == '.')
            std::printf("%c", *dp);
        else if(*ip == ',') {
            std::cin >> std::hex >> input;
            *dp = input;
        }
        else if(*ip == '[' && !*dp)
            move_ip(&ip, 1, first_ip, last_ip, '[', ']');
        else if(*ip == ']' && *dp)
            move_ip(&ip, -1, first_ip, last_ip, ']', '[');

        ip++;
    }

    return 0;
}

void print_err(const int& err) {
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

    std::size_t size = argc > 2 ? std::strtoul(argv[2], NULL, 10) : SIZE;
    if(size == 0) size = SIZE;

    print_err(interpret(argv[1], size));
    return 0;
}