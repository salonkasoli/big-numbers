#include "big_number.h"
#define BASE_BORDER 10



//base base_mul(base a, base b, base& owerflow);

void big_number::add_base(base b, int shift)
{
    base overflow = 0;

    m_data[shift] += b;

    if(m_data[shift] < b)
    {
        overflow = 1;

        for(int i = shift+1; i < m_len; i++ )
        {
            m_data[i] += overflow;

            if (m_data[i] != 0)
            {
                overflow = 0;
                break;
            }
        }
    }

    if (overflow == 1)
    {
        if (m_capacity == m_len)
        {

            m_capacity = ceil(m_len*1.5);

            base * new_data = new base [m_capacity];

            memcpy(new_data,m_data,sizeof(base)*m_len);

            delete [] m_data;

            m_data = new_data;

        }

        m_data[m_len] = 1;

        m_len += 1;

    }
}


void big_number::division(const big_number & right1, big_number & q, big_number & r) const
{



   // assert(right1.m_len > 1 && "WRONG RIGHT NUMBER WHILE DIVIDING");
    //JUST THRoUGHT KHNUT

    doublebase b = 1;
    b <<= sizeof(base)*8;


    big_number left(m_len+1);
    left = *this;
    big_number right(right1.m_len);
    right = right1;

    if(right1.m_len == 1)
    {
        base remainder = 0;

        left.base_div(right.m_data[0],remainder);

        q = left;

        r.m_data[0] = remainder;

        r.m_len = 1;

        return;
    }
    //D1


    base d=1;

    if(right.m_data[right.m_len - 1] < 0x80000000) //2147483648
    {
        d = (base) (b / (right.m_data[right.m_len - 1] + 1 ) );

        big_number d_tmp(1);

        d_tmp.m_data[0] = d;

        left = left * d_tmp;

        right = right * d_tmp;
    }

    left.m_len = m_len + 1;


    //D2 - j = (..)

    int n = right.m_len;
    int m = left.m_len - right.m_len-1;

    for(int j = m; j >= 0 ;j--)
    {
        //D3

        doublebase q_cap = ( (doublebase)left.m_data[j+n] * b + left.m_data[j+n-1] ) / right.m_data[n-1];

      //  std::cout << std::hex << q_cap << std::endl;

        doublebase r_cap = ( (doublebase)left.m_data[j+n] * b + left.m_data[j+n-1] ) % right.m_data[n-1];

       // std::cout << std::hex << r_cap << std::endl;

        if(q_cap == b || (q_cap*right.m_data[n-2]) > (b*r_cap + left.m_data[j+n-2]) )
        {
            q_cap -= 1;

            r_cap += right.m_data[n-1];

            if(r_cap < b)
            {
                if(q_cap == b || (q_cap*right.m_data[n-2]) > (b*r_cap + left.m_data[j+n-2]) )
                {
                    q_cap -= 1;

                    r_cap += right.m_data[n-1];
                }
            }
        }

        //D4

        big_number tmp(1);

        tmp.m_data[0] = (base)q_cap;

        bool borrow = left.shift_sub(right*tmp,j); // left has been changed.



        //D5

        q.m_data[j] = (base)q_cap;

        if (borrow)
        {
            printf("BORROW\n");

            q.m_data[j] -= 1;

            left.shift_add(right,j);
        }

    }

    //D8




    for(int i = 0; i < right.m_len; i++)
        r.m_data[i] = left.m_data[i];

    base remainder=0;
    r.base_div(d,remainder);




}

