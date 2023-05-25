#include <stdio.h>
#include <string.h>
 
//截断有特殊符号的字符串，并取后段
char * cutString_special_byte_behind(char* string, char *byte) {
    char *p_start = string;
    char newString[256];
	char byteBuf[10] = {0};
	char *newBuf;
	
	sprintf(byteBuf,"%s",byte);
    while(*p_start) {
        if (*p_start == 'p') {
            p_start++;
            strncpy(newString, p_start, sizeof(newString));
            break;
        }
        p_start++;
    }
	memcpy(newBuf,newString,strlen(newString));
    printf("newString = %s\n", newBuf);
	return newString;
}
 
//截断有特殊符号的字符串，并取前段
void cutString_special_byte_before(char* string,char *byte) {
    char *p_start = string;
    char newString[256]={0};
    int num = 0;
	char byteBuf[10] = {0};
	
	sprintf(byteBuf,"%s",byte);
 
    while(*p_start) {
        if (*p_start == 'd') 
            break;
        p_start++;
        num ++;
    }
 
    strncpy(newString, string, num);
    printf("newString = %s\n", newString);
}
 
//截取字符串的前n个字符
char * cutString_before_num(char* string, int num) {
    char *p_start = string;
    char newString[320];
 
    strncpy(newString, p_start, num);
    printf("newString = %s\n", newString);
	return newString;
}
 
//截取字符串的后n个字符
char *   cutString_behind_num(char* string, int num) {
    char *p_start = string;
    char newString[320];
 
    int len = strlen(string);
    strncpy(newString, p_start + (len - num), sizeof(newString));
    
    printf("newString = %s\n", newString);
	return newString;
}
 
 
int string_test(void)
{
    // char string[32] = "hello.txt";
    // cutString_A(string);
    // cutString_B(string);
    // cutString_C(string, 5);
    // cutString_D(string, 5);
 
    return 0;
}