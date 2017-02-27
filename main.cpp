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
    big_number a("123");
    big_number b("456");
    big_number c = big_number::karatsuba_multiply_idiotic(a,b);
    c.print();



}
