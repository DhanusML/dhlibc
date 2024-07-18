#include "../include/dhlibc.h"
#include <stdarg.h>
#include <unistd.h>

/*
 * syntax of conversion specification
 * %[$][flags][width][.precision][length modifier]conversion
 *
 * flags
 * ------
 *  #: for x or X conversion, the result is prepended with 0x or 0X
 *     unless the result is already zero.
 *  0: value is padded on the left with zeros rather than blanks
 *     for d i x and X. If precision is given for these, this flag
 *     is ignored
 *  -:  left padded with blanks and right justified
 *  ' ': leave blank before signed positive number (or empty string)
 *  +: always put sign (+/-) for signed conversions
 *
 *  width
 *  ------
 *  optional decimal digit string with nonzero first digit
 *  specifying min field width. If converted char has
 *  fewer chars than width, it will be padded with spaces.
 *  Can be specified using just '*' instead of a decimal
 *  digit indicating that the value is present in the next argument.
 *  If field width is lower than the conversion, the conversion
 *  is printed as is.
 *
 *  precision
 *  ----------
 *  For d i x X conversions the minimum number of digits to be
 *  printed. For s conversion, the max number of characters to be
 *  printed.
 *
 *  
 * 
 * conversion specifiers
 * ----------------------
 *  c: converts int to unsigned char
 *  s: prints the string
 *  p: void * pointer argument printed as hexadecimal
 *  d: converts int to signed decimal noation
 *  i: converts into to signed decimal
 *  u: unsigned int argument is converted to unsigned decimal
 *  x: unsigned int argument is converted to unsigned hexadecimal (abcdef)
 *  X: unsigned int argument is converted to unsiged hexadecimal (ABCDEF)
 *  %: prints %, no conversion is done
 *
 *
 */ 

enum state {no, yes};

typedef struct format_descriptor
{
    int width;
    int precision;
    enum state left_indent;
    enum state hash;
    enum state zero_pad;
    enum state sign;
    enum state pos_sign_blank;
} fmt_des_t;

static void set_fmt_flag(fmt_des_t* fmt_des, char format);

static int char_stream_to_int(const char * restrict *format_ptr);

static int max(int a, int b) { return a<b ? b : a;}

void print_n_times(char c, int n);


