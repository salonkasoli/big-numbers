#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string>




typedef unsigned int base;
typedef unsigned long long doublebase;
typedef unsigned short halfbase;



class big_number
{
private:

    base* m_data;

    base m_len;

    base m_capacity;

    void add_base(base b, int shift = 0);

    void division(const big_number & right1, big_number & q, big_number &r) const;

    void Ox_input(const char *text);

    void dec_input(const char* text);

    void delete_leading_zeroes();

    void shift_add ( const  big_number & a, int shift);

    bool shift_sub(  const big_number & right, int shift );

    static big_number base_mul(base a, base b);

    void base_div( base divider, base & remainder);

    void set_bit(int base_num, int bit_num, int bit_value);

    void base_shift(int base_number, int k);

    void make_uneven();

    static int count_leading_zeroes(unsigned int x);






public:

    enum Filltype
    {
        ZERO = 0,
        RANDOM
    };

    big_number(const char* text);

    big_number();

    explicit big_number(int lenght, Filltype filltype = ZERO);

    big_number( const big_number & a);

    void print() const;

    void dec_print() const;

    void print_dbg() const;

    big_number & operator=( big_number const & a );

    big_number operator+( const big_number & right ) const;

    big_number operator-( const big_number & right ) const;

    big_number operator*( const big_number & right ) const;

    big_number operator/ ( const big_number & right) const;

    big_number operator% ( const big_number & right1 ) const;

    static big_number power(const big_number x, const big_number y, const big_number m);

    base& operator[] ( int i );

    bool operator> ( const big_number& right) const;

    bool operator>=( const big_number& right) const;

    bool operator<( const big_number& right) const;

    bool operator<=(const big_number& right) const;

    bool operator==( const big_number& right) const;

    bool operator!=( const big_number& right) const;

    static big_number random_from_bit_quantity(int k);

    int count_tailing_zeroes();

    big_number shift_big_number(const int shift);

    static base* array_add(base *a, unsigned int a_len, base *b, unsigned int b_len, unsigned int &res_len);






    ~big_number()
    {
        delete [] m_data;
    }

};

#endif
