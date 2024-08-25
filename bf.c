#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#define SIZE 30000

void jump_ip(char** ipp, const int dir, const char* from, const char* to, const char open, const char close) {
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

int mism_par(char* first_ip, const char open, const char close) {
 int64_t mism = 0;
 for(char* ip = first_ip; *ip; ip++) {
  if(*ip == open) mism++;
  if(*ip == close && mism == 0) return 1;
  if(*ip == close) mism--;
 }
 return mism != 0;
}

void interpret(char* first_ip, const size_t data_size) {
 if(mism_par(first_ip, '[', ']')) {
  printf("Syntax error: unmatched brackets\n");
  return;
 }
 char* first_dp = (char*) malloc(data_size * sizeof(char));
 if(!first_dp) {
  printf("Runtime error: memory not allocated\n");
  return;
 }
 memset(first_dp, 0, data_size);
 char* dp = first_dp;
 char* last_dp = first_dp + data_size - 1;
 char* ip = first_ip;
 char* last_ip = first_ip + strlen(first_ip) - 1;
 while(*ip) {
  if(*ip == '>') {
   if(dp < last_dp) dp++;
   else dp = first_dp;
  }
  if(*ip == '<') {
   if(dp > first_dp) dp--;
   else dp = last_dp;
  }
  if(*ip == '+') (*dp)++;
  if(*ip == '-') (*dp)--;
  if(*ip == '.') printf("%c", *dp);
  if(*ip == ',') {
   fflush(stdin);
   *dp = getchar();
  }
  if(*ip == '[' && !*dp) jump_ip(&ip, 1, first_ip, last_ip, '[', ']');
  if(*ip == ']' && *dp) jump_ip(&ip, -1, first_ip, last_ip, ']', '[');
  ip++;
 }
 free(first_dp);
}

int main(int argc, char* argv[]) {
 if(argc < 2) {
  printf("Invalid instructions\n");
  return 0;
 }
 size_t data_size = argc > 2 ? strtol(argv[2], NULL, 10) : SIZE;
 if(!data_size) data_size = SIZE;
 interpret(argv[1], data_size);
 return 0;
}
