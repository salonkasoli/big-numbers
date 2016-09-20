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

    void print();

    void dec_print();

    void print_dbg();

    big_number & operator=( big_number const & a );

    big_number operator+( const big_number & right ) const;

    big_number operator-( const big_number & right ) const;

    big_number operator*( const big_number & right ) const;

    big_number operator/ ( const big_number & right) const;

    big_number operator% ( const big_number & right1 ) const;

    base& operator[] ( int i );

    bool operator> ( const big_number& right) const;

    bool operator>=( const big_number& right) const;

    bool operator<( const big_number& right) const;

    bool operator<=(const big_number& right) const;

    bool operator==( const big_number& right) const;

    bool operator!=( const big_number& right) const;

    ~big_number()
    {
        delete [] m_data;
    }

};

#endif