void big_number::Ox_input(const char *text)
{
        int text_lenght = strlen(text);

        m_len = ((text_lenght - 2) + sizeof(base)*2 -1 ) / ( sizeof(base)*2 );

        m_capacity = m_len;

        m_data = new base [ m_len ]; // text = 0xFFFFF ; 8F -> 1 int; => len = (a-2)/8!!!

        for( int i = 0; i < m_len ; i++)
            m_data[i]=0;

        int current_base = 0, current_symbol = 0;

        for(int i = text_lenght - 1; i > 1 ; i--)
        {



            //current_base flag = num of chars. each char it inc. base = 8 chars

            if ( current_symbol == sizeof(base)*2 )
                {
                    current_symbol = 0;

                    current_base++;
                }


            // fill m_data with blocks of 8 chars (tmp)

            base tmp = 0;

            if( (text[i]>='0') && (text[i]<='9') )
                tmp = text[i]-'0';

            else
                tmp = text[i]-'a' + 10;

            tmp <<= ( 4*current_symbol );

            m_data[current_base] = m_data[current_base] | tmp;

            current_symbol++;
        }
    delete_leading_zeroes();

}

void big_number::delete_leading_zeroes()
{
    assert(m_len != 0 && "WRONG BIG NUMBER");
    for (int i = m_len - 1 ; i > 0 && (m_data[i] == 0 ); i--)
            m_len--;
}


big_number big_number::base_mul(base a, base b)
{
    halfbase* p_a = reinterpret_cast<halfbase*>(&a);

    halfbase* p_b = reinterpret_cast<halfbase*>(&b);

    base result, tmp;

    base owerflow = 0;

    halfbase* p_tmp = reinterpret_cast<halfbase*>(&tmp);

    result = p_a[0] * p_b[0];

    //STEP 1 FINISHED

    //STEP 2

    tmp = (base)p_a[1] * p_b[0] + (base)p_a[0] * p_b[1];

    if ( tmp < ((base)p_a[1] * p_b[0]) )
        owerflow += ( 1 << ( sizeof(base)*4 ) );

    result += ((base)(p_tmp[0])) << ( sizeof(base)*4 );

    if ( result < ( ((base)p_tmp[0]) << ( sizeof(base)*4 ) ) )
        owerflow += 1;

    owerflow += p_tmp[1];

    //STEP 2 FINISHED

    //STEP 3

    owerflow += p_a[1] * p_b[1];

    big_number c(2);

    c.m_data[0] = result;

    c.m_data[1] = owerflow;

    c.delete_leading_zeroes();

    return c;
}


void big_number::base_div( base divider, base & remainder)
{
    doublebase dividing = 0;

    for(int i = m_len-1; i >=0; i--)
    {
        dividing <<= (sizeof(base) * 8 );

        dividing += m_data[i];

        base q = dividing / divider;

        m_data[i] = q;

        dividing = dividing % divider;
    }

    remainder = base(dividing);

    delete_leading_zeroes();
}


void big_number::shift_add ( const big_number &a, int shift)
{
    int overflow = 0;


    for(int i = 0 ; i < a.m_len; i++)
    {
        m_data[i+shift] += a.m_data[i] + overflow;
        if(m_data[i+shift] < a.m_data[i])
            overflow = 1;
        else
            overflow = 0;
    }


    for(int i = a.m_len + shift; i < m_len && overflow; i++)
    {

        m_data[i] += overflow;
        if( m_data[i] < overflow)
            overflow = 1;
        else
            overflow = 0;
    }

 //   assert(overflow == 0 && "Invalid use of shift add method!");
}


bool big_number::shift_sub(  const big_number & right, int shift )
{
    int borrow = 0;

    base tmp=0;

    for (int i = 0; i < right.m_len; i++)
    {
        assert(i+shift < m_len);
        tmp = m_data[i+shift] - right.m_data[i] - borrow;
        if (tmp > m_data[i+shift])
            borrow = 1;
        else
            borrow = 0;
        m_data[i+shift] = tmp;
    }

    for(int i = right.m_len + shift; i < m_len && borrow == 1; i++)
    {
        tmp = m_data[i] - borrow;
        if (tmp > m_data[i])
            borrow = 1;
        else
            borrow = 0;
        m_data[i] = tmp;
    }

    delete_leading_zeroes();

    if (borrow == 1)
        return true;
    else
        return false;

}


