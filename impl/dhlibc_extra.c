#include "../include/dhlibc.h"


static size_t min(size_t a, size_t b) {return a<b?a:b;}
static int absolute(int a){return a<0?-a:a;}


char *
dh_substr(const char *s, unsigned int start, size_t len)
{
    s += start;
    size_t buff_size = min(len, dh_strlen(s))+1;
    char *sub_str_start = (char *)malloc(buff_size);
    if (!sub_str_start) return 0;
    char *sub_str = sub_str_start;
    for(size_t i=0; i<buff_size-1; i++)
    {
        *sub_str++ = *s++;
    }
    *sub_str = 0;
    return sub_str_start;
}


char *
dh_strjoin(const char *s1, const char *s2)
{
    //check for overflow later
    char *join_st = (char *)malloc(dh_strlen(s1)+dh_strlen(s2)+1);
    if (!join_st) return 0;
    char *join = join_st;
    while (*s1) {*join++ = *s1++;}
    while ((*join++ = *s2++)){}
    return join_st;
}


char *
dh_strtrim(const char *s1, const char *set)
{
    const char *start  = s1;
    const char *end = s1+dh_strlen(s1)-1;
    const char *ptr = start;
    const char *set_iter;
    size_t start_offset = 0;
    size_t end_offset = 0;
    while (ptr!=end)
    {
        set_iter = set;
        while(*set_iter)
        {
            if (*set_iter == *ptr)
            {
                break;
            }
            set_iter++;
        }
        if(!*set_iter)
        {
            break;
        }
        start_offset++;
        ptr++;
    }
    if (ptr==end) return dh_calloc(1, 1);
    ptr = end;
    while (ptr!=start)
    {
        set_iter = set;
        while (*set_iter)
        {
            if(*set_iter == *ptr)
            {
                break;
            }
           set_iter++;
        }
        if (!*set_iter)
        {
            break;
        }
        end_offset++;
        ptr--;
    }
    if (ptr==start) return dh_calloc(1, 1);
    size_t buff_size = dh_strlen(s1) - start_offset - end_offset + 1;
    char *trimmed_str = (char *)malloc(buff_size);
    s1 += start_offset;
    for (size_t i=0; i<buff_size-1; i++)
    {
        trimmed_str[i] = s1[i];
    }
    trimmed_str[buff_size-1] = (char)0;
    return trimmed_str;
}


char
**dh_split(const char *s, char c)
{
    //TODO: handle malloc fails and add helper function for allocation,
    //initializations etc.
    size_t str_arr_size = 2;
    size_t str_arr_ptr = 0;
    char **str_array = (char **)malloc(str_arr_size*sizeof(char*));
    char **temp_str_array;
    const char *start = s;
    const char *ptr = s;
    const char *end;
    while(*ptr)
    {
        if (*ptr==c)
        {
            end = ptr;
            char *piece = malloc(end-start+1);
            dh_memcpy(piece, start, end-start);
            piece[end-start] = 0;
            if (str_arr_ptr>=str_arr_size)
            {
                str_arr_size*=2;
                temp_str_array =  (char **)realloc(str_array,
                        str_arr_size*sizeof(char*));
                str_array = temp_str_array;
            }
            str_array[str_arr_ptr++] = piece;
            while(*ptr==c) ptr++;
            start = ptr;
            continue;
        }
        ptr++;
    }
    //handle last piece
    end = s+dh_strlen(s);
    if(str_arr_ptr>=str_arr_size)
    {
        str_arr_size+=2;
        temp_str_array = (char**)realloc(str_array,
                str_arr_size*sizeof(char*));
        str_array = temp_str_array;
    }
    char *piece = malloc(end-start+1);
    dh_strlcpy(piece, start, end-start+1);
    str_array[str_arr_ptr++] = piece;
    //put null at the end
    if (str_arr_ptr>=str_arr_size)
    {
        str_arr_size++;
        temp_str_array = (char **)realloc(str_array,
                str_arr_size*sizeof(char*));
        str_array = temp_str_array;
    }
    str_array[str_arr_ptr] = 0;
    return str_array;
}


char *
dh_iota(int n)
{
    int temp_n = n;
    size_t buff_size = 1;    
    while(temp_n)
    {
        temp_n/=10;
        buff_size++;
    }
    buff_size += (n<0);
    temp_n = n;
    char *int_str = (char *)malloc(buff_size);
    if (n<0) {*int_str = '-';}
    char* str_ptr = int_str+buff_size-1;
    *str_ptr-- = 0;
    int digit;
    while(temp_n)
    {
        digit = absolute(temp_n%10);
        *str_ptr-- = '0'+(char)digit;
        temp_n/=10;
    }
    return int_str;
}


char *
dh_strmapi(const char *s, char (*f)(char))
{
    size_t buff_size = dh_strlen(s)+1;
    char *transformed_str = (char *)malloc(buff_size);
    if (!transformed_str) return 0;
    transformed_str[buff_size-1] = 0;
    for (size_t i=0; i<buff_size-1; i++)
    {
        transformed_str[i] = f(s[i]);
    }
    return transformed_str;
}


void
dh_putchar_fd(char c, int fd)
{
    write(fd, &c, 1);
}


void
dh_putstr_fd(char *s, int fd)
{
    while(*s)
    {
        dh_putchar_fd(*s++, fd);
    }
}


void
dh_putendl_fd(char *s, int fd)
{
    dh_putstr_fd(s, fd);
    dh_putchar_fd('\n', fd);
}


void
dh_putnbr_fd(int n, int fd)
{
    char *str_int = dh_iota(n);
    dh_putstr_fd(str_int, fd);
    free(str_int);
}
