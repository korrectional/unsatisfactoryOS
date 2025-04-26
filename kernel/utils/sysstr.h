#ifndef SYSSTR_H
#define SYSSTR_H



int strlen(char str[]){
    int len = 0;
    while(str[len] != '\0' || str[len] != 0x0){
        len++;
    }
    return len;
}


int strCompare(char* str1, char* str2){
    unsigned int j = 0;
    unsigned int minLen = 0;
    while(1){
        if(str1[j] == '\0' || str2[j] == '\0'){
            minLen = j;
            break;
        }
        j++;
    }
    for(int i = 0; i < minLen; i++){
        if(str1[i] != str2[i]){
            return 0;
        }
    }
    return 1;
}





#endif