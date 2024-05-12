#include "../include/dhlibc.h"

static size_t min(size_t a, size_t b) {return a<b?a:b;}

void *
dh_memset(void *s, int c, size_t n)
{
    char *ptr = (char *) s;
    for (size_t i=0; i<n; i++)
    {
        ptr[i] = (char) c;
    }
    return s;
}


void
dh_bzero(void *s, size_t n)
{
    dh_memset(s, 0, n);
}


void *
dh_memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    char *ptr_src, *ptr_dest;
    ptr_src = (char *)src;
    ptr_dest = (char *)dest;
    for (int i=0; i<n; i++)
    {
       ptr_dest[i] = ptr_src[i];
    }
    return dest;
}


void *
dh_memmove(void *dest, const void *src, size_t n)
{
    char *ptr_src, *ptr_dest;
    ptr_src = (char *)src;
    ptr_dest = (char *)dest;
    char *buffer = (char *) malloc(n*sizeof(char));
    dh_memcpy(buffer, ptr_src, n);
    for (size_t i=0; i<n; i++)
    {
        ptr_dest[i] = buffer[i];
    }
    free(buffer);
    return dest;
}


void *
dh_memchr(const void *s, int c, size_t n)
{
    unsigned char *str_ptr = (unsigned char *)s;
    unsigned char token = (unsigned char)c;
    for (size_t i=0; i<n; i++)
    {
       if (str_ptr[i]==token) return str_ptr+i; 
    }
    return (void *)NULL;
}


int
dh_memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned char *s1_ptr, *s2_ptr;
    s1_ptr = (unsigned char *)s1;
    s2_ptr = (unsigned char *)s2;
    int diff = 0;

    for (size_t i=0; i<n; i++)
    {
        diff = (int) (s1_ptr[i]-s2_ptr[i]);        
        if (diff) return diff;
    }
    return diff;
}


size_t
dh_strlen(const char *s)
{
    size_t len = 0;
    while (*s++) len++;
    return len;
}


size_t
dh_strlcpy(char *restrict dest, const char *restrict src, size_t dsize)
{
    size_t len = 0;
    while(*src)
    {
        if (++len<dsize)
        {
            *dest++ = *src++;
        }
        else
        {
            src++;
        }
    }
    *dest = 0;
    return len;
}


size_t
dh_strlcat(char *restrict dest, const char *restrict src, size_t dsize)
{
    size_t len = 0;
    while(*dest)
    {
        len++;
        dest++;
    }
    while(*src)
    {
        if(++len<dsize) //actual len < dsize-1
        {
            *dest++ = *src++;
        }
        else
        {
            src++;
        }
    }
    *dest = 0;
    return len;
}


char *
dh_strchr(const char *s, int c)
{
    while(*s)
    {
        if (*s == c) return (char *)s;
        s++;
    }
    return 0;
}


char *
dh_strchrnul(const char *s, int c)
{
    while(*s && *s!=c) s++;
    return (char *)s;
}


char *
dh_strrchr(const char *s, int c)
{
    char *ret = 0;
    while (*s)
    {
        if (*s==c) ret = (char *)s;        
    }
    return ret;
}


char *
dh_strnstr(const char *big, const char *little, size_t len)
{
    int match = 1;
    int window_width = dh_strlen(little);
    int end = min(dh_strlen(big), len)+1-window_width;
    char *match_start; 
    for (int i=0; i<end; i++)
    {
        match_start = (char *)big + i;
        match = 1;
        for (int j=0; j<window_width; j++)
        {
            if (little[j]!=match_start[j])
            {
                match = 0;
                break;
            }
        }
        if (match) return match_start;
    }
    return 0;
}


int
dh_strncmp(const char *s1, const char *s2, size_t n)
{
    int diff1 = 0;
    int diff2 = 0;
    diff1 = n - min(dh_strlen(s1), dh_strlen(s2));
    diff2 = dh_strlen(s1) - dh_strlen(s2);
    if (diff1>0)
    {
        if (diff2) return diff2;
        else return dh_memcmp(s1, s2, dh_strlen(s1));
    }
    else return dh_memcmp(s1, s2, n); 
}


int
dh_atoi(const  char * nptr)
{
    int result = 0;
    char digit = 0;
    while (*nptr)
    {
        digit = (int) *nptr - 48;
        if (digit<0 || digit >9) return 0;
        result *= 10;
        result += digit;
        nptr++;
    }
    return result;
}


int
dh_isalpha(int c)
{
    if (c<65 || c>122) return 0;
    if (c>90 && c<97) return 0;
    return 1;
}


int
dh_isdigit(int c)
{
    if (c<48 || c>57) return 0;
    return 1;
}


int
dh_isalnum(int c)
{
    return dh_isalpha(c) || dh_isdigit(c);
}


int
dh_isacii(int c)
{
    if (c>-1 && c<128) return 1;
    return 0;
}


int
dh_isprint(int c)
{
    if (c<32 || c>126) return 0;
    return 1;
    return 0;
}


int
dh_toupper(int c)
{
    if(!dh_isalpha(c)) return c;
    if (c<92) return c;
    return c-(1<<5);
}


int
dh_tolower(int c)
{
    if(!dh_isalpha(c)) return c;
    if (c>96) return c;
    return c+(1<<5);
}


void *
dh_calloc(size_t nmemb, size_t size)
{
    size_t max_size_t = (size_t)-1;
    if ((max_size_t/size) < nmemb) return 0;
    void *ptr = malloc(nmemb*size);
    if (!ptr) return ptr;
    dh_memset(ptr, 0, nmemb*size);
    return ptr;
}

char *
dh_strdup(const char *s)
{
    size_t buff_size = dh_strlen(s)+1;
    char *copy = (char *)malloc(buff_size);
    if(!copy) return copy;
    dh_memcpy(copy, s, buff_size);
    return copy;
}



