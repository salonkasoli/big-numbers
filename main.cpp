#define NDEBUG
#include <iostream>
#include "big_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <sstream>



using namespace std;

int main()
{
    srand(time(0));
    for(int i = 1; i < 100; i++){
        big_number x(i,big_number::RANDOM);
        big_number y("11111111111111111111111111111111411111111111111111111111111111111");
        //big_number y("2147483647");
        big_number pow = big_number::power(x,y,y);
        if (x%y != pow){
            x.dec_print();
            y.dec_print();
            (x%y).print();
            pow.print();
            break;
        }
    }



}
