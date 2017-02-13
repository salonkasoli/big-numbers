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
    big_number a("0x12345000000000");
    a.print_dbg();
    int r = a.count_tailing_zeroes();
    printf("r = %d\n",r);
    big_number b = a.shift_big_number(r);
    b.print_dbg();


    /*
    srand(time(0));
    const int number_of_tests = 1000;
    const int size_of_big_number = 1000;

    int n = 1 + rand()%size_of_big_number;
    int m = 1 + rand()%size_of_big_number;

    for(int i = 0; i < number_of_tests; i++)
    {
        big_number a(n,big_number::RANDOM);
        big_number b(m,big_number::RANDOM);
        big_number q = a / b;
        big_number r = a % b;
        if ((a == b * q + r) && (a - r == b * q) && ( r < b ) ) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }
*/

/*
    for(int i = 0; i < number_of_tests; i++)
    {
        big_number a(size_of_big_number,big_number::RANDOM);
        big_number b(size_of_big_number,big_number::RANDOM);
        if (a+b-a-b == big_number (1)) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }
    */
/*
    for(int i = 0; i < number_of_tests; i++)
    {
        big_number a(size_of_big_number,big_number::RANDOM);
        big_number b(size_of_big_number,big_number::RANDOM);
        big_number c(size_of_big_number,big_number::RANDOM);
        if(a * b * c != a * (b * c) ) {printf("ERROR"); break;}
        if(a * b * c != c * b * a ) {printf("ERROR"); break;}
        if(a*(b*c-c) != a*b*c - a*c ) {printf("ERROR"); break;}
        if(a*(b*c+c) != a*b*c + a*c ) {printf("ERROR"); break;}
        printf("VSJO HOROSHO, %d \n", i);
    }
*/

/*
    for(int i = 0; i < number_of_tests; i++)
    {
        big_number a(2*size_of_big_number,big_number::RANDOM);
        big_number b(size_of_big_number,big_number::RANDOM);
        if (a*b/b == a) printf("TEST %d is COMPLETED\n", i);
        if(a*b*b == a || true) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }
*/

/*
    for(int i = 0; i < number_of_tests; i++)
    {
        big_number a(2*size_of_big_number,big_number::RANDOM);
        big_number b(size_of_big_number,big_number::RANDOM);
        if (a/b*b + a%b == a) printf("TEST %d is COMPLETED\n", i);
        else {printf("ERROR!!!!!!\n"); break;}
    }
*/

/*
    string test_root_dir = "C:\\tests";

    string test_name = "bigint_div_tests";



    char test_num_str [4];


    for(int i = 1; i <= 50; i++)
    {
        sprintf(test_num_str,"%03d",i);

        string datname =  test_root_dir + "\\" + test_name + "\\" + test_num_str + ".dat";

        ifstream in_dat_file(datname.c_str());

        if (!in_dat_file)
            cout << "cant open " << datname << endl;

        string num_1_str, num_2_str;

        in_dat_file >> num_1_str;

        in_dat_file >> num_2_str;

        big_number a (num_1_str.c_str());

        big_number b (num_2_str.c_str());

        string ansname =  test_root_dir + "\\" + test_name + "\\" + test_num_str + ".ans";

        ifstream in_ans_file(ansname.c_str());

        if (!in_ans_file)
            cout << "cant open " << datname << endl;

        string ans_str;

        in_ans_file >> ans_str;

        big_number c (ans_str.c_str());

        if(a / b == c)
            printf("TEST %d PASSED\n", i);
        else
        {
            printf("TEST %d FAILED\n", i);

            a.dec_print();
            b.dec_print();
            (a/b).dec_print();
            c.dec_print();
            break;
        }

    }


   // big_number a("79750903799563644671");
  //  a.dec_print();
*/
}
