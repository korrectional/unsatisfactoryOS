#ifndef MATH_H
#define MATH_H

// get the power of base by power
int powi(int base, int power){
    int result = base;
    for(int i = 1; i < power; i++){
        result *= base; 
    }
    return result;
}

int add(int a, int b){
    return a + b;
}

#endif