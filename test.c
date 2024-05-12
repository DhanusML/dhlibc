#include "./include/dhlibc.h"
#include <stdio.h>
#include <limits.h>

char toUpper(char c)
{
    return (char)dh_toupper((int)c);
}

int
main()
{
    int a[4] = {1, 2, 3, 4};
    int b[4] = {0, 0, 0, 0};
    int *c = a+1;

    dh_bzero(a, 4*2);
    printf("a: ");
    for (int i=0; i<4; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");

    int *ptr = (int *)dh_memset(a, 255, 4*3);
    printf("a: ");
    for (int i=0; i<4; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");

    int *ptr2 = (int *)dh_memcpy(b, a, 4*3);
    printf("b: ");
    for (int i=0; i<4; i++)
    {
        printf("%d,", b[i]);
    }
    printf("\n");

    printf("c: ");
    for (int i=0; i<3; i++)
    {
        printf("%d, ", c[i]);
    }
    printf("\n");

    int *ptr3 = (int *)dh_memmove(a, c, 4*3);
    printf("a: ");
    for (int i=0; i<4; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");

    char string[] = {'a', 'b', 'c', 'd'};
    char* match = (char *)dh_memchr(string, 'f', 4);
    if(match)
        printf("%c<-%p\n", *match, match);

    int a_cmp_a = dh_memcmp(a, a, 4*4);
    printf("a?a: %d\n", a_cmp_a);
    int a_cmp_b = dh_memcmp(a, b,4*4);
    printf("a?b: %d\n", a_cmp_b);

    char name[] = {'D', 'h', 'a', 'n', 'u', 's', 0};
    char name2[] = {0, 0};
    char name3[] = {'a', 0, 0};
    printf("length of name: %zu\n", dh_strlen(name));
    printf("length of name2: %zu\n", dh_strlen(name2));
    printf("length of name3: %zu\n", dh_strlen(name3));

    size_t len_name1 = dh_strlcpy(name2, name, 2);
    size_t len_name2 = dh_strlcpy(name3, name, 3);
    printf("name2<-name(%zu): %s\n", len_name1, name2);
    printf("name3<-name (%zu): %s\n",len_name2,  name3);
    printf("name3 (%zu): %s\n", dh_strlen(name3), name3);
    size_t len_name3 = dh_strlcpy(name, name3, 7);
    printf("name<-name3 (%zu): %s\n", len_name3, name);
    //size_t len_name4 = dh_strlcpy(name, "dhanus", 7);
    //printf("name<-dhanus (%zu): %s\n", len_name4, name);
    size_t len_name5 = dh_strlcat(name, "anu", 7);
    printf("name||\"anu\" (%zu): %s\n", len_name5, name);
    size_t len_name6 = dh_strlcat(name, "s", 7);
    printf("name||\"s\" (%zu): %s\n", len_name6, name);
    size_t len_name7 = dh_strlcat(name, "", 7);
    printf("name||\"\" (%zu): %s\n", len_name7, name);
    size_t len_name8 = dh_strlcat(name2, name, 2);
    printf("name2||name (%zu): %s\n", len_name8, name2);

    char *hit = dh_strchr("dhanus", 'h'); 
    char *hit1 = dh_strchr("dhanus", 0); 
    char *hit2 = dh_strchr("dhanus", '2'); 
    //printf("h ? dhanus: %c\n", hit?*hit:hit);
    //printf("0 ? dhanus: %c\n", hit1?*hit1:hit1);
    //printf("2 ? dhanus: %c\n", hit2?*hit2:hit2);

    hit = dh_strchrnul("dhanus", 'h'); 
    hit1 = dh_strchrnul("dhanus", 0); 
    hit2 = dh_strchrnul("dhanus", '2'); 
    printf("h ? dhanus: %c\n",*hit);
    printf("0 ? dhanus: %c\n",*hit1);
    printf("2 ? dhanus: %c\n",*hit2);

    char *str_match = dh_strnstr("dhanus", "anu", 6);
    printf("anu in dhanus (6): %s\n", str_match);
    char *str_match1 = dh_strnstr("dhanus", "au", 6);
    printf("au in dhanus (6): %s\n", str_match1);
    char *str_match2 = dh_strnstr("dhanus", "anu", 2);
    printf("anu in dhanus (2): %s\n", str_match2);
    char *str_match3 = dh_strnstr("dhanus", "", 5);
    printf("\"\" in dhanus (5): %s\n", str_match3);
    char *str_match4 = dh_strnstr("", "dhanus", 5);
    printf("dhanus in \"\" (5): %s\n", str_match4);
    char *str_match5 = dh_strnstr("dhanus", "dhanus", 10);
    printf("dhanus in dhanus (5): %s\n", str_match5);

    printf("dhanus?dhanusmlal: %d\n", dh_strncmp("dhanus", "dhanusmlal", 100));
    printf("dhanus?dhanus: %d\n", dh_strncmp("dhanus", "dhanus", 100));
    printf("dhanusmlal?dhanus: %d\n", dh_strncmp("dhanusmlal", "dhanus", 100));
    printf("dhanusmlal?dhanus (first 2): %d\n", dh_strncmp("dhanusmlal", "dhanus", 2));

    //dh_atoi needs more testing
    printf("1000 = %d\n", dh_atoi("1000"));
    printf("dhanus = %d\n", dh_atoi("dhanus"));
    printf("1000000000 = %d\n", dh_atoi("1000000000000"));
    printf("\"\"=%d\n", dh_atoi(""));

    printf("isalpha a, %d\n", dh_isalpha('a'));
    printf("isalpha 0, %d\n", dh_isalpha('0'));
    printf("isalpha (char )0, %d\n", dh_isalpha(0));

    printf("upper(a): %c\n", dh_toupper('a'));
    printf("upper(r): %c\n", dh_toupper('r'));
    printf("upper(z): %c\n", dh_toupper('z'));
    printf("upper(Z): %c\n", dh_toupper('Z'));
    printf("upper(0): %c\n", dh_toupper('0'));
    printf("upper(1000): %c\n", dh_toupper(1000));

    printf("loweer(A): %c\n", dh_tolower('A'));
    printf("loweer(R): %c\n", dh_tolower('R'));
    printf("loweer(Z): %c\n", dh_tolower('Z'));
    printf("loweer(z): %c\n", dh_tolower('z'));
    printf("loweer(0): %c\n", dh_tolower('0'));
    printf("loweer(1000): %c\n", dh_toupper(1000));

    int *zero_arr = dh_calloc(10, sizeof(int));
    for (int i=0; i<10; i++)
    {
        printf("%d,", zero_arr[i]);
    }
    printf("\n");

    char *dhanus = dh_strdup("dhanus");
    printf("%s\n", dhanus);

    free(dhanus);
    free(zero_arr);

    char *dhanus_sub = dh_substr("dhanusmlal", 6, 10);
    char *dhanusmlal = dh_strjoin("dhanus", "mlal");
    printf("%s\n",dhanus_sub);
    printf("%s\n", dhanusmlal);
    free(dhanus_sub);
    free(dhanusmlal);

    char *trimmed_str = dh_strtrim("$ dhanus $", " $");
    char *trimmed_str2 = dh_strtrim("((()))", "()");
    printf("trimmed string:...%s...\n", trimmed_str);
    printf("trimmed string:...%s...\n", trimmed_str2);
    free(trimmed_str);

    //testing split
    char **pieces = dh_split("   dhanus  m    lal  ", ' ');
    char **temp_pieces = pieces;
    char *temp_piece;
    while (*pieces)
    {
        printf("...%s...\n", *pieces);
        temp_piece = *pieces;
        pieces++;
        free(temp_piece++);
    }
    free(temp_pieces);

    char *num_str = dh_iota(INT_MAX+1);
    printf("num as str: %s\n", num_str);
    char *num_str2 = dh_iota(100);
    printf("num as str: %s\n", num_str2);
    free(num_str);
    free(num_str2);

    char *upper_dhanus = dh_strmapi("dhanus m lal 123 #\n", toUpper);
    printf("uppercase: %s\n", upper_dhanus);
    free(upper_dhanus);

    dh_putchar_fd('c', STDOUT_FILENO);
    dh_putchar_fd('\n', STDOUT_FILENO);
    dh_putstr_fd("dhanus\n", STDOUT_FILENO);
    dh_putendl_fd("dhanus", STDOUT_FILENO);
    dh_putnbr_fd(4000, STDOUT_FILENO);
    dh_putchar_fd('\n', STDOUT_FILENO);
    return 0;
}