void big_number::print() const
    {
        assert(m_len != 0 && "WRONG BIG NUMBER");
        printf("0x");

        printf("%x", m_data[m_len-1]);

        for( int i = m_len - 2; i >= 0 ; i-- )
            printf("%08x", m_data[i]);
        printf("\n");

    }


 big_number::big_number(const char* text)
{
    if(strlen(text) == 0)
        throw "INVALID USER";

    if(strlen(text) > 1)
        if(text[0] == '0' && text[1] == 'x')
        {
           Ox_input(text);
           return;
        }

    dec_input(text);

}

big_number::big_number()
{
    m_data = NULL;

    m_len = 0;

    m_capacity = 0;
}

big_number::big_number(int lenght, Filltype filltype)
    {

        m_data = new base [lenght];
/*
        for ( int i = 0; i < lenght; i++)
        {
            m_data[i] = 0;
        }
    */
        m_capacity = lenght;

        m_len = lenght;

        switch(filltype)
        {
            case ZERO: memset(m_data, 0, lenght*sizeof(base)); break;

            case RANDOM: for(int i=0; i< m_len; i++) m_data[i] = rand(); break;

            default: assert(!"Invalid Filltype");
        }
    }

big_number::big_number( const big_number & a) : m_len(a.m_len), m_capacity(a.m_capacity)
{
    m_data = new base [a.m_capacity];

    memcpy(m_data, a.m_data, m_len*(sizeof(base)));
}


big_number& big_number:: operator=( big_number const & a )
    {

        if (m_capacity < a.m_len)
        {
            delete [] m_data;

            m_data = new base [a.m_len];

            m_capacity = a.m_len;

        }

        m_len = a.m_len;


        //IS IT NESSECCARY???
       //memset(m_data,0,sizeof(base)*m_len);

        memcpy(m_data, a.m_data, m_len*(sizeof(base)));

        return *this;

    }

big_number big_number:: operator+(  const big_number & right ) const
    {

        int maxlen = std::max(m_len,right.m_len);
        int minlen = std::min(m_len,right.m_len);

        big_number result(maxlen+1);

        int overflow = 0;


        for (int i = 0; i < minlen; i++)
        {
            result.m_data[i] = m_data[i] + right.m_data[i] + overflow;
            if (result.m_data[i] < m_data[i])
                overflow = 1;
            else overflow = 0;
        }

        if (m_len > right.m_len)
        {
            for (int i = minlen; i < maxlen; i++)
            {
                result.m_data[i] = m_data[i] + overflow;
                if (result.m_data[i] < m_data[i])
                    overflow = 1;
                else overflow = 0;
            }
        }

        else
        {
            for (int i = minlen; i < maxlen; i++)
            {
                result.m_data[i] = right.m_data[i] + overflow;
                if (result.m_data[i] < right.m_data[i])
                    overflow = 1;
                else overflow = 0;
            }
        }

        if (overflow == 1)
            result.m_data[result.m_len-1] = 1;

        else
            result.m_len -= 1;
        return result;

    }



big_number big_number::operator-(  const big_number & right ) const
{
    if (m_len < right.m_len)
        throw std::logic_error("Sub");

    int borrow = 0;

    big_number result(m_len);

    for (int i = 0; i < right.m_len; i++)
    {
        result.m_data[i] = m_data[i] - right.m_data[i] - borrow;

        if (result.m_data[i] > m_data[i])
            borrow = 1;
        else borrow = 0;
    }

    for(int i = right.m_len; i < m_len; i++)
    {
        result.m_data[i] = m_data[i] - borrow;
        if (result.m_data[i] > m_data[i])
            borrow = 1;
        else borrow = 0;
    }

    result.delete_leading_zeroes();

    return result;

}


