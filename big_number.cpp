#include "big_number.h"



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


void big_number::print()
    {
        assert(m_len != 0 && "WRONG BIG NUMBER");
        printf("0x");

        printf("%x", m_data[m_len-1]);

        for( int i = m_len - 2; i >= 0 ; i-- )
            printf("%08x", m_data[i]);

        printf("\n len = %d \n capacity = %d\n\n", m_len, m_capacity);

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

void big_number::dec_print()
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


void big_number::print_dbg()
{

    for(int i = m_len - 1; i >= 0 ; i --)

    {
        printf("%08x ", m_data[i]);
    }

    printf("\n len = %d\n capacity = %d\n", m_len, m_capacity);
}
