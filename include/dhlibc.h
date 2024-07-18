#ifndef __LIBDH_H__
#define __LIBDH_H__

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

//memset
void *dh_memset(void *s, int c, size_t n);

// bzero
void dh_bzero(void *s, size_t n);

//memcpy
void *dh_memcpy(void *restrict dest, const void *restrict src, size_t n);

//memmove
void *dh_memmove(void *dest, const void *src, size_t n);

//memchr
void *dh_memchr(const void *s, int c, size_t n);

//memcmp
int dh_memcmp(const void *s1, const void *s2, size_t n);

//strlen
size_t dh_strlen(const char *s);

//strlcpy
size_t dh_strlcpy(char *restrict dest, const char *restrict src, size_t dsize);

//strlcat
size_t dh_strlcat(char *restrict dest, const char *restrict src, size_t dsize);

//strchr
char *dh_strchr(const char *s, int c);

//strchrnul
char *dh_strchrnul(const char *s, int c);

//strrstr
char *dh_strnstr(const char *big, const char *little, size_t len);

//strncmp
int dh_strncmp(const char *s1, const char *s2, size_t n);

//atoi
int dh_atoi(const char *nptr);

//isalpha
int dh_isalpha(int c);

//isdigit
int dh_isdigit(int c);

//isalnum
int dh_isalnum(int c);

//isascii
int dh_isascii(int c);

//isprint
int dh_isprint(int c);

//toupper
int dh_toupper(int c);

//tolower
int dh_tolower(int c);

//calloc
void *dh_calloc(size_t nmemb, size_t size);

//strdup
char *dh_strdup(const char *s);

//substr
char *dh_substr(const char *s, unsigned int start, size_t len);

//strjoin
char *dh_strjoin(const char *s1, const char *s2);

//strtrim
char *dh_strtrim(const char *s1, const char *set);

//split
char **dh_split(const char *s, char c);

//iota
char *dh_iota(int n);

//strmapi
char *dh_strmapi(const char *s, char (*f)(char));

//putchar
void dh_putchar_fd(char c, int fd);

//putstr_fd
void dh_putstr_fd(char *s, int fd);

//putendl_fd
void dh_putendl_fd(char *s, int fd);

//putnbr_fd
void dh_putnbr_fd(int n, int fd);


//list operations
typedef struct s_list
{
    void *content;
    struct s_list *next;
} t_list;

t_list *dh_lstnew(void *content);

void dh_lstadd_front(t_list **lst, t_list *);

int dh_lstsize(t_list *lst);

t_list *dh_lstlast(t_list *lst);

void dh_lstadd_back(t_list **lst, t_list *);

void dh_lstdelone(t_list *lst, void (*del)(void*));

void dh_lstclear(t_list **lst, void (*del)(void *));

void dh_lstiter(t_list *lst, void (*f)(void *));

t_list *dh_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));


//printf
int dh_printf(const char *restrict format, ...);

#endif