big_number big_number::operator*( const big_number & right ) const
{
    big_number result(m_len + right.m_len);

    for (int i = 0; i < m_len ; i++)
        for(int j = 0; j < right.m_len ; j++)
            {
                big_number tmp = big_number::base_mul(m_data[i], right.m_data[j]);
                result.shift_add(tmp, i+j);
            }

    result.delete_leading_zeroes();
    return result;
}




big_number big_number::operator/ ( const big_number & right ) const
{

    if(m_len < right.m_len)
    {
        big_number q (1);
        q.m_data[0] = 0;
        return q;
    }
    big_number q(m_len-right.m_len+2);
    big_number r(right.m_len);
    division(right,q,r);
    q.delete_leading_zeroes();
    return q;
}

big_number big_number::operator% ( const big_number & right ) const
{
    if(m_len < right.m_len)
        return *this;
    big_number q(m_len-right.m_len+2);
    big_number r(right.m_len);
    division(right,q,r);
    q.delete_leading_zeroes();
    return r;
}

base& big_number::operator[] ( int i )
{
    if(i >= m_len || i < 0)
    {
        printf("\ni = %d len = %d", i, m_len );
        assert(!"WTF");
    }

    return m_data[i];
}

bool big_number::operator>(const big_number& right) const
{
    if (m_len > right.m_len)
        return true;
    if (m_len < right.m_len)
        return false;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return true;
        if (m_data[i] < right.m_data[i])
            return false;
    }


    //Left == Right
    return false;

}

bool  big_number::operator>=( const big_number& right) const
{
    if (m_len > right.m_len)
        return true;
    if (m_len < right.m_len)
        return false;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return true;
        if (m_data[i] < right.m_data[i])
            return false;
    }

    //Left == Right
    return true;
}

bool  big_number::operator<( const big_number& right) const
{
    if (m_len > right.m_len)
        return false;
    if (m_len < right.m_len)
        return true;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return false;
        if (m_data[i] < right.m_data[i])
            return true;
    }


    //Left == Right
    return false;
}

bool  big_number::operator<=(const big_number& right) const
{
    if (m_len > right.m_len)
        return false;
    if (m_len < right.m_len)
        return true;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return false;
        if (m_data[i] < right.m_data[i])
            return true;
    }

    //Left == Right
    return true;
}

bool  big_number::operator==( const big_number& right) const
{
    if (m_len > right.m_len)
        return false;
    if (m_len < right.m_len)
        return false;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return false;
        if (m_data[i] < right.m_data[i])
            return false;
    }

    //Left == Right
    return true;
}



bool  big_number::operator!=( const big_number& right) const
{
    if (m_len > right.m_len)
        return true;
    if (m_len < right.m_len)
        return true;
    for (int i = m_len - 1; i >= 0; i--)
    {
        if(m_data[i] > right.m_data[i])
            return true;
        if (m_data[i] < right.m_data[i])
            return true;
    }

    //Left == Right
    return false;
}

void big_number::dec_input(const char* text)
{

    int text_len = strlen(text);

    m_len = text_len * log2(10)/(sizeof(base)*8) + 1;

    m_data = new base [m_len];

    m_capacity = m_len;

    memset(m_data,0,sizeof(base)*m_len);

    big_number ten (1);

    big_number milliard (1);

    milliard.m_data[0] = 1000*1000*1000;

    ten.m_data[0] = 10;

    base current_base = 0;

    for(int i = 0; i < text_len / 9 ; i++)
    {
        *this = *this * milliard;

        current_base = 0;

        for(int j = 0; j < 9; j++)
        {
            current_base *= 10;

            current_base += text[i*9+j] - '0';
        }

        add_base(current_base);
    }

    int i = text_len%9;

    big_number ne_milliard (1);

    ne_milliard.m_data[0] = ceil(pow(10,i));


    *this = *this * ne_milliard;

    current_base = 0;

    for(int j = 0; j < i; j++)
    {
        current_base *= 10;

        current_base += text[strlen(text)/9*9+j] - '0';
    }

    add_base(current_base);
}

