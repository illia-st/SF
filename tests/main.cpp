#include <iostream>
#include <fstream>
#include <SF.h>


int main(){
    Operator::SF sf(u8"/home/illiast");
    auto str = sf.FindPath(u8"temp.txt");
    return 0;
}