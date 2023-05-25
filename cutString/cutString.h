#ifndef CUTSTRING_H
#define CUTSTRING_H

#include <stdio.h>
#include <string.h>
 
//截断有特殊符号的字符串，并取后段
char * cutString_special_byte_behind(char* string, char *byte) ;
 
//截断有特殊符号的字符串，并取前段
void cutString_special_byte_before(char* string, char *byte) ;
 
//截取字符串的前n个字符
char *  cutString_before_num(char* string, int num) ;
 
//截取字符串的后n个字符
char *   cutString_behind_num(char* string, int num) ;
 
 
#endif