void big_number::dec_print() const
{
    std::string s = "";

    big_number tmp = *this;

    base print_char;

    big_number ten (1);
    ten.m_data[0] = 10;

    big_number zero (1);
    zero.m_data[0] = 0;

    while(zero != tmp)
    {
        print_char = (tmp % ten).m_data[0];

        s = std::string(1,print_char + '0') + s;

        tmp = tmp/ten;
    }

    std::cout << s << std::endl;
}


void big_number::print_dbg() const
{

    for(int i = m_len - 1; i >= 0 ; i --)

    {
        printf("%08x ", m_data[i]);
    }

    printf("\n len = %d\n capacity = %d\n", m_len, m_capacity);
}

big_number big_number::random_from_bit_quantity(int k){
    if(k % (sizeof(base) * 8) == 0 ){
        int number_of_bases = k / (sizeof(base) * 8);
        big_number a(number_of_bases,big_number::RANDOM);
        a.make_uneven();
        a.set_bit(number_of_bases - 1, sizeof(base)*8 - 1,1);

        return a;
    }
    else{
        int number_of_bases = k / (sizeof(base) * 8) + 1;
        int overflow_bits = k % (sizeof(base) * 8);
        int base_remainder = sizeof(base) * 8 - overflow_bits;

        big_number a(number_of_bases,big_number::RANDOM);
        a.base_shift(number_of_bases - 1,base_remainder);
        a.make_uneven();
        a.set_bit(number_of_bases - 1, overflow_bits - 1,1);

        return a;
    }
}

void big_number::base_shift(int base_number, int k){

    if(m_len < base_number){
        printf("Base shift error");
        return;
    }
    else{
        m_data[base_number] >>= k;
        return;
    }
}

void big_number::make_uneven(){
        m_data[0] = m_data[0] | 1;
        return;
}

void big_number::set_bit(int base_num, int bit_num, int bit_value){
    if(bit_value == 1){
        m_data[base_num] = m_data[base_num] | (1 << bit_num) ;
    }
}

int big_number::count_tailing_zeroes(){
    int zeroes = 0;
    int i = 0;
    while(m_data[i] == 0){
        zeroes += sizeof(base)*8;
        i++;
    }

    int mask = 1;
    while(true){
        if((m_data[i] & mask) == 0){
            zeroes++;
            mask = mask << 1;
        }
        else{
            return zeroes;
        }
    }

}

big_number big_number::shift_big_number(const int shift){
    int number_of_null_bases = shift / ( sizeof(base)*8 );

    int base_shift = shift % ( sizeof(base)*8 );

    big_number a(m_len - number_of_null_bases);

    unsigned int mask = 1 << base_shift;

    mask--;

    for(int i = number_of_null_bases; i < m_len; i++){

        base tmp;

        if(i == m_len - 1){
            tmp = 0;
        }
        else{
            tmp = m_data[i+1] & mask;
        }

        a.m_data[i - number_of_null_bases] = m_data[i] >> base_shift;

        a.m_data[i - number_of_null_bases] = a.m_data[i - number_of_null_bases] | (tmp << (sizeof(base)*8 - shift) );
    }

    return a;

}


big_number big_number::power(const big_number x, const big_number y, const big_number m) {


    big_number q = x;
    big_number z;
    unsigned int mask = 1;
    int j = 0;
    if(y.m_data[j] & mask){
        z = x;
    }
    else{
        z = big_number("1");
    }

    for(int i = 1; i < (sizeof(base)*8)*y.m_len; i++){
        q = (q * q) % m;
        mask <<= 1;
        if(mask == 0){
            mask = 1;
            j++;
        }
        if(mask & y.m_data[j]){
            z = z * q % m;
        }

    }

    return z;

}

int big_number::count_leading_zeroes(unsigned int x){
    int n = 0;
    if(x >> 16 == 0){
        n += 16;
        x <<= 16;
    }
    if(x >> 24 == 0){
        n += 8;
        x <<= 8;
    }
    if(x >> 28 == 0){
        n += 4;
        x <<= 4;
    }
    if(x >> 30 == 0){
        n += 2;
        x <<= 2;
    }
    if(x >> 31 == 0){
        n += 1;
    }
    return n;
}



