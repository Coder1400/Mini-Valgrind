/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2016
 */

#include "mini_valgrind.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
  // your tests here using malloc and free

    
    char* c = malloc(100);
    char* s = malloc(100);
    char* b = malloc(100);
    char* x = malloc(100);
    char* y = malloc(100);

    
    
    
    puts(c);
    puts(s);
    puts(b);
    puts(x);
    puts(y);
    
    
    
    free(c);
    free(c);
    
    
    char* cd = malloc(100);
      puts(cd);
    
    
    
  // Do NOT modify this line
  atexit(print_report);
  return 0;
}