int
dh_printf(const char* restrict format, ...)
{
    const char *flag_list = "#0- +";
    char hex_digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'
    };
    va_list ap;
    va_start(ap, format);
    char conversion;
    char *convert_buffer;
    unsigned char convert_char;
    int int_arg;
    unsigned int uint_arg;
    char hex_buffer[9] = {0,0,0,0,0,0,0,0,0}; // one for null char
    char ox_buffer[6] = {0,0,0,0,0,0}; // two more for `0x/0X' or '(nil)'
    int temp_hex_pos;
    unsigned int mask = 0b1111;
    char padding_char = ' ';
    int temp_buff_len = 0;
    char sign = 0;
    while (*format)
    {
        if (*format != '%')
            dh_putchar_fd(*(format++), STDOUT_FILENO);
        else
        {
            fmt_des_t format_des = {0, 0, no, no, no, no, no};
            // flags
            while (dh_strchr(flag_list, *(format+1)))
            {
                format++;
                set_fmt_flag(&format_des, *format);
            }
            // width
            format_des.width = char_stream_to_int(&format);
            while (*(format+1)=='*')
            {
                format_des.width = va_arg(ap, int);
                format++;
            }
            // precision
            if (*(format+1)=='.')
            {
                format++;
                format_des.precision = char_stream_to_int(&format);
            }
            if (format_des.zero_pad && !format_des.precision)
                padding_char = '0';
            conversion = *(++format);
            format++;
            switch (conversion)
            {
                case 'c':
                    int_arg = va_arg(ap, int);
                    convert_char = (unsigned char)int_arg;
                    if (!format_des.left_indent)
                        print_n_times(' ', format_des.width-1);
                    dh_putchar_fd(convert_char, STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(' ', format_des.width-1);
                    break;
 
                case 'i':
                case 'd':
                    int_arg = va_arg(ap, int);
                    sign = 0;
                    if (format_des.sign || format_des.pos_sign_blank)
                    {
                        if (int_arg<0) sign = '-';
                        if (int_arg>0) sign = '+';
                        if (format_des.pos_sign_blank && int_arg>0) sign = ' ';
                        int_arg = int_arg<0 ? -int_arg : int_arg;
                    }
                    convert_buffer = dh_iota(int_arg);
                    temp_buff_len = dh_strlen(convert_buffer);
                    if (!format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(temp_buff_len, format_des.precision) - \
                            !!sign
                            );
                    dh_putchar_fd(sign, STDOUT_FILENO);
                    print_n_times('0', format_des.precision-temp_buff_len);
                    dh_putstr_fd(convert_buffer, STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(temp_buff_len,format_des.precision) - \
                            !!sign
                            );
                    free(convert_buffer);
                    break;
 
                 case 's':
                    convert_buffer = va_arg(ap,  char *);
                    temp_buff_len = dh_strlen(convert_buffer);
                    if (format_des.precision && \
                            temp_buff_len > format_des.precision)
                        temp_buff_len = format_des.precision;
                    if (!format_des.left_indent)
                        print_n_times(padding_char,
                                format_des.width-temp_buff_len);
                    for (int i=0; i<temp_buff_len; i++)
                        dh_putchar_fd(convert_buffer[i], STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(padding_char,
                                format_des.width-temp_buff_len);
                    break;
                
                case 'x':
                    uint_arg = va_arg(ap, unsigned int);
                    temp_hex_pos = 7;
                    while (uint_arg)
                    {
                        hex_buffer[temp_hex_pos--] = \
                                hex_digits[uint_arg & mask];
                        uint_arg = uint_arg >> 4;
                    }
                    if (format_des.hash && temp_hex_pos<7)
                    {
                        ox_buffer[0] = '0';
                        ox_buffer[1] = 'x';
                    }
                    if (temp_hex_pos == 7)
                    {
                        hex_buffer[temp_hex_pos--] = '0';
                    }
                    temp_buff_len = 8 - temp_hex_pos -1 + \
                                    dh_strlen(ox_buffer);
                    if (!format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(
                                temp_buff_len,
                                format_des.precision + dh_strlen(ox_buffer)
                                )
                            );
                    dh_putstr_fd(ox_buffer, STDOUT_FILENO);
                    print_n_times('0',
                            format_des.precision + dh_strlen(ox_buffer) - temp_buff_len);
                    dh_putstr_fd(hex_buffer+temp_hex_pos+1, STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(
                                temp_buff_len,
                                format_des.precision + dh_strlen(ox_buffer)
                                )
                            );
                    break;

                case 'X':
                    uint_arg = va_arg(ap, unsigned int);
                    temp_hex_pos = 7;
                    while (uint_arg)
                    {
                        hex_buffer[temp_hex_pos--] = \
                                dh_toupper(hex_digits[uint_arg & mask]);
                        uint_arg = uint_arg >> 4;
                    }
                    if (format_des.hash && temp_hex_pos<7)
                    {
                        ox_buffer[0] = '0';
                        ox_buffer[1] = 'X';
                    }
                    if (temp_hex_pos == 7)
                    {
                        hex_buffer[temp_hex_pos--] = '0';
                    }
                    temp_buff_len = 8 - temp_hex_pos -1 + \
                                    dh_strlen(ox_buffer);
                    if (!format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(
                                temp_buff_len,
                                format_des.precision + dh_strlen(ox_buffer)
                                )
                            );
                    dh_putstr_fd(ox_buffer, STDOUT_FILENO);
                    print_n_times('0',
                            format_des.precision +\
                            dh_strlen(ox_buffer) - temp_buff_len);
                    dh_putstr_fd(hex_buffer+temp_hex_pos+1, STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(
                            padding_char,
                            format_des.width - \
                            max(
                                temp_buff_len,
                                format_des.precision + dh_strlen(ox_buffer)
                                )
                            );
                    break;

                case 'p':
                    uint_arg = va_arg(ap, unsigned int);
                    temp_hex_pos = 7;
                    while (uint_arg)
                    {
                        hex_buffer[temp_hex_pos--] = \
                                hex_digits[uint_arg & mask];
                        uint_arg = uint_arg >> 4;
                    }
                    if (temp_hex_pos<7)
                    {
                        ox_buffer[0] = '0';
                        ox_buffer[1] = 'x';
                    }
                    else
                    {
                        ox_buffer[0] = '(';
                        ox_buffer[1] = 'n';
                        ox_buffer[2] = 'i';
                        ox_buffer[3] = 'l';
                        ox_buffer[4] = ')';
                    }
                    temp_buff_len = 8 - temp_hex_pos -1 + dh_strlen(ox_buffer);
                    if (!format_des.left_indent)
                        print_n_times(' ',
                                format_des.width-temp_buff_len);
                    dh_putstr_fd(ox_buffer, STDOUT_FILENO);
                    dh_putstr_fd(hex_buffer+temp_hex_pos+1, STDOUT_FILENO);
                    if (format_des.left_indent)
                        print_n_times(' ',
                                format_des.width-temp_buff_len);
                    break;

                case '%':
                    dh_putchar_fd(conversion, STDOUT_FILENO);
                    break;
            }
       }
    }
    va_end(ap);


    return 0;
}

static void set_fmt_flag(fmt_des_t* fmt_des, char format)
{
    switch(format)
    {
        case '#':
            fmt_des -> hash = yes;
            break;
        case '0':
            fmt_des -> zero_pad = yes;
            break;
        case '-':
            fmt_des -> left_indent = yes;
            break;
        case ' ':
            fmt_des -> pos_sign_blank = yes;
            break;
        case '+':
            fmt_des -> pos_sign_blank = no;
            fmt_des -> sign = yes;
            break;
        }
}

static int char_stream_to_int(const char * restrict *format_ptr)
{
    const char *format = *format_ptr;
    int width = 0;
    int width_buff_ptr = 0;
    int width_buff_size = 4;
    char *temp_width_buff;
    char *width_buff = (char *)malloc((width_buff_size+1)*sizeof(char));
    if (!width_buff) return 0; //TODO: handle better
    while (dh_isdigit(*(format+1)))
    {
        if (width_buff_ptr < width_buff_size)
            width_buff[width_buff_ptr++] = *(++format);
        else
        {
            width_buff_size *=2;
            temp_width_buff = realloc(width_buff,
                    width_buff_size+1);
            if (!temp_width_buff) return 0; //TODO: handle better
            width_buff = temp_width_buff;
            width_buff[width_buff_ptr++] = *(++format);
        }
    }
    if (width_buff_ptr)
    {
        width_buff[width_buff_ptr] = (char)0;
        width = atoi(width_buff);
        free(width_buff);
    }
    *format_ptr = format;
    return width;
}

void print_n_times(char c, int n)
{
    for (int i=0; i<n; i++)
        dh_putchar_fd(c, STDOUT_FILENO);
}