big_number karatsuba_multiply(const big_number a, const big_number b){




}

base* karatsuba_multiply_rec(base *a,unsigned int a_len, base *b,unsigned int b_len, base *res_array, unsigned int res_array_len){

    if(a_len == 0 || b_len == 0){
        return 0;
    }

    int max_len = std::max(a_len,b_len);

    base *a_0, *a_1, *b_0, *b_1;
    unsigned int a_0_len, a_1_len, b_0_len, b_1_len;

    unsigned int half_len = (max_len+1) / 2;

    if (a_len > half_len){
        a_0 = a;
        a_0_len = half_len;
        a_1 = a;
        a_1_len = a_len - half_len;
    }
    else{
        a_0 = a;
        a_0_len = a_len;
        a_1 = 0;
        a_1_len = 0;
    }

    if (a_len > half_len){
        b_0 = b;
        b_0_len = half_len;
        b_1 = b;
        b_1_len = b_len - half_len;
    }
    else{
        b_0 = b;
        b_0_len = b_len;
        b_1 = 0;
        b_1_len = 0;
    }

    if(a_len > BASE_BORDER || b_len > BASE_BORDER){


        karatsuba_multiply_rec(a_0,a_0_len,b_0,b_0_len,res_array,res_array_len/2);
        karatsuba_multiply_rec(a_1,a_1_len,b_1,b_1_len, res_array + res_array_len/2,res_array_len/2);


        unsigned int c_0_len,c_1_len;
        base *c_1 = big_number::array_add(a_1,a_1_len,a_0,a_0_len,c_1_len);
        base *c_0 = big_number::array_add(b_1,b_1_len,b_0,b_0_len,c_0_len);


        base *c_res = new base[(half_len+1)*2];
        unsigned int c_res_len = (half_len+1)*2;
        karatsuba_multiply_rec(c_1,c_1_len,c_0,c_0_len, c_res, c_res_len);
        add_to_array(res_array+(half_len+1)/2, res_array_len - (half_len+1)/2, c_res, c_res_len);

    }
    else{




    }





}


void add_to_array(base *a, unsigned int a_len, base*b, unsigned int b_len){

    base overflow = 0;

    for(int i = 0; i < b_len; i++){
        a[i] += b[i] + overflow;
        if(a[i] < b[i]){
            overflow = 1;
        }
        else{
            overflow = 0;
        }
    }
    if(overflow == 1){
        for(int i = b_len; i < a_len; i++){
            a[i] += overflow;
            if(a[i] != 0){
                overflow = 0;
                break;
            }

            if(overflow == 1){
                printf("ERROR IN ADD TO ARRAY");
            }
        }
    }

}



base* big_number::array_add(base *a, unsigned int a_len, base *b, unsigned int b_len, unsigned int &res_len){

    unsigned int max_len, min_len;

    base *max_array;
    if(a_len >= b_len){
        max_len = a_len;
        min_len = b_len;
        max_array = a;
    }
    else{
        max_len = a_len;
        min_len = b_len;
        max_array = b;
    }

    base* res_array = new base[max_len+1];
    base overflow = 0;

    for(int i = 0; i < min_len; i++){
        res_array[i] = a[i] + b[i] + overflow;
        if(res_array[i] < a[i]){
            overflow = 1;
        }
        else{
            overflow = 0;
        }
    }

    if(overflow == 1){
        for(int i = min_len; i < max_len; i++){
            res_array[i] = max_array[i] + overflow;
            if(res_array[i] < max_array[i]){
                overflow = 1;
            }
            else{
                overflow = 0;
                break;
            }
        }

    }

    if(overflow == 1){
        res_len = max_len+1;

    }
    else{
        res_len = max_len;
    }

    return res_array;


